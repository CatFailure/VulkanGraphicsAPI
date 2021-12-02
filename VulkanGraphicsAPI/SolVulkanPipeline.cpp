#include "pch.hpp"
#include "SolVulkanPipeline.hpp"

namespace SolEngine
{
    SolVulkanPipeline::SolVulkanPipeline(SolVulkanDevice &rSolVulkanDevice, 
                                         const std::string &vertShaderFilePath, 
                                         const std::string &fragShaderFilePath, 
                                         const PipelineConfigInfo &configInfo)
        : _rSolVulkanDevice(rSolVulkanDevice)
    {
        CreateGraphicsPipeline(vertShaderFilePath, 
                               fragShaderFilePath, 
                               configInfo);
    }

    SolVulkanPipeline::~SolVulkanPipeline()
    {
        Dispose();
    }

    void SolVulkanPipeline::Bind(const VkCommandBuffer &commandBuffer)
    {
        vkCmdBindPipeline(commandBuffer, 
                          VK_PIPELINE_BIND_POINT_GRAPHICS, 
                          _vkGraphicsPipeline);
    }

    void SolVulkanPipeline::DefaultPipelineConfigInfo(PipelineConfigInfo &rConfigInfo)
    {
        const PipelineConfigInfo configInfo
        {
            // Combine Viewport and Scissor
            .viewportStateCreateInfo
            {
                .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .viewportCount = 1,
                .pViewports    = NULL,
                .scissorCount  = 1,
                .pScissors     = NULL
            },
            .inputAssemblyStateCreateInfo
            {
                .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                .primitiveRestartEnable = VK_FALSE
            },
            // Rasterization
            .rasterizationStateCreateInfo
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .depthClampEnable        = VK_FALSE,                    // Forces the z-component of glPosition to be between 0 and 1
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode             = VK_POLYGON_MODE_FILL,
                .cullMode                = VK_CULL_MODE_NONE,           // Face-culling
                .frontFace               = VK_FRONT_FACE_CLOCKWISE,
                .depthBiasEnable         = VK_FALSE,                    // Used to ensure that shadows in a scene are displayed properly
                .depthBiasConstantFactor = 0.0f,                        // Optional
                .depthBiasClamp          = 0.0f,                        // Optional
                .depthBiasSlopeFactor    = 0.0f,                        // Optional
                .lineWidth               = 1.0f
            },
            // Multi-sampling
            .multisampleStateCreateInfo
            {
                .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
                .sampleShadingEnable   = VK_FALSE,
                .minSampleShading      = 1.0f,      // Optional
                .pSampleMask           = NULL,      // Optional
                .alphaToCoverageEnable = VK_FALSE,  // Optional
                .alphaToOneEnable      = VK_FALSE   // Optional
            },
            // Blending
            .colourBlendAttachmentState
            {
                .blendEnable = VK_FALSE,
                .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,     // Optional
                .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,    // Optional
                .colorBlendOp        = VK_BLEND_OP_ADD,         // Optional
                .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,     // Optional
                .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,    // Optional
                .alphaBlendOp        = VK_BLEND_OP_ADD,         // Optional
                .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT |
                                       VK_COLOR_COMPONENT_G_BIT |
                                       VK_COLOR_COMPONENT_B_BIT |
                                       VK_COLOR_COMPONENT_A_BIT,
            },
            .colourBlendStateCreateInfo
            {
                .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .logicOpEnable   = VK_FALSE,
                .logicOp         = VK_LOGIC_OP_COPY,    // Optional
                .attachmentCount = 1,
                .pAttachments    = &configInfo.colourBlendAttachmentState,
                .blendConstants
                {
                    0.0f,   // Optional
                    0.0f,   // Optional
                    0.0f,   // Optional
                    0.0f    // Optional
                }
            },
            // Depth Buffer
            .depthStencilStateCreateInfo
            {
                .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .depthTestEnable       = VK_TRUE,
                .depthWriteEnable      = VK_TRUE,
                .depthCompareOp        = VK_COMPARE_OP_LESS,
                .depthBoundsTestEnable = VK_FALSE,
                .stencilTestEnable     = VK_FALSE,
                .front{},                       // Optional
                .back{},                        // Optional
                .minDepthBounds        = 0.0f,  // Optional
                .maxDepthBounds        = 1.0f,  // Optional
            },
            .dynamicStateEnables 
            { 
                VK_DYNAMIC_STATE_VIEWPORT, 
                VK_DYNAMIC_STATE_SCISSOR 
            },
            .dynamicStateCreateInfo
            {
                .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .flags             = NULL,
                .dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size()),
                .pDynamicStates    = configInfo.dynamicStateEnables.data(),
            }
        };

        rConfigInfo = configInfo;
    }

    void SolVulkanPipeline::Dispose()
    {
    }

    void SolVulkanPipeline::CreateGraphicsPipeline(const std::string &vertShaderFilePath, 
                                                   const std::string &fragShaderFilePath, 
                                                   const PipelineConfigInfo &configInfo)
    {
        // Ensure Config Info is properly set up
        DBG_ASSERT_MSG((configInfo.pipelineLayout != VK_NULL_HANDLE), 
                       "Cannot create Graphics pipeline:: No pipelineLayout provided in configInfo!");

        DBG_ASSERT_MSG((configInfo.renderPass != VK_NULL_HANDLE),
                       "Cannot create Graphics pipeline:: No renderPass provided in configInfo!");
    }

    void SolVulkanPipeline::CreateShaderModule(const std::vector<char> &shaderCode, VkShaderModule *pOutShaderModule)
    {
    }
}