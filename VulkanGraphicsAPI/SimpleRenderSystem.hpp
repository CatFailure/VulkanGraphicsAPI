#pragma once
#include "GenericRenderSystem.hpp"

namespace SolEngine::Rendering
{
    class SimpleRenderSystem : public GenericRenderSystem
    {
    public:
        SimpleRenderSystem() = delete;
        SimpleRenderSystem(SolDevice &rSolDevice, VkRenderPass renderPass);

        void RenderGameObjects(const SolCamera &solCamera, const VkCommandBuffer commandBuffer, const std::vector<SolGameObject> &gameObjects) const;
    };
}
