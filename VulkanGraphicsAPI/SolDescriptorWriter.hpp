#pragma once
#include "SolDescriptorSetLayout.hpp"
#include "SolDescriptorPool.hpp"

namespace SolEngine::Descriptors
{
    class SolDescriptorWriter
    {
    public:
        SolDescriptorWriter(SolDescriptorSetLayout &rSolDescSetLayout, SolDescriptorPool &rSolDescPool);

        SolDescriptorWriter &WriteBuffer(const uint32_t binding, const VkDescriptorBufferInfo *pBufferInfo);
        SolDescriptorWriter &WriteImage(const uint32_t binding, const VkDescriptorBufferInfo *pBufferInfo);

        bool Build(VkDescriptorSet &rDescSet);
        void Overwrite(VkDescriptorSet &rDescSet);

    private:
        SolDescriptorSetLayout &          _rSolDescSetLayout;
        SolDescriptorPool &               _rSolDescPool;
        std::vector<VkWriteDescriptorSet> _writes;
    };
}


