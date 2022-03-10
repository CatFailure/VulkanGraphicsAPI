#include "Application.hpp"

Application::Application(const ApplicationData &appData, 
                         DiagnosticData& rDiagnosticData,
                         GridSettings& rGridSettings, 
                         GameOfLifeSettings& rGameOfLifeSettings)
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
      _rGameOfLifeSettings(rGameOfLifeSettings)
{
    CreateDescriptorPool();

#ifndef DISABLE_IM_GUI
    CreateGuiWindowManager();
#endif  // !DISABLE_IM_GUI

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
    const SimpleRenderSystem renderSystem(_solDevice, 
                                          _solRenderer.GetSwapchainRenderPass());

    if (commandBuffer == nullptr)
    {
        return;
    }

    _solRenderer.BeginSwapchainRenderPass(commandBuffer);

    if (_pSolGrid->IsGridDataValid())
    {
        renderSystem.RenderGameObject(_solCamera, 
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
                                                            _rGameOfLifeSettings);

    _pGameOfLifeSystem->CheckAllCellNeighbours();
}

void Application::SetupEventCallbacks()
{
    _pGameOfLifeSystem->onUpdateAllCellStatesEvent
                      .AddListener([this]() 
                      { 
                          _pMarchingCubesSystem->March(); 
                      });
}

#ifndef DISABLE_IM_GUI
void Application::CreateGuiWindowManager()
{
    const ImGuiWindowFlags flags{ 0 };

    _pGuiWindowManager = std::make_unique<GuiWindowManager>(_solDevice,  
                                                            _solWindow,
                                                            _solRenderer, 
                                                            _pSolDescriptorPool->GetDescriptorPool());

    _pGuiWindowManager->CreateGuiWindow<GuiDiagnosticWindow>("Diagnostics", 
                                                             true, 
                                                             flags, 
                                                             _rDiagnosticData)
                      .CreateGuiWindow<GuiGameOfLifeWindow>("Game of Life Settings", 
                                                            true, 
                                                            flags, 
                                                            _rGameOfLifeSettings);
}
#endif // !DISABLE_IM_GUI
