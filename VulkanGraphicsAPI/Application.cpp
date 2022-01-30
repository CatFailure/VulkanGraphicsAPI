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
    _pSolDescriptorPool = SolDescriptorPool::Builder(_solDevice).SetMaxDescriptorSets(SolSwapchain::MAX_FRAMES_IN_FLIGHT)
                                                                .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 
                                                                             SolSwapchain::MAX_FRAMES_IN_FLIGHT)
                                                                .Build();

    _pGuiWindowManager = std::make_unique<GuiWindowManager>(_solDevice,  
                                                            _solWindow,
                                                            _solRenderer, 
                                                            _pSolDescriptorPool->GetDescriptorPool());

    _pGuiWindowManager->CreateGuiWindow<GuiDiagnosticWindow>("Diagnostics", true, 0, _diagnosticData);

    const PerspectiveProjectionInfo projInfo
    {
        .fovDeg = 50.f
    };

    _solCamera.SetProjectionInfo(projInfo);
    _solCamera.SetPosition({ 0, 0, -2.5f });
    _solCamera.LookAt(_solCamera.GetPosition() + VECTOR3_AXIS_Z);   // Look forwards

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
        _diagnosticData.deltaTime = deltaTime;

        // Start Dear ImGui frame...
        _pGuiWindowManager->NewFrame();

        Update(deltaTime);
        Draw();
    }

    // Make CPU wait until all GPU operations have completed.
    vkDeviceWaitIdle(_solDevice.GetDevice());
}

std::shared_ptr<SolModel> Application::CreateCubeModel(SolDevice &rDevice, 
                                                       const glm::vec3 &offset)
{
    std::vector<Vertex> vertices
    {
        { { -.5f, .5f, .5f }, { .9f, .9f, .9f } },      // 0
        { { .5f, .5f, .5f }, { .8f, .8f, .1f } },       // 1
        { { .5f, .5f, -.5f }, { 1.f, .6f, .1f } },      // 2
        { { -.5f, .5f, -.5f }, { .8f, .1f, .1f } },     // 3
        { { -.5f, -.5f, .5f }, { .1f, .1f, .8f } },     // 4
        { { .5f, -.5f, .5f }, { .98f, .27f, .41f } },   // 5
        { { .5f, -.5f, -.5f }, { .24f, .36f, .98f } },  // 6
        { { -.5f, -.5f, -.5f }, { .13f, .02f, .3f } },  // 7
    };

    const std::vector<Index_t> indices
    {
        // Bottom-Face
        0, 1, 2,
        0, 2, 3,

        // Back-Face
        0, 4, 5,
        0, 5, 1,

        // Right-Face
        1, 5, 6,
        1, 6, 2,

        // Front-Face
        2, 6, 7,
        2, 7, 3,

        // Left-Face
        3, 7, 4,
        3, 4, 0,

        // Top-Face
        4, 7, 6,
        4, 6, 5,
    };

    if (offset != glm::vec3(0, 0, 0))
    {
        for (Vertex &rVertex : vertices) 
        {
            rVertex.position += offset;
        }
    }
    
    return std::make_shared<SolModel>(rDevice, vertices, indices);
}

void Application::Dispose()
{
    // Guarantee Descriptor Pool and GuiWindowManager are destructed before SolDevice
    _pSolDescriptorPool = nullptr;
    _pGuiWindowManager = nullptr;
}

void Application::Update(const float deltaTime)
{
    _solCamera.Update(deltaTime);
    _pGuiWindowManager->Update(deltaTime);

    for (SolGameObject &rGameObject : _gameObjects)
    {
        const float scaledTwoPi = deltaTime * glm::two_pi<float>();

        rGameObject.transform.rotation.y += 0.1f * scaledTwoPi;
        rGameObject.transform.rotation.x += 0.05f * scaledTwoPi;

        rGameObject.transform.position.y = sinf(_solClock.GetTotalTime());
    }
}

void Application::Draw()
{
    const VkCommandBuffer commandBuffer = _solRenderer.BeginFrame();
    const SimpleRenderSystem renderSystem(_solDevice, _solRenderer.GetSwapchainRenderPass());

    if (commandBuffer == nullptr)
    {
        return;
    }

    _solRenderer.BeginSwapchainRenderPass(commandBuffer);

    renderSystem.RenderGameObjects(_solCamera, commandBuffer, _gameObjects);

    // Render Dear ImGui...
    _pGuiWindowManager->Render(commandBuffer);

    _solRenderer.EndSwapchainRenderPass(commandBuffer);
    _solRenderer.EndFrame();
}

void Application::LoadGameObjects()
{
    std::shared_ptr<SolModel> cubeModel = CreateCubeModel(_solDevice, { 0,0,0 });
    SolGameObject cubeGameObject = SolGameObject::CreateGameObject();

    cubeGameObject.SetModel(cubeModel);

    cubeGameObject.transform.position = { 0, 0, 0 };

    _gameObjects.push_back(std::move(cubeGameObject));
}
