#pragma once
#include "SolDevice.hpp"
#include "SolGameObject.hpp"
#include "SolPipeline.hpp"
#include "SolCamera.hpp"

namespace SolEngine
{
    class SimpleRenderSystem : private IDisposable
    {
    public:
        SimpleRenderSystem() = delete;
        SimpleRenderSystem(SolDevice &rSolDevice, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        void RenderGameObjects(const SolCamera &solCamera, const VkCommandBuffer commandBuffer, const std::vector<SolGameObject> &gameObjects) const;

    private:
        // Inherited via IDisposable
        void virtual Dispose() override;

        void CreatePipelineLayout();
        void CreatePipeline(VkRenderPass renderPass);

        SolDevice                   &_rSolDevice;
        std::unique_ptr<SolPipeline> _pSolPipeline{ nullptr };
        VkPipelineLayout             _pipelineLayout;
    };
}
