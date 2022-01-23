#include "pch.hpp"
#include "Application.hpp"

namespace SolEngine
{
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
    }

    Application::~Application()
    {
        Dispose();
    }

    void Application::Run()
    {
        // TEMP
        const float deltaTime = 1 / 60.f;

        while (!_solWindow.ShouldClose())
        {
            glfwPollEvents();   // Poll Window Events

            Update(deltaTime);
            Draw();
        }

        // Make CPU wait until all GPU operations have completed.
        vkDeviceWaitIdle(_solDevice.Device());
    }

    std::shared_ptr<SolVulkanModel> Application::CreateCubeModel(SolVulkanDevice &rDevice, 
                                                                 const glm::vec3 &offset)
    {
        std::vector<Vertex> vertices
        {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };

        for (Vertex &rVertex : vertices) 
        {
            rVertex.position += offset;
        }

        return std::make_shared<SolVulkanModel>(rDevice, vertices);
    }

    void Application::Dispose()
    {}

    void Application::Update(const float deltaTime)
    {
        for (SolVulkanGameObject &rGameObject : _gameObjects)
        {
            const float scaledTwoPi = deltaTime * glm::two_pi<float>();

            rGameObject.transform.rotation.y += 0.01f * scaledTwoPi;
            rGameObject.transform.rotation.x += 0.005f * scaledTwoPi;
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

        renderSystem.RenderGameObjects(commandBuffer, _gameObjects);

        _solRenderer.EndSwapchainRenderPass(commandBuffer);
        _solRenderer.EndFrame();
    }

    void Application::LoadGameObjects()
    {
        std::shared_ptr<SolVulkanModel> cubeModel = CreateCubeModel(_solDevice, { 0,0,0 });
        SolVulkanGameObject cubeGameObject = SolVulkanGameObject::CreateGameObject();

        cubeGameObject.SetModel(cubeModel);

        cubeGameObject.transform.position = { 0, 0, .5f };
        cubeGameObject.transform.scale    = { .5f, .5f, .5f };

        _gameObjects.push_back(std::move(cubeGameObject));
    }
}
