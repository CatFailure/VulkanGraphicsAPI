#include "SolDescriptorSetLayout.hpp"

namespace SolEngine::Descriptors
{
    //======== Builder ========//

    SolDescriptorSetLayout::Builder::Builder(SolDevice &rSolDevice)
        : _rSolDevice(rSolDevice)
    {}

    SolDescriptorSetLayout::Builder &SolDescriptorSetLayout::Builder::AddBinding(const uint32_t binding, 
                                                                                 const VkDescriptorType descriptorType,
                                                                                 const VkShaderStageFlags stageFlags, 
                                                                                 const uint32_t count)
    {
        DBG_ASSERT_MSG(_descSetLayoutBindings.count(binding) == 0, 
                       "Binding already in use!");

        const VkDescriptorSetLayoutBinding descSetLayoutBinding
        {
            .binding         = binding,
            .descriptorType  = descriptorType,
            .descriptorCount = count,
            .stageFlags      = stageFlags
        };

        _descSetLayoutBindings[binding] = descSetLayoutBinding;

        return *this;
    }

    std::unique_ptr<SolDescriptorSetLayout> SolDescriptorSetLayout::Builder::Build() const
    {
        return std::make_unique<SolDescriptorSetLayout>(_rSolDevice, _descSetLayoutBindings);
    }

    //======== Descriptor Set Layout ========//

    SolDescriptorSetLayout::SolDescriptorSetLayout(SolDevice &rSolDevice, const DescSetLayoutBindingsUMap_t &bindings)
        : _rSolDevice(rSolDevice),
          _descSetLayoutBindings(bindings)
    {
        std::vector<VkDescriptorSetLayoutBinding> descSetLayoutBindings{};

        for (const DescSetLayoutBindingsPair_t &layoutBindingsPair : bindings)
        {
            descSetLayoutBindings.push_back(layoutBindingsPair.second);
        }

        const VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo
        {
            .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<uint32_t>(descSetLayoutBindings.size()),
            .pBindings    = descSetLayoutBindings.data()
        };

        DBG_ASSERT_VULKAN_MSG(vkCreateDescriptorSetLayout(_rSolDevice.GetDevice(), 
                                                          &descSetLayoutCreateInfo, 
                                                          NULL, 
                                                          &_descriptorSetLayout), 
                              "Failed to create Descriptor Set Layout!");
    }

    SolDescriptorSetLayout::~SolDescriptorSetLayout()
    {
        Dispose();
    }

    void SolDescriptorSetLayout::Dispose()
    {
        vkDestroyDescriptorSetLayout(_rSolDevice.GetDevice(), _descriptorSetLayout, NULL);
    }
}