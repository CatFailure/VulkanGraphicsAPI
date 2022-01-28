#include "pch.hpp"
#include "SimpleRenderSystem.hpp"

namespace SolEngine::Rendering
{
    SimpleRenderSystem::SimpleRenderSystem(SolDevice& rSolDevice, 
                                           VkRenderPass renderPass)
        : _rSolDevice(rSolDevice)
    {
        CreatePipelineLayout();
        CreatePipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem()
    {
        Dispose();
    }

    void SimpleRenderSystem::RenderGameObjects(const SolCamera &solCamera,
                                               const VkCommandBuffer commandBuffer, 
                                               const std::vector<SolGameObject> &gameObjects) const
    {
        const glm::mat4 projectionView = solCamera.GetProjectionViewMatrix();

        _pSolPipeline->Bind(commandBuffer);

        for (const SolGameObject& gameObject : gameObjects)
        {
            const std::shared_ptr<SolModel>& pGameObjectModel = gameObject.GetModel();

            const SimplePushConstantData pushConstantData
            {
                .transform = projectionView * gameObject.transform.TransformMatrix(),
                .colour    = gameObject.GetColour()
            };

            vkCmdPushConstants(commandBuffer,
                               _pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0,
                               sizeof(SimplePushConstantData),
                               &pushConstantData);


            pGameObjectModel->Bind(commandBuffer);
            pGameObjectModel->Draw(commandBuffer);
        }
    }

    void SimpleRenderSystem::Dispose()
    {
        vkDestroyPipelineLayout(_rSolDevice.GetDevice(), 
                                _pipelineLayout, 
                                nullptr);
    }

    void SimpleRenderSystem::CreatePipelineLayout()
    {
        const VkPushConstantRange pushConstantRange
        {
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,    // Allow access to push constant data and both vertex/frag shaders.
            .offset     = 0,
            .size       = sizeof(SimplePushConstantData)
        };

        const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo
        {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount         = 0,
            .pSetLayouts            = NULL,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges    = &pushConstantRange
        };

        const VkResult result = vkCreatePipelineLayout(_rSolDevice.GetDevice(),
                                                       &pipelineLayoutCreateInfo,
                                                       NULL,
                                                       &_pipelineLayout);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Pipeline Layout.");
    }

    void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
    {
        DBG_ASSERT_MSG((_pipelineLayout != nullptr), 
                       "Cannot create Pipeline before Pipeline Layout.");

        PipelineConfigInfo pipelineConfigInfo{};
        SolPipeline::DefaultPipelineConfigInfo(pipelineConfigInfo);

        // TEMP
        pipelineConfigInfo.renderPass     = renderPass;
        pipelineConfigInfo.pipelineLayout = _pipelineLayout;

        _pSolPipeline = std::make_unique<SolPipeline>(_rSolDevice,
                                                            "Shaders/SimpleShader.vert.spv",
                                                            "Shaders/SimpleShader.frag.spv",
                                                            pipelineConfigInfo);
    }
}