#pragma once
#include "SolDevice.hpp"
#include "SolGameObject.hpp"
#include "SolPipeline.hpp"
#include "SolCamera.hpp"
#include "SimplePushConstantData.hpp"
#include "RenderSettings.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Settings;

namespace SolEngine::Rendering
{
	class GenericRenderSystem : private IDisposable
	{
	public:
		GenericRenderSystem() = delete;
		GenericRenderSystem(SolDevice& rSolDevice, RenderSettings& rRenderSettings, const VkRenderPass renderPass);
		virtual ~GenericRenderSystem();

	protected:
		// Inherited via IDisposable
		virtual void Dispose() override;

		virtual void CreatePipelineLayout();
		virtual void CreatePipeline(const VkRenderPass renderPass);

		SolDevice&					 _rSolDevice;
		RenderSettings&				 _rRenderSettings;
		std::unique_ptr<SolPipeline> _pSolPipeline{ nullptr };
		VkPipelineLayout             _pipelineLayout;
	};
}
