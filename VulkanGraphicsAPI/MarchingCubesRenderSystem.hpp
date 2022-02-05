#pragma once
#include "GenericRenderSystem.hpp"

namespace SolEngine::Rendering
{
	class MarchingCubesRenderSystem : public GenericRenderSystem
	{
	public:
		MarchingCubesRenderSystem() = delete;
		MarchingCubesRenderSystem(SolDevice &rSolDevice, VkRenderPass renderPass);

		void Render(const SolCamera &solCamera, const VkCommandBuffer commandBuffer, const std::shared_ptr<SolModel> pMarchingCubesModel) const;
	};
}
