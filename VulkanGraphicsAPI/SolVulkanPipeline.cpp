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
        rConfigInfo = PipelineConfigInfo
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
                .maxDepthBounds        = 1.0f   // Optional
            },
            .dynamicStateEnables 
            { 
                VK_DYNAMIC_STATE_VIEWPORT, 
                VK_DYNAMIC_STATE_SCISSOR 
            },
            .dynamicStateCreateInfo
            {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .flags = NULL
            }
        };

        // We have to set these afterwards or else they will be <???>
        rConfigInfo.colourBlendStateCreateInfo.pAttachments = &rConfigInfo.colourBlendAttachmentState;

        rConfigInfo.dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(rConfigInfo.dynamicStateEnables.size());
        rConfigInfo.dynamicStateCreateInfo.pDynamicStates    = rConfigInfo.dynamicStateEnables.data();
    }

    void SolVulkanPipeline::Dispose()
    {
        const VkDevice &device = _rSolVulkanDevice.Device();

        vkDestroyShaderModule(device, 
                              _vkVertexShaderModule, 
                              NULL);

        vkDestroyShaderModule(device, 
                              _vkFragmentShaderModule,
                              NULL);

        vkDestroyPipeline(device, 
                          _vkGraphicsPipeline, 
                          NULL);
    }

    std::vector<char> SolVulkanPipeline::ReadFile(const std::string &filePath)
    {
        const std::string openFileFailedMessage = "Failed to open file: " + filePath;

        // ate = seek to the end of the open file immediately.
        // binary = read file in as a binary.
        std::ifstream file(filePath,
            std::ios::ate | std::ios::binary);

        DBG_ASSERT_MSG(file.is_open(), openFileFailedMessage.c_str());

        const size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> fileBuffer(fileSize);

        // Read from file into buffer
        file.seekg(0);
        file.read(fileBuffer.data(), fileSize);
        file.close();

        return fileBuffer;
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

        const std::vector<char> vertexShaderCode = ReadFile(vertShaderFilePath);
        const std::vector<char> fragShaderCode = ReadFile(fragShaderFilePath);
        const char *shaderStageName = "main";

        CreateShaderModule(vertexShaderCode, &_vkVertexShaderModule);
        CreateShaderModule(fragShaderCode, &_vkFragmentShaderModule);

        const std::array<VkPipelineShaderStageCreateInfo, 2> shaderStageCreateInfos
        {
            // Vertex Shader
            VkPipelineShaderStageCreateInfo
            {
                .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .pNext               = NULL,
                .flags               = 0,
                .stage               = VK_SHADER_STAGE_VERTEX_BIT,
                .module              = _vkVertexShaderModule,
                .pName               = shaderStageName,
                .pSpecializationInfo = NULL             // Customise Shader functionality
            },
            // Fragment Shader
            VkPipelineShaderStageCreateInfo
            {
                .sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .pNext               = NULL,
                .flags               = 0,
                .stage               = VK_SHADER_STAGE_FRAGMENT_BIT,
                .module              = _vkFragmentShaderModule,
                .pName               = shaderStageName,
                .pSpecializationInfo = NULL             // Customise Shader functionality
            }
        };

        const std::vector<VkVertexInputBindingDescription> inputBindingDescriptions     = Vertex::InputBindingDescriptors();
        const std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions = Vertex::InputAttributeDescriptions();

        // Describe how to interpret Vertex Buffer data
        // that is the initial input into the Graphics Pipeline
        const VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo
        {
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount   = static_cast<uint32_t>(inputBindingDescriptions.size()),
            .pVertexBindingDescriptions      = inputBindingDescriptions.data(),
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(inputAttributeDescriptions.size()),
            .pVertexAttributeDescriptions    = inputAttributeDescriptions.data()
        };

        const VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo
        {
            .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount          = static_cast<uint32_t>(shaderStageCreateInfos.size()),
            .pStages             = shaderStageCreateInfos.data(),
            .pVertexInputState   = &vertexInputStateCreateInfo,
            .pInputAssemblyState = &configInfo.inputAssemblyStateCreateInfo,
            .pViewportState      = &configInfo.viewportStateCreateInfo,
            .pRasterizationState = &configInfo.rasterizationStateCreateInfo,
            .pMultisampleState   = &configInfo.multisampleStateCreateInfo,
            .pDepthStencilState  = &configInfo.depthStencilStateCreateInfo,
            .pColorBlendState    = &configInfo.colourBlendStateCreateInfo,
            .pDynamicState       = &configInfo.dynamicStateCreateInfo,
            .layout              = configInfo.pipelineLayout,
            .renderPass          = configInfo.renderPass,
            .subpass             = configInfo.subpass,
            .basePipelineHandle  = VK_NULL_HANDLE,
            .basePipelineIndex   = -1,
        };

        const VkResult result = vkCreateGraphicsPipelines(_rSolVulkanDevice.Device(), 
                                                          VK_NULL_HANDLE, 
                                                          1,
                                                          &graphicsPipelineCreateInfo, 
                                                          NULL, 
                                                          &_vkGraphicsPipeline);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Graphics Pipeline.");
    }

    void SolVulkanPipeline::CreateShaderModule(const std::vector<char> &shaderCode, 
                                               VkShaderModule *pOutShaderModule)
    {
        const VkShaderModuleCreateInfo shaderModuleCreateInfo
        {
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = shaderCode.size(),
            .pCode    = reinterpret_cast<const uint32_t *>(shaderCode.data())
        };

        const VkResult result = vkCreateShaderModule(_rSolVulkanDevice.Device(), 
                                                     &shaderModuleCreateInfo, 
                                                     NULL, 
                                                     pOutShaderModule);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Shader Module.");
    }
}