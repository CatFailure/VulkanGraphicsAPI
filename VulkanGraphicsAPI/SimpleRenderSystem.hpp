#pragma once
#include "GenericRenderSystem.hpp"

namespace SolEngine::Rendering
{
    class SimpleRenderSystem : public GenericRenderSystem
    {
    public:
        SimpleRenderSystem() = delete;
        SimpleRenderSystem(const std::string& workingDir, SolDevice& rSolDevice, RenderSettings& rRenderSettings, VkRenderPass renderPass);

        void RenderGameObject(const SolCamera& solCamera, const VkCommandBuffer commandBuffer, const SolGameObject& gameObject) const;
        void RenderGameObjects(const SolCamera& solCamera, const VkCommandBuffer commandBuffer, const std::vector<SolGameObject>& gameObjects) const;
    };
}
