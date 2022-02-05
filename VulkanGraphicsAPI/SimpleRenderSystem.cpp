#include "pch.hpp"
#include "SimpleRenderSystem.hpp"

namespace SolEngine::Rendering
{
    SimpleRenderSystem::SimpleRenderSystem(SolDevice& rSolDevice, 
                                           VkRenderPass renderPass)
        : GenericRenderSystem(rSolDevice, 
                              renderPass)
    {}

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
                .transform = projectionView * gameObject.transform.TransformMatrix(),
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