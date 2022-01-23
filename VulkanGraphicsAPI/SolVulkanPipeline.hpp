#pragma once
#include "SolVulkanDevice.hpp"

namespace SolEngine
{
    class SolVulkanPipeline : public IDisposable
    {
    public:
        SolVulkanPipeline() = default;
        SolVulkanPipeline(SolVulkanDevice &rSolDevice, const std::string &vertShaderFilePath, const std::string &fragShaderFilePath, const PipelineConfigInfo &configInfo);
        ~SolVulkanPipeline();

        void        Bind(const VkCommandBuffer commandBuffer);
        static void DefaultPipelineConfigInfo(PipelineConfigInfo &rConfigInfo);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        static std::vector<char> ReadFile(const std::string &filePath);

        void CreateGraphicsPipeline(const std::string &vertShaderFilePath, const std::string &fragShaderFilePath, const PipelineConfigInfo &configInfo);
        void CreateShaderModule(const std::vector<char> &shaderCode, VkShaderModule *pOutShaderModule);

        SolVulkanDevice &_rSolDevice;
        
        VkPipeline _vkGraphicsPipeline;

        VkShaderModule _vkVertexShaderModule;
        VkShaderModule _vkFragmentShaderModule;
    };
}
