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
        CreatePipelineLayout();
        CreatePipeline();
    }

    void Application::Dispose()
    {
        vkDestroyPipelineLayout(_solDevice.Device(), 
                                _vkPipelineLayout, 
                                NULL);
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

    void Application::Update(const float deltaTime)
    {
    }

    void Application::Draw()
    {
        const VkCommandBuffer commandBuffer = _solRenderer.BeginFrame();

        if (commandBuffer == nullptr)
        {
            return;
        }

        _solRenderer.BeginSwapchainRenderPass(commandBuffer);

        RenderGameObjects(commandBuffer);

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
        triangle.transform2D.scale = { 1.f, 1.f };
        
        _gameObjects.push_back(std::move(triangle));
    }

    void Application::RenderGameObjects(const VkCommandBuffer commandBuffer)
    {
        _pSolPipeline->Bind(commandBuffer);

        for (const SolVulkanGameObject &gameObject : _gameObjects)
        {
            const std::shared_ptr<SolVulkanModel> &pGameObjectModel = gameObject.GetModel();

            const SimplePushConstantData pushConstantData
            {
                .transform = gameObject.transform2D.Mat2(),
                .offset    = gameObject.transform2D.position,
                .colour    = gameObject.GetColour(),
            };

            vkCmdPushConstants(commandBuffer, 
                               _vkPipelineLayout, 
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0, 
                               sizeof(SimplePushConstantData), 
                               &pushConstantData);


            pGameObjectModel->Bind(commandBuffer);
            pGameObjectModel->Draw(commandBuffer);
        }
    }

    void Application::CreatePipelineLayout()
    {
        const VkPushConstantRange pushConstantRange
        {
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,    // Allow access to push constant data and both vertex/frag shaders.
            .offset = 0,
            .size = sizeof(SimplePushConstantData)
        };

        const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo
        {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount         = 0,
            .pSetLayouts            = NULL,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges    = &pushConstantRange
        };

        const VkResult result = vkCreatePipelineLayout(_solDevice.Device(),
                                                       &pipelineLayoutCreateInfo, 
                                                       NULL, 
                                                       &_vkPipelineLayout);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Pipeline Layout.");
    }

    void Application::CreatePipeline()
    {
        DBG_ASSERT_MSG((_vkPipelineLayout != nullptr), "Cannot create Pipeline before Pipeline Layout.");

        PipelineConfigInfo pipelineConfigInfo{};
        SolVulkanPipeline::DefaultPipelineConfigInfo(pipelineConfigInfo);

        // TEMP
        pipelineConfigInfo.renderPass = _solRenderer.GetSwapchainRenderPass();
        pipelineConfigInfo.pipelineLayout = _vkPipelineLayout;

        _pSolPipeline = std::make_unique<SolVulkanPipeline>(_solDevice,
                                                                  "Shaders/SimpleShader.vert.spv",
                                                                  "Shaders/SimpleShader.frag.spv",
                                                                  pipelineConfigInfo);
    }
}
