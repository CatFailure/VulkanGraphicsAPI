#pragma once
#include <vulkan/vulkan.hpp>

namespace SolEngine::Data
{
    // Data specifying how to configure the pipeline
    struct PipelineConfigInfo
    {
        VkPipelineViewportStateCreateInfo	   viewportStateCreateInfo{};
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
        VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
        VkPipelineMultisampleStateCreateInfo   multisampleStateCreateInfo{};
        VkPipelineColorBlendAttachmentState    colourBlendAttachmentState{};
        VkPipelineColorBlendStateCreateInfo    colourBlendStateCreateInfo{};
        VkPipelineDepthStencilStateCreateInfo  depthStencilStateCreateInfo{};
        std::vector<VkDynamicState>            dynamicStateEnables{};
        VkPipelineDynamicStateCreateInfo       dynamicStateCreateInfo{};
        VkPipelineLayout                       pipelineLayout{NULL};
        VkRenderPass                           renderPass{NULL};
        uint32_t                               subpass{0};
    };
}
