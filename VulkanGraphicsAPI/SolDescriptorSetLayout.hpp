#pragma once
#include "SolDevice.hpp"

namespace SolEngine::Descriptors
{
	class SolDescriptorSetLayout : private IDisposable
	{
	public:
		typedef std::pair<uint32_t, VkDescriptorSetLayoutBinding> DescSetLayoutBindingsPair_t;
		typedef std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> DescSetLayoutBindingsUMap_t;

		/// <summary>
		/// Helper class that builds the descriptor set layouts.
		/// </summary>
		class Builder
		{
		public:
			Builder(SolDevice &rSolDevice);

			/// <summary>
			/// Appends to the map of bindings.
			/// </summary>
			/// <param name="binding"></param>
			/// <param name="descriptorType">What type of descriptor type to expect. i.e ImageBuffer, UniformBuffer, etc.</param>
			/// <param name="stageFlags">Which shader stages has access to this binding?</param>
			/// <param name="count">Descriptor count held by the binding.</param>
			/// <returns></returns>
			Builder &AddBinding(const uint32_t binding, const VkDescriptorType descriptorType, const VkShaderStageFlags stageFlags, const uint32_t count = 1U);

			/// <summary>
			/// Creates instance of DescriptorSetLayout wrapper.
			/// </summary>
			/// <returns></returns>
			std::unique_ptr<SolDescriptorSetLayout> Build() const;

		private:
			SolDevice &_rSolDevice;
			DescSetLayoutBindingsUMap_t _descSetLayoutBindings;
		};

		SolDescriptorSetLayout(SolDevice &rSolDevice, const DescSetLayoutBindingsUMap_t &bindings);
		~SolDescriptorSetLayout();

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return _descriptorSetLayout; }
	private:
		// Inherited via IDisposable
		virtual void Dispose() override;

		SolDevice &_rSolDevice;
		VkDescriptorSetLayout _descriptorSetLayout;
		DescSetLayoutBindingsUMap_t _descSetLayoutBindings;

		friend class SolDescriptorWriter;
	};
}
