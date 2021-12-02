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
        // TODO
        rConfigInfo =
        {
            
        };
    }

    void SolVulkanPipeline::Dispose()
    {
    }

    void SolVulkanPipeline::CreateGraphicsPipeline(const std::string &vertShaderFilePath, 
                                                   const std::string &fragShaderFilePath, 
                                                   const PipelineConfigInfo &configInfo)
    {
    }
}