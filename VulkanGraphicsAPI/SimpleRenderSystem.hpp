#pragma once
#include "SolVulkanDevice.hpp"
#include "SolVulkanGameObject.hpp"
#include "SolVulkanPipeline.hpp"

namespace SolEngine
{
    class SimpleRenderSystem : private IDisposable
    {
    public:
        SimpleRenderSystem() = delete;
        SimpleRenderSystem(SolVulkanDevice &rSolDevice, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        void RenderGameObjects(const VkCommandBuffer commandBuffer, const std::vector<SolVulkanGameObject> &gameObjects) const;

    private:
        // Inherited via IDisposable
        void virtual Dispose() override;

        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);

        SolVulkanDevice &_rSolDevice;

        std::unique_ptr<SolVulkanPipeline> _pSolPipeline{ nullptr };
        VkPipelineLayout                   _pipelineLayout;
    };
}
