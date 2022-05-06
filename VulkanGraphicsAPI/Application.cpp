#include "Application.hpp"

Application::Application(const ApplicationData& appData, 
                         DiagnosticData& rDiagnosticData, 
                         SettingsBundle& rSettings)
    : _performanceProfiler(_appData.exeDirectory.string()),
      _solRenderer(_appData, 
                   _solWindow,
                   _solDevice),
      _solDevice(_solWindow,
                 _appData),
      _solWindow(_appData.windowTitle,
                 _appData.windowDimensions,
                 _rSettings),
      _appData(appData),
      _rDiagnosticData(rDiagnosticData),
      _rSettings(rSettings)
{
    CreateDescriptorPool();

#ifndef DISABLE_IM_GUI
    CreateGuiWindowManager();
#endif  // !DISABLE_IM_GUI

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
    Cursor&    rCursor           = Cursor::GetInstance();
    Transform& rGameObjTransform = _pMarchingCubesSystem->GetGameObject().transform;

    HandleUserInput(rGameObjTransform);

    _pSolCamera->LookAt(rGameObjTransform.position);
    _pSolCamera->Update(deltaTime);

    CheckForSimulationResetFlag();
    CheckForGridDimenionsChangedFlag();

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
    const VkCommandBuffer commandBuffer   = _solRenderer.BeginFrame();
    RenderSettings&       rRenderSettings = _rSettings.renderSettings;

    if (rRenderSettings.isRendererOutOfDate)
    {
        _pRenderSystem = std::make_unique<SimpleRenderSystem>(_appData.exeDirectory.string(),
                                                              _solDevice, 
                                                              rRenderSettings, 
                                                              _solRenderer.GetSwapchainRenderPass());

        rRenderSettings.isRendererOutOfDate = false;
    }

    if (commandBuffer == nullptr)
    {
        return;
    }

    _solRenderer.BeginSwapchainRenderPass(commandBuffer);

    if (_pSolGrid->IsGridDataValid())
    {
        _pRenderSystem->RenderGameObject(*_pSolCamera, 
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
    RandomNumberGenerator::SetSeed(_rSettings.simulationSettings.seed);
}

void Application::SetupCamera()
{
    _pSolCamera = std::make_unique<SolCamera>(_solRenderer,
                                              _rSettings.cameraSettings);

    _pSolCamera->LookAt(_pSolCamera->GetPosition() - VEC3_FORWARD);    // Look forwards
}

void Application::SetupGrid()
{
    _pSolGrid = std::make_unique<SolGrid>(_rSettings.gridSettings, 
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
                                                            _rSettings.gameOfLifeSettings,
                                                            _rSettings.simulationSettings);

    _pGameOfLifeSystem->CheckAllCellNeighbours();
}

void Application::SetupEventCallbacks()
{
    _pGameOfLifeSystem->onUpdateAllCellStatesEvent
                      .AddListener([this]() 
                      { 
                          _pMarchingCubesSystem->March(); 
                      });

    _rSettings.simulationSettings.onSimulationSpeedChangedEvent
                        .AddListener([this](const float speed) 
                        {
                            // Force update the next generation delay to the new value
                            _pGameOfLifeSystem->ResetNextGenerationDelayRemaining();
                        });

    _rSettings.gameOfLifeSettings.onNeighbourhoodTypeChangedEvent
                        .AddListener([this]() 
                        {
                            // Re-check neighbours in accordance
                            // to new neighbourhood ruleset
                            _pGameOfLifeSystem->CheckAllCellNeighbours();
                        });

    SettingsFileLoader::GetInstance().onFileLoadedEvent
                                     .AddListener([this](const SettingsBundle& settings)
                                     {
                                          // Queue up a simulation reset to reflect settings file changes
                                          _rSettings.simulationSettings.isSimulationResetRequested = true;
                                          _rSettings.gridSettings.isGridDimensionsChangeRequested = true;
                                     });
}

void Application::HandleUserInput(Transform& rGameObjectTransform)
{
    Cursor& rCursor = Cursor::GetInstance();

    if (_rSettings.cameraSettings.isMouseOverGUI)
    {
        rCursor.UpdateLastMousePosition();

        return;
    }

    const glm::dvec2 mouseDelta = rCursor.GetMouseDelta();

    if (rCursor.IsButtonDown(MouseButton::LEFT))
    {
        const glm::vec3 rotation(0.f, mouseDelta.x, -mouseDelta.y);
        rGameObjectTransform.rotation += (rotation * GAME_OBJECT_ROT_SPEED);
    }

    if (rCursor.IsButtonDown(MouseButton::RIGHT))
    {
        _pSolCamera->Move({ 0.f, 0.f, mouseDelta.y * CAMERA_MOVE_SPEED });
    }

    rCursor.UpdateLastMousePosition();
}

void Application::CheckForSimulationResetFlag()
{
    SimulationSettings& rSimulationSettings = _rSettings.simulationSettings;

    if (!rSimulationSettings.isSimulationResetRequested)
    {
        return;
    }

    // Reset the seed to generate new values 
    // OR keep same values for repeatable simulations
    RandomNumberGenerator::SetSeed(rSimulationSettings.seed);

    _pSolGrid->Reset();                             // Reset the grid nodes and re-generate initial node states
    _pMarchingCubesSystem->March();                 // Create the reset vertices
    _pGameOfLifeSystem->CheckAllCellNeighbours();   // Retrieve the next generation state

    // Finished!
    rSimulationSettings.isSimulationResetRequested = false;
}

void Application::CheckForGridDimenionsChangedFlag()
{
    GridSettings& rGridSettings = _rSettings.gridSettings;

    if (!rGridSettings.isGridDimensionsChangeRequested)
    {
        return;
    }

    // Reset the seed to generate new values 
    // OR keep same values for repeatable simulations
    RandomNumberGenerator::SetSeed(_rSettings.simulationSettings.seed);

    _pSolGrid->Initialise();                                // Re-initialise the Grid
    _pMarchingCubesSystem->ResetVerticesContainerSize();    // Shrink vertex container to free up wasted memory
    _pMarchingCubesSystem->March();                         // Create the new vertices
    _pGameOfLifeSystem->CheckAllCellNeighbours();           // Retrieve the next generation state

    // Finished!
    rGridSettings.isGridDimensionsChangeRequested = false;
}

#ifndef DISABLE_IM_GUI
void Application::CreateGuiWindowManager()
{
    const ImGuiWindowFlags flags{ ImGuiWindowFlags_AlwaysAutoResize };

    _pGuiWindowManager = std::make_unique<GuiWindowManager>(_solDevice,
                                                            _rCameraSettings,
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
                                                          _rSimulationSettings,
                                                          _rGridSettings,
                                                          _rRenderSettings);
}
#endif // !DISABLE_IM_GUI
