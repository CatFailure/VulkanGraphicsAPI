#include "Application.hpp"

Application::Application(const ApplicationData &appData, 
                         DiagnosticData& rDiagnosticData,
                         GridSettings& rGridSettings, 
                         GameOfLifeSettings& rGameOfLifeSettings,
                         SimulationSettings& rSimulationSettings)
    : _solCamera(_solRenderer),
      _solRenderer(_appData,
                   _solWindow, 
                   _solDevice),
      _solDevice(_solWindow,
                 _appData),
      _solWindow(_appData.windowTitle,
                 _appData.windowDimensions),
      _appData(appData),
      _rDiagnosticData(rDiagnosticData),
      _rGridSettings(rGridSettings),
      _rGameOfLifeSettings(rGameOfLifeSettings),
      _rSimulationSettings(rSimulationSettings)
{
    CreateDescriptorPool();

#ifndef DISABLE_IM_GUI
    CreateGuiWindowManager();
#endif  // !DISABLE_IM_GUI

    SetupRenderSystem();
    SetupRandomNumberGenerator();
    SetupCamera();
    SetupGrid();
    SetupMarchingCubesSystem();
    SetupGameOfLifeSystem();
    SetupEventCallbacks();
}

Application::~Application()
{
    // Guarantee Descriptor Pool and GuiWindowManager are destructed before SolDevice
    _pSolDescriptorPool = nullptr;

#ifndef DISABLE_IM_GUI
    _pGuiWindowManager = nullptr;
#endif
}

void Application::Run()
{
    const uint32_t maxFramesInFlight = SolSwapchain::MAX_FRAMES_IN_FLIGHT;

    std::unique_ptr<SolBuffer> uniformBufferObjects[maxFramesInFlight];

    for (size_t i = 0; i < maxFramesInFlight; ++i)
    {
        uniformBufferObjects[i] = std::make_unique<SolBuffer>(_solDevice,
                                                              sizeof(GlobalUniformBufferObject),
                                                              1U, 
                                                              VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

        uniformBufferObjects[i]->Map();
    }

    std::unique_ptr<SolDescriptorSetLayout> pGlobalSetLayout = 
        SolDescriptorSetLayout::Builder(_solDevice).AddBinding(0, 
                                                               VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                               VK_SHADER_STAGE_VERTEX_BIT)
                                                   .Build();

    VkDescriptorSet globalDescriptorSets[maxFramesInFlight];

    for (size_t i = 0; i < maxFramesInFlight; ++i)
    {
        const VkDescriptorBufferInfo descriptorBufferInfo = uniformBufferObjects[i]->DescriptorBufferInfo();

        SolDescriptorWriter(*pGlobalSetLayout, 
                            *_pSolDescriptorPool).WriteBuffer(0U,
                                                              &descriptorBufferInfo)
                                                 .Build(globalDescriptorSets[i]);
    }

    while (!_solWindow.ShouldClose())
    {
        glfwPollEvents();   // Poll Window Events

        const float deltaTime = _solClock.Restart();

#ifndef DISABLE_IM_GUI
        _rDiagnosticData.deltaTimeSeconds = deltaTime;
        _rDiagnosticData.totalTimeSeconds = _solClock.GetTotalTime();

        // Start Dear ImGui frame...
        _pGuiWindowManager->NewFrame();
#endif  // !DISABLE_IM_GUI

        Update(deltaTime);
        Render();
    }

    // Make CPU wait until all GPU operations have completed.
    vkDeviceWaitIdle(_solDevice.GetDevice());
}

void Application::Update(const float deltaTime)
{
    _solCamera.LookAt(_pMarchingCubesSystem->GetGameObject()
                                           .transform
                                           .position);

    _solCamera.Update(deltaTime);

    CheckForSimulationResetFlag();

    if (_pSolGrid->IsGridDataValid())
    {
        _pGameOfLifeSystem->Update(deltaTime);
    }
    else
    {
        printf_s("Bad Grid data, cannot update Game of Life!\n");
    }

#ifndef DISABLE_IM_GUI
    _pGuiWindowManager->Update(deltaTime);
#endif  // !DISABLE_IM_GUI
}

void Application::Render()
{
    const VkCommandBuffer commandBuffer = _solRenderer.BeginFrame();

    if (commandBuffer == nullptr)
    {
        return;
    }

    _solRenderer.BeginSwapchainRenderPass(commandBuffer);

    if (_pSolGrid->IsGridDataValid())
    {
        _pRenderSystem->RenderGameObject(_solCamera, 
                                         commandBuffer, 
                                         _pMarchingCubesSystem->GetGameObject());
    }
    else
    {
        printf_s("Bad Grid data, cannot render GameObject!\n");
    }

#ifndef DISABLE_IM_GUI
    // Render Dear ImGui...
    _pGuiWindowManager->Render(commandBuffer);
#endif  // !DISABLE_IM_GUI

    _solRenderer.EndSwapchainRenderPass(commandBuffer);
    _solRenderer.EndFrame();
}

void Application::CreateDescriptorPool()
{
    _pSolDescriptorPool = SolDescriptorPool::Builder(_solDevice).SetMaxDescriptorSets(SolSwapchain::MAX_FRAMES_IN_FLIGHT)
                                                                .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 
                                                                             SolSwapchain::MAX_FRAMES_IN_FLIGHT)
                                                                .Build();
}

void Application::SetupRandomNumberGenerator()
{
    RandomNumberGenerator::SetSeed(_rSimulationSettings.seed);
}

void Application::SetupRenderSystem()
{
    _pRenderSystem = std::make_unique<SimpleRenderSystem>(_solDevice, 
                                                          _solRenderer.GetSwapchainRenderPass());
}

void Application::SetupCamera()
{
    const PerspectiveProjectionInfo projInfo
    {
        .fovDeg = 50.f,
        .far    = 250.f
    };

    _solCamera.SetProjectionInfo(projInfo)
              .SetPosition({ 35.f, 2.5f, 55.f })
              .LookAt(_solCamera.GetPosition() + VEC3_FORWARD);    // Look forwards
}

void Application::SetupGrid()
{
    _pSolGrid = std::make_unique<SolGrid>(_rGridSettings, 
                                          _rDiagnosticData);
}

void Application::SetupMarchingCubesSystem()
{
    _pMarchingCubesSystem = std::make_unique<MarchingCubesSystem>(_solDevice, 
                                                                  *_pSolGrid,
                                                                  _rDiagnosticData);

    _pMarchingCubesSystem->March();
}

void Application::SetupGameOfLifeSystem()
{
    _pGameOfLifeSystem = std::make_unique<GameOfLifeSystem>(*_pSolGrid, 
                                                            _rGameOfLifeSettings,
                                                            _rSimulationSettings);

    _pGameOfLifeSystem->CheckAllCellNeighbours();
}

void Application::SetupEventCallbacks()
{
    _pGameOfLifeSystem->onUpdateAllCellStatesEvent
                      .AddListener([this]() 
                      { 
                          _pMarchingCubesSystem->March(); 
                      });

    _rSimulationSettings.onSimulationSpeedChangedEvent
                        .AddListener([this](const float speed) 
                        {
                            // Force update the next generation delay to the new value
                            _pGameOfLifeSystem->ResetNextGenerationDelayRemaining();
                        });
}

void Application::CheckForSimulationResetFlag()
{
    if (!_rSimulationSettings.isSimulationResetRequested)
    {
        return;
    }

    // Reset the seed to generate new values 
    // OR keep same values for repeatable simulations
    RandomNumberGenerator::SetSeed(_rSimulationSettings.seed);

    // Reset the grid nodes and re-generate node states
    _pSolGrid->Reset();

    // Force Game of Life to re-check live neighbours
    _pGameOfLifeSystem->ForceUpdateCellStates();

    // Finished!
    _rSimulationSettings.isSimulationResetRequested = false;
}

#ifndef DISABLE_IM_GUI
void Application::CreateGuiWindowManager()
{
    const ImGuiWindowFlags flags{ ImGuiWindowFlags_AlwaysAutoResize };

    _pGuiWindowManager = std::make_unique<GuiWindowManager>(_solDevice,  
                                                            _solWindow,
                                                            _solRenderer, 
                                                            _pSolDescriptorPool->GetDescriptorPool());

    _pGuiWindowManager->CreateGuiWindow<GuiDiagnosticWindow>(TITLE_DIAGNOSTICS,
                                                             true,
                                                             flags,
                                                             _rDiagnosticData)
                      .CreateGuiWindow<GuiSettingsWindow>(TITLE_SETTINGS, 
                                                          true, 
                                                          flags, 
                                                          _rGameOfLifeSettings,
                                                          _rSimulationSettings);
}
#endif // !DISABLE_IM_GUI
