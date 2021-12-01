#pragma once
#include "SolVulkanDevice.hpp"

namespace SolEngine
{
    struct PipelineConfigInfo {};

    class SolVulkanPipeline : public IDisposable
    {
    public:
        SolVulkanPipeline() = default;
        SolVulkanPipeline(SolVulkanDevice &rSolVulkanDevice, const std::string &vertShaderFilePath, const std::string &fragShaderFilePath, const PipelineConfigInfo &configInfo);
        ~SolVulkanPipeline();

        void Bind(const VkCommandBuffer &commandBuffer);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void CreateGraphicsPipeline(const std::string &vertShaderFilePath, const std::string &fragShaderFilePath, const PipelineConfigInfo &configInfo);

        SolVulkanDevice &_rSolVulkanDevice;
        
        VkPipeline _vkGraphicsPipeline;

        VkShaderModule _vkVertexShaderModule;
        VkShaderModule _vkFragmentShaderModule;
    };
}
