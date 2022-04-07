#include "GenericRenderSystem.hpp"

namespace SolEngine::Rendering
{
    GenericRenderSystem::GenericRenderSystem(SolDevice& rSolDevice, 
                                             const VkRenderPass renderPass)
        : _rSolDevice(rSolDevice)
    {
        CreatePipelineLayout();
        CreatePipeline(renderPass);
    }

    GenericRenderSystem::~GenericRenderSystem()
    {
        Dispose();
    }

    void GenericRenderSystem::Dispose()
    {
        vkDestroyPipelineLayout(_rSolDevice.GetDevice(), 
                                _pipelineLayout, 
                                nullptr);
    }

    void GenericRenderSystem::CreatePipelineLayout()
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

        DBG_ASSERT_VULKAN_MSG(result, 
                              "Failed to Create Pipeline Layout.");
    }

    void GenericRenderSystem::CreatePipeline(const VkRenderPass renderPass)
    {
        DBG_ASSERT_MSG((_pipelineLayout != nullptr), 
                       "Cannot create Pipeline before Pipeline Layout.");

        PipelineConfigInfo pipelineConfigInfo{};
        SolPipeline::DefaultPipelineConfigInfo(pipelineConfigInfo);

        pipelineConfigInfo.renderPass     = renderPass;
        pipelineConfigInfo.pipelineLayout = _pipelineLayout;

        _pSolPipeline = std::make_unique<SolPipeline>(_rSolDevice,
                                                      "Shaders/SimpleShader.vert.spv",
                                                      "Shaders/SimpleShader.frag.spv",
                                                      pipelineConfigInfo);
    }
}