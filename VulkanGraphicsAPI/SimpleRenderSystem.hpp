#pragma once
#include "GenericRenderSystem.hpp"

namespace SolEngine::Rendering
{
    class SimpleRenderSystem : public GenericRenderSystem
    {
    public:
        SimpleRenderSystem() = delete;
        SimpleRenderSystem(SolDevice &rSolDevice, const VkRenderPass renderPass, VkDescriptorSetLayout globalDescSetLayout);

        void RenderGameObject(const SolCamera &solCamera, const VkCommandBuffer commandBuffer, 
                              const VkDescriptorSet globalDescriptorSet, const SolGameObject &gameObject) const;
        void RenderGameObjects(const SolCamera &solCamera, const VkCommandBuffer commandBuffer, const std::vector<SolGameObject> &gameObjects) const;
    };
}
