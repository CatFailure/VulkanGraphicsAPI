#include "MarchingCubesRenderSystem.hpp"

namespace SolEngine::Rendering
{
    MarchingCubesRenderSystem::MarchingCubesRenderSystem(SolDevice &rSolDevice, 
                                                         VkRenderPass renderPass)
        : GenericRenderSystem(rSolDevice, renderPass)
    {}

    void MarchingCubesRenderSystem::Render(const SolCamera &solCamera, 
                                           const VkCommandBuffer commandBuffer, 
                                           const std::shared_ptr<SolModel> pMarchingCubesModel) const
    {
        /*const glm::mat4 projectionView = solCamera.GetProjectionViewMatrix();

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
        }*/
    }
}