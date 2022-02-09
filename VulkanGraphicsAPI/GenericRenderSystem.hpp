#pragma once
#include "SolDevice.hpp"
#include "SolGameObject.hpp"
#include "SolPipeline.hpp"
#include "SolCamera.hpp"
#include "SimplePushConstantData.hpp"

using namespace SolEngine::Data;

namespace SolEngine::Rendering
{
	class GenericRenderSystem : private IDisposable
	{
	public:
		GenericRenderSystem() = delete;
		GenericRenderSystem(SolDevice &rSolDevice, const VkRenderPass renderPass);
		virtual ~GenericRenderSystem();

	protected:
		// Inherited via IDisposable
		virtual void Dispose() override;

		virtual void CreatePipelineLayout();
		virtual void CreatePipeline(const VkRenderPass renderPass);

		SolDevice                   &_rSolDevice;
		std::unique_ptr<SolPipeline> _pSolPipeline{ nullptr };
		VkPipelineLayout             _pipelineLayout;
	};
}
