#include "pch.hpp"
#include "SolDescriptorSetLayout.hpp"

namespace SolEngine::Descriptors
{
    SolDescriptorSetLayout::Builder::Builder(SolDevice &rSolDevice)
        : _rSolDevice(rSolDevice)
    {}

    SolDescriptorSetLayout::Builder &SolDescriptorSetLayout::Builder::AddBinding(const uint32_t binding, 
                                                                                 const VkDescriptorType descriptorType,
                                                                                 const VkShaderStageFlags stageFlags, 
                                                                                 const uint32_t count)
    {
        // TODO: insert return statement here
    }

    std::unique_ptr<SolDescriptorSetLayout> SolDescriptorSetLayout::Builder::Build() const
    {
        return std::unique_ptr<SolDescriptorSetLayout>();
    }

    SolDescriptorSetLayout::SolDescriptorSetLayout(SolDevice &rSolDevice, const DescSetLayoutBindingsUMap_t &bindings)
        : _rSolDevice(rSolDevice),
          _descSetLayoutBindings(bindings)
    {}

    SolDescriptorSetLayout::~SolDescriptorSetLayout()
    {
        Dispose();
    }

    void SolDescriptorSetLayout::Dispose()
    {
        vkDestroyDescriptorSetLayout(_rSolDevice.GetDevice(), _descriptorSetLayout, NULL);
    }
}