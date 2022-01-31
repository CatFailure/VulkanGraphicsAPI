#include "pch.hpp"
#include "SolDescriptorPool.hpp"

namespace SolEngine::Descriptors
{
    //======== Builder ========//

    SolDescriptorPool::Builder::Builder(SolDevice &rSolDevice)
        : _rSolDevice(rSolDevice)
    {}

    SolDescriptorPool::Builder &SolDescriptorPool::Builder::AddPoolSize(const VkDescriptorType descriptorType, 
                                                                        const uint32_t count)
    {
        _poolSizes.push_back({ descriptorType, count });

        return *this;
    }

    SolDescriptorPool::Builder &SolDescriptorPool::Builder::SetPoolFlags(const VkDescriptorPoolCreateFlags flags)
    {
        _poolFlags = flags;

        return *this;
    }

    SolDescriptorPool::Builder &SolDescriptorPool::Builder::SetMaxDescriptorSets(const uint32_t count)
    {
        _maxDescriptorSets = count;

        return *this;
    }

    std::unique_ptr<SolDescriptorPool> SolDescriptorPool::Builder::Build() const
    {
        return std::make_unique<SolDescriptorPool>(_rSolDevice,
                                                   _maxDescriptorSets, 
                                                   _poolFlags, 
                                                   _poolSizes);
    }

    //======== SolDescriptorPool ========//

    SolDescriptorPool::SolDescriptorPool(SolDevice &rSolDevice,
                                         const uint32_t maxDescriptorSets, 
                                         const VkDescriptorPoolCreateFlags poolFlags, 
                                         const std::vector<VkDescriptorPoolSize> &poolSizes)
        : _rSolDevice(rSolDevice)
    {
        const VkDescriptorPoolCreateInfo descPoolCreateInfo
        {
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags         = poolFlags,
            .maxSets       = maxDescriptorSets,
            .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
            .pPoolSizes    = poolSizes.data()
        };

        DBG_ASSERT_VULKAN_MSG(vkCreateDescriptorPool(_rSolDevice.GetDevice(), 
                                                     &descPoolCreateInfo,
                                                     NULL, 
                                                     &_descriptorPool), 
                              "Failed to create Descriptor Pool!");
    }

    SolDescriptorPool::~SolDescriptorPool()
    {
        Dispose();
    }

    bool SolDescriptorPool::AllocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, 
                                                  VkDescriptorSet &rDescriptor) const
    {
        const VkDescriptorSetAllocateInfo descSetAllocInfo
        {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool     = _descriptorPool,
            .descriptorSetCount = 1,
            .pSetLayouts        = &descriptorSetLayout,
        };

        // TODO: Make a DescriptorPoolManager to handle instances where 
        // the pool is filled up and adds a new one.
        return vkAllocateDescriptorSets(_rSolDevice.GetDevice(), 
                                        &descSetAllocInfo, 
                                        &rDescriptor) == VK_SUCCESS;
    }

    void SolDescriptorPool::FreeDescriptors(const std::vector<VkDescriptorSet> &descriptors) const
    {
        DBG_ASSERT_VULKAN_MSG(vkFreeDescriptorSets(_rSolDevice.GetDevice(), 
                                                   _descriptorPool, 
                                                   static_cast<uint32_t>(descriptors.size()), 
                                                   descriptors.data()), 
                              "Failed to free Descriptor Sets!");
    }

    void SolDescriptorPool::ResetPool()
    {
        vkResetDescriptorPool(_rSolDevice.GetDevice(), _descriptorPool, 0);
    }

    void SolDescriptorPool::Dispose()
    {
        vkDestroyDescriptorPool(_rSolDevice.GetDevice(), _descriptorPool, NULL);
    }
}