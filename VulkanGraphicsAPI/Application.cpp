#include "Application.hpp"

Application::Application(const ApplicationData &appData)
    : _solCamera(_solRenderer),
      _solRenderer(_appData,
                   _solWindow, 
                   _solDevice),
      _solDevice(_solWindow,
                 _appData),
      _solWindow(_appData.windowTitle,
                 _appData.windowDimensions),
      _appData(appData)
{
    CreateDescriptorPool();

#ifndef DISABLE_IM_GUI
    CreateGuiWindowManager();
#endif  // !DISABLE_IM_GUI

    SetupCamera();

    SetupGrid();
    SetupMarchingCubesSystem();
}

Application::~Application()
{
    Dispose();
}

void Application::Run()
{
    while (!_solWindow.ShouldClose())
    {
        glfwPollEvents();   // Poll Window Events

        const float deltaTime = _solClock.Restart();

#ifndef DISABLE_IM_GUI
        _diagnosticData.deltaTimeSeconds = deltaTime;
        _diagnosticData.totalTimeSeconds = _solClock.GetTotalTime();

        // Start Dear ImGui frame...
        _pGuiWindowManager->NewFrame();
#endif  // !DISABLE_IM_GUI

        Update(deltaTime);
        Render();
    }

    // Make CPU wait until all GPU operations have completed.
    vkDeviceWaitIdle(_solDevice.GetDevice());
}

void Application::Dispose()
{
    // Guarantee Descriptor Pool and GuiWindowManager are destructed before SolDevice
    _pSolDescriptorPool = nullptr;

#ifndef DISABLE_IM_GUI
    _pGuiWindowManager = nullptr;
#endif
}

void Application::Update(const float deltaTime)
{
    _solCamera.Update(deltaTime);

#ifndef DISABLE_IM_GUI
    _pGuiWindowManager->Update(deltaTime);
#endif  // !DISABLE_IM_GUI
}

void Application::Render()
{
    const VkCommandBuffer commandBuffer = _solRenderer.BeginFrame();
    const SimpleRenderSystem renderSystem(_solDevice, _solRenderer.GetSwapchainRenderPass());

    if (commandBuffer == nullptr)
    {
        return;
    }

    _solRenderer.BeginSwapchainRenderPass(commandBuffer);

#ifndef DISABLE_IM_GUI
    // Render Dear ImGui...
    _pGuiWindowManager->Render(commandBuffer);
#endif  // !DISABLE_IM_GUI

    renderSystem.RenderGameObject(_solCamera, 
                                  commandBuffer, 
                                  _pMarchingCubesSystem->GetGameObject());

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
        .fovDeg = 50.f
    };

    _solCamera.SetProjectionInfo(projInfo)
              .SetPosition({ 0, 0, -25.f })
              .LookAt(_solCamera.GetPosition() + VEC3_FORWARD);    // Look forwards
}

void Application::SetupGrid()
{
    const glm::uvec3 gridDimensions(20);

    _pSolGrid = std::make_unique<SolGrid>(gridDimensions, 
                                          _diagnosticData);
}

void Application::SetupMarchingCubesSystem()
{
    _pMarchingCubesSystem = std::make_unique<MarchingCubesSystem>(_marchingCubesData, 
                                                                  _solDevice);

    _pMarchingCubesSystem->GenerateIsoValues(*_pSolGrid, _diagnosticData);
    _pMarchingCubesSystem->March(*_pSolGrid, _diagnosticData);
}

#ifndef DISABLE_IM_GUI
void Application::CreateGuiWindowManager()
{
    _pGuiWindowManager = std::make_unique<GuiWindowManager>(_solDevice,  
                                                            _solWindow,
                                                            _solRenderer, 
                                                            _pSolDescriptorPool->GetDescriptorPool());

    _pGuiWindowManager->CreateGuiWindow<GuiDiagnosticWindow>("Diagnostics", true, 0, _diagnosticData);
    _pGuiWindowManager->CreateGuiWindow<GuiMarchingCubesWindow>("Marching Cubes Settings", true, 0, _marchingCubesData);
}
#endif // !DISABLE_IM_GUI
