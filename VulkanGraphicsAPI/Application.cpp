#include "pch.hpp"
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
    SetupMarchingCubesManager();

    LoadGameObjects();
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

std::shared_ptr<SolModel> Application::CreateCubeModel(SolDevice &rSolDevice)
{    
    return std::make_shared<SolModel>(rSolDevice, 
                                      CUBE_VERTICES, 
                                      CUBE_VERTEX_COUNT * CUBE_VERTEX_COUNT);
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
    _pMarchingCubesManager->Update(deltaTime);
    _solCamera.Update(deltaTime);

#ifndef DISABLE_IM_GUI
    _pGuiWindowManager->Update(deltaTime);
#endif  // !DISABLE_IM_GUI

    for (SolGameObject &rGameObject : _gameObjects)
    {
        rGameObject.transform.rotation.y += .5f * deltaTime;
    }
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

    renderSystem.RenderGameObjects(_solCamera, commandBuffer, _gameObjects);

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
        .fovDeg = 50.f
    };

    _solCamera.SetProjectionInfo(projInfo)
              .SetPosition({ 0, 0, -25.f })
              .LookAt(_solCamera.GetPosition() + VEC3_FORWARD);    // Look forwards
}

void Application::SetupMarchingCubesManager()
{
    // Create a 5x5x5 grid for testing...
    _pMarchingCubesManager = std::make_unique<MarchingCubesManager>(_solDevice, 20);
}

#ifndef DISABLE_IM_GUI
void Application::CreateGuiWindowManager()
{
    _pGuiWindowManager = std::make_unique<GuiWindowManager>(_solDevice,  
                                                            _solWindow,
                                                            _solRenderer, 
                                                            _pSolDescriptorPool->GetDescriptorPool());

    _pGuiWindowManager->CreateGuiWindow<GuiDiagnosticWindow>("Diagnostics", true, 0, _diagnosticData);
}
#endif // !DISABLE_IM_GUI

void Application::LoadGameObjects()
{
    std::shared_ptr<SolModel> marchingCubeModel = _pMarchingCubesManager->CreateModel();

    if (marchingCubeModel == nullptr)
    {
        return;
    }

    SolGameObject marchingCubeGameObject = SolGameObject::CreateGameObject();
    marchingCubeGameObject.SetModel(marchingCubeModel);

    _gameObjects.push_back(std::move(marchingCubeGameObject));
}
