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
        DBG_ASSERT_MSG(_rSolDescSetLayout._descSetLayoutBindings.count(binding) == 1,
                       "Layout does not contain specified binding!");

        const VkDescriptorSetLayoutBinding &descSetLayoutBinding = 
            _rSolDescSetLayout._descSetLayoutBindings[binding];

        DBG_ASSERT_MSG(descSetLayoutBinding.descriptorCount == 1, 
                       "Binding single Descriptor Info, but Binding excepts multiple!");

        const VkWriteDescriptorSet writeDescSet
        {
            .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstBinding      = binding,
            .descriptorCount = 1,
            .descriptorType  = descSetLayoutBinding.descriptorType,
            .pBufferInfo     = pBufferInfo
        };

        _writeDescSets.push_back(writeDescSet);

        return *this;
    }

    SolDescriptorWriter &SolDescriptorWriter::WriteImage(const uint32_t binding, 
                                                         const VkDescriptorImageInfo *pImageInfo)
    {
        DBG_ASSERT_MSG(_rSolDescSetLayout._descSetLayoutBindings.count(binding) == 1,
                       "Layout does not contain specified binding!");

        const VkDescriptorSetLayoutBinding &descSetLayoutBinding = 
            _rSolDescSetLayout._descSetLayoutBindings[binding];

        DBG_ASSERT_MSG(descSetLayoutBinding.descriptorCount == 1, 
                       "Binding single Descriptor Info, but Binding excepts multiple!");

        const VkWriteDescriptorSet writeDescSet
        {
            .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstBinding      = binding,
            .descriptorCount = 1,
            .descriptorType  = descSetLayoutBinding.descriptorType,
            .pImageInfo      = pImageInfo
        };

        _writeDescSets.push_back(writeDescSet);

        return *this;
    }

    bool SolDescriptorWriter::Build(VkDescriptorSet &rDescSet)
    {
        const bool isSuccess = 
            _rSolDescPool.AllocateDescriptorSet(_rSolDescSetLayout.GetDescriptorSetLayout(), 
                                                rDescSet);

        if (!isSuccess)
        {
            return false;
        }

        Overwrite(rDescSet);

        return true;
    }

    void SolDescriptorWriter::Overwrite(const VkDescriptorSet &newDescSet)
    {
        for (VkWriteDescriptorSet &rWriteDescSet : _writeDescSets)
        {
            rWriteDescSet.dstSet = newDescSet;
        }

        vkUpdateDescriptorSets(_rSolDescPool._rSolDevice.GetDevice(), 
                               static_cast<uint32_t>(_writeDescSets.size()), 
                               _writeDescSets.data(), 
                               0,
                               NULL);
    }
}