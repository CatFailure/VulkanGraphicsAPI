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

    void Application::Dispose()
    {}

    void Application::Update(const float deltaTime)
    {
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
        const std::vector<Vertex> vertices
        {
            {{ 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
            {{ 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }},
            {{ -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }}
        };

        std::shared_ptr<SolVulkanModel> pModel = std::make_shared<SolVulkanModel>(_solDevice, vertices);
        SolVulkanGameObject triangle = SolVulkanGameObject::CreateGameObject();

        triangle.SetModel(pModel);
        triangle.SetColour({ .1f, .8f, .1f });
        triangle.transform2D.position.x = .2f;
        triangle.transform2D.scale = { 2.f, .5f };
        //triangle.transform2D.rotation = 0.25f * glm::two_pi<float>();
        
        _gameObjects.push_back(std::move(triangle));
    }
}
