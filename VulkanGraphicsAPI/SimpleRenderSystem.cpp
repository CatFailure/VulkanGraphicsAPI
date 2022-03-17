#include "SimpleRenderSystem.hpp"

namespace SolEngine::Rendering
{
    SimpleRenderSystem::SimpleRenderSystem(SolDevice& rSolDevice, 
                                           const VkRenderPass renderPass,
                                           VkDescriptorSetLayout globalDescSetLayout)
        : GenericRenderSystem(rSolDevice, 
                              renderPass,
                              globalDescSetLayout)
    {}

    void SimpleRenderSystem::RenderGameObject(const SolCamera& solCamera,
                                              const VkCommandBuffer commandBuffer, 
                                              const VkDescriptorSet globalDescriptorSet,
                                              const SolGameObject& gameObject) const
    {
        if (gameObject.GetModel() == nullptr)
        {
            // No model to bind/draw
            return;
        }

        //const glm::mat4 projectionView = solCamera.GetProjectionViewMatrix();

        _pSolPipeline->Bind(commandBuffer);

        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                _pipelineLayout,
                                0U, 
                                1U, 
                                &globalDescriptorSet, 
                                0U, 
                                nullptr);

        const SimplePushConstantData pushConstantData
        {
            .modelMatrix = gameObject.transform.GetModelMatrix(),
            .colour      = gameObject.GetColour()
        };

        vkCmdPushConstants(commandBuffer,
                           _pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                           0,
                           sizeof(SimplePushConstantData),
                           &pushConstantData);


        const std::shared_ptr<SolModel>& pGameObjectModel = gameObject.GetModel();

        pGameObjectModel->Bind(commandBuffer);
        pGameObjectModel->Draw(commandBuffer);
    }

    void SimpleRenderSystem::RenderGameObjects(const SolCamera &solCamera,
                                               const VkCommandBuffer commandBuffer, 
                                               const std::vector<SolGameObject> &gameObjects) const
    {
        const glm::mat4 projectionView = solCamera.GetProjectionViewMatrix();

        _pSolPipeline->Bind(commandBuffer);

        for (const SolGameObject& gameObject : gameObjects)
        {
            const std::shared_ptr<SolModel>& pGameObjectModel = gameObject.GetModel();

            const SimplePushConstantData pushConstantData
            {
                .modelMatrix = projectionView * gameObject.transform.GetModelMatrix(),
                .colour    = gameObject.GetColour()
            };

            vkCmdPushConstants(commandBuffer,
                               _pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0,
                               sizeof(SimplePushConstantData),
                               &pushConstantData);


            pGameObjectModel->Bind(commandBuffer);
            pGameObjectModel->Draw(commandBuffer);
        }
    }
}