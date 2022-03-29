#pragma once
#include <iostream>
#include <fstream>

#include "PipelineConfigInfo.hpp"
#include "SolDevice.hpp"
#include "Vertex.hpp"

using namespace SolEngine::Data;

namespace SolEngine
{
    class SolPipeline : public IDisposable
    {
    public:
        SolPipeline() = default;
        SolPipeline(SolDevice& rSolDevice, const std::string& vertShaderFilePath, 
                    const std::string& fragShaderFilePath, const PipelineConfigInfo& configInfo);
        ~SolPipeline();

        void        Bind(const VkCommandBuffer commandBuffer);
        static void DefaultPipelineConfigInfo(PipelineConfigInfo& rConfigInfo);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        static std::vector<char> ReadFile(const std::string& filePath);

        void CreateGraphicsPipeline(const std::string& vertShaderFilePath, const std::string& fragShaderFilePath, const PipelineConfigInfo& configInfo);
        void CreateShaderModule(const std::vector<char>& shaderCode, VkShaderModule* pOutShaderModule);

        SolDevice& _rSolDevice;
        
        VkPipeline _graphicsPipeline;

        VkShaderModule _vertexShaderModule;
        VkShaderModule _fragmentShaderModule;
    };
}
