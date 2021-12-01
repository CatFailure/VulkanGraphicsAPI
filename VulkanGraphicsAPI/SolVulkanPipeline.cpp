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

    void SolVulkanPipeline::Dispose()
    {
    }

    void SolVulkanPipeline::CreateGraphicsPipeline(const std::string &vertShaderFilePath, 
                                                   const std::string &fragShaderFilePath, 
                                                   const PipelineConfigInfo &configInfo)
    {
    }
}