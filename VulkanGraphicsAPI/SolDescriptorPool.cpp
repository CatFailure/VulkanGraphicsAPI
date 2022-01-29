#include "pch.hpp"
#include "SolDescriptorPool.hpp"

namespace SolEngine::Descriptors
{
    SolDescriptorPool::Builder::Builder(SolDevice &rSolDevice)
        : _rSolDevice(rSolDevice)
    {}

    SolDescriptorPool::Builder &SolDescriptorPool::Builder::AddPoolSize(const VkDescriptorType descriptorType, 
                                                                        const uint32_t count)
    {
        // TODO: insert return statement here
    }

    SolDescriptorPool::Builder &SolDescriptorPool::Builder::SetPoolFlags(const VkDescriptorPoolCreateFlags flags)
    {
        // TODO: insert return statement here
    }

    SolDescriptorPool::Builder &SolDescriptorPool::Builder::SetMaxDescriptorSets(const uint32_t count)
    {
        // TODO: insert return statement here
    }

    std::unique_ptr<SolDescriptorPool> SolDescriptorPool::Builder::Build() const
    {

    }

    SolDescriptorPool::SolDescriptorPool(SolDevice &rSolDevice,
                                         const uint32_t maxDescriptorSets, 
                                         const VkDescriptorPoolCreateFlags poolFlags, 
                                         const std::vector<VkDescriptorPoolSize> &poolSizes)
        : _rSolDevice(rSolDevice)
    {
    }

    SolDescriptorPool::~SolDescriptorPool()
    {
        Dispose();
    }

    bool SolDescriptorPool::AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, 
                                               VkDescriptorSet &rDescriptor) const
    {
        return false;
    }

    void SolDescriptorPool::FreeDescriptors(const std::vector<VkDescriptorSet> &descriptors) const
    {
    }

    void SolDescriptorPool::ResetPool()
    {
    }

    void SolDescriptorPool::Dispose()
    {
    }
}