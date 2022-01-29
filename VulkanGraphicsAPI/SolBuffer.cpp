#include "pch.hpp"
#include "SolBuffer.hpp"

namespace SolEngine
{
    SolBuffer::SolBuffer(SolDevice &rDevice, 
                         const VkDeviceSize &instanceSize,
                         const uint32_t instanceCount, 
                         const VkBufferUsageFlags usageFlags, 
                         const VkMemoryPropertyFlags memPropertyFlags, 
                         const VkDeviceSize minOffsetAlignment)
        : _rDevice(rDevice),
          _instanceSize(instanceSize),
          _instanceCount(instanceCount),
          _usageFlags(usageFlags),
          _memoryPropertyFlags(memPropertyFlags)
    {
        _alignmentSize = GetMinimumAlignmentSize(instanceSize, minOffsetAlignment);
        _bufferSize = _alignmentSize * instanceCount;
        
        _rDevice.CreateBuffer(_bufferSize, 
                              usageFlags, 
                              memPropertyFlags, 
                              _buffer, 
                              _bufferMemory);
    }

    SolBuffer::~SolBuffer()
    {
        Dispose();
    }

    VkResult SolBuffer::Map(const VkDeviceSize size, const VkDeviceSize offset)
    {
        DBG_ASSERT_MSG(_buffer && _bufferMemory,
                       "Map invoked on before before creation!");

        return vkMapMemory(_rDevice.GetDevice(), _bufferMemory, offset, size, 0, &_pMappedData);
    }

    void SolBuffer::Unmap()
    {
        if (_pMappedData == nullptr)
        {
            return;
        }

        vkUnmapMemory(_rDevice.GetDevice(), _bufferMemory);
        _pMappedData = nullptr;
    }

    void SolBuffer::WriteToBuffer(void *pData, 
                                  const VkDeviceSize size, 
                                  const VkDeviceSize offset)
    {
        if (size == VK_WHOLE_SIZE)
        {
            memcpy(_pMappedData, pData, _bufferSize);

            return;
        }

        char *pMemOffset = (char *)_pMappedData;

        pMemOffset += offset;

        memcpy(pMemOffset, pData, size);
    }

    VkResult SolBuffer::FlushBuffer(const VkDeviceSize size, const VkDeviceSize offset)
    {
        const uint32_t memoryRangeCount{ 1U };
        const VkMappedMemoryRange mappedMemRange
        {
            .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
            .memory = _bufferMemory,
            .offset = offset,
            .size   = size
        };

        return vkFlushMappedMemoryRanges(_rDevice.GetDevice(),
                                         memoryRangeCount, 
                                         &mappedMemRange);
    }

    VkDescriptorBufferInfo SolBuffer::DescriptorBufferInfo(const VkDeviceSize size, const VkDeviceSize offset)
    {
        return VkDescriptorBufferInfo
        {
            .buffer = _buffer,
            .offset = offset,
            .range  = size
        };
    }

    VkResult SolBuffer::InvalidateBuffer(const VkDeviceSize size, const VkDeviceSize offset)
    {
        const uint32_t memoryRangeCount{ 1U };
        const VkMappedMemoryRange mappedMemRange
        {
            .sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
            .memory = _bufferMemory,
            .offset = offset,
            .size   = size
        };

        return vkInvalidateMappedMemoryRanges(_rDevice.GetDevice(),
                                              memoryRangeCount, 
                                              &mappedMemRange);
    }

    void SolBuffer::WriteToIndex(void *pData, const size_t index)
    {
    }

    VkResult SolBuffer::FlushIndex(const size_t index)
    {
        return FlushBuffer(_instanceSize, index * _instanceSize);
    }

    VkDescriptorBufferInfo SolBuffer::DescriptorIndexInfo(const size_t index)
    {
        return DescriptorBufferInfo(_instanceSize, index * _instanceSize);
    }

    VkResult SolBuffer::InvalidateIndex(const size_t index)
    {
        return InvalidateBuffer(_instanceSize, index * _instanceSize);
    }

    VkDeviceSize SolBuffer::GetMinimumAlignmentSize(const VkDeviceSize instanceSize, 
                                                   const VkDeviceSize minOffsetAlignment)
    {
        if (minOffsetAlignment > 0)
        {
            return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        }

        return instanceSize;
    }

    void SolBuffer::Dispose()
    {
        Unmap();

        _rDevice.DisposeBuffer(_buffer, _bufferMemory);
    }
}