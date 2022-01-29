#include "pch.hpp"
#include "SolDescriptorWriter.hpp"

namespace SolEngine::Descriptors
{
    SolDescriptorWriter::SolDescriptorWriter(SolDescriptorSetLayout &rSolDescSetLayout, 
                                             SolDescriptorPool &rSolDescPool)
        : _rSolDescSetLayout(rSolDescSetLayout),
          _rSolDescPool(rSolDescPool)
    {}

    SolDescriptorWriter &SolDescriptorWriter::WriteBuffer(const uint32_t binding, 
                                                          const VkDescriptorBufferInfo *pBufferInfo)
    {
        // TODO: insert return statement here
    }

    SolDescriptorWriter &SolDescriptorWriter::WriteImage(const uint32_t binding, 
                                                         const VkDescriptorBufferInfo *pBufferInfo)
    {
        // TODO: insert return statement here
    }

    bool SolDescriptorWriter::Build(VkDescriptorSet &rDescSet)
    {
        return false;
    }

    void SolDescriptorWriter::Overwrite(VkDescriptorSet &rDescSet)
    {
    }
}