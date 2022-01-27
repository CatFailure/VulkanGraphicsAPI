#include "pch.hpp"
#include "Application.hpp"

Application::Application(const ApplicationData &appData)
    : _solRenderer(_appData,
                   _solWindow, 
                   _solDevice),
        _solDevice(_solWindow,
                   _appData),
        _solWindow(_appData.windowTitle,
                   _appData.windowDimensions),
        _appData(appData)
{
    LoadGameObjects();

    _solCamera.SetView({ .5f, -1.f, -2.5f }, 
                       { 10.f, 0, 0 });
}

Application::~Application()
{
    Dispose();
}

void Application::Run()
{
    SolClock clock{};

    while (!_solWindow.ShouldClose())
    {
        glfwPollEvents();   // Poll Window Events

        const float deltaTime = clock.Restart();

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
        { { 0, 0, 0 }, { .9f, .9f, .9f } },
        { { 1.f, 0, 0 }, { .8f, .8f, .1f } },
        { { 1.f, 0, -1.f }, { 1.f, .6f, .1f } },
        { { 0, 0, -1.f }, { .8f, .1f, .1f } },
        { { 0, -1.f, 0 }, { .1f, .1f, .8f } },
        { { 1.f, -1.f, 0 }, { .98f, .27f, .41f } },
        { { 1.f, -1.f, -1.f }, { .24f, .36f, .98f } },
        { { 0, -1.f, -1.f }, { .13f, .02f, .3f } },
    };

    const std::vector<Index_t> indices
    {
        // Bottom
        0, 1, 2,
        0, 2, 3,
        0, 4, 5,
        0, 5, 1,
        1, 5, 6,
        1, 6, 2,
        2, 6, 7,
        2, 7, 3,
        3, 7, 4,
        3, 4, 0,
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
{}

void Application::Update(const float deltaTime)
{
    for (SolGameObject &rGameObject : _gameObjects)
    {
        const float scaledTwoPi = deltaTime * glm::two_pi<float>();

        rGameObject.transform.rotation.y += 0.1f * scaledTwoPi;
        rGameObject.transform.rotation.x += 0.05f * scaledTwoPi;

        // Camera will look at the cube

        //_solCamera.LookAt(glm::vec3{ 0.f, -2.f, -0.f }, 
        //                    rGameObject.transform.position);
    }
}

void Application::Draw()
{
    const VkCommandBuffer commandBuffer = _solRenderer.BeginFrame();
    const SimpleRenderSystem renderSystem(_solDevice, _solRenderer.GetSwapchainRenderPass());
    const float aspectRatio = _solRenderer.GetAspectRatio();

    _solCamera.SetPerspectiveProjection(50.f, aspectRatio, CAM_NEAR, CAM_FAR);

    if (commandBuffer == nullptr)
    {
        return;
    }

    _solRenderer.BeginSwapchainRenderPass(commandBuffer);

    renderSystem.RenderGameObjects(_solCamera, commandBuffer, _gameObjects);

    _solRenderer.EndSwapchainRenderPass(commandBuffer);
    _solRenderer.EndFrame();
}

void Application::LoadGameObjects()
{
    std::shared_ptr<SolModel> cubeModel = CreateCubeModel(_solDevice, { 0,0,0 });
    SolGameObject cubeGameObject = SolGameObject::CreateGameObject();

    cubeGameObject.SetModel(cubeModel);

    cubeGameObject.transform.position = { 0, 0, 2.5f };
    //cubeGameObject.transform.scale    = { .5f, .5f, .5f };

    _gameObjects.push_back(std::move(cubeGameObject));
}
