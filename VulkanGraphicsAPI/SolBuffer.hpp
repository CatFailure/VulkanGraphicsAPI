#pragma once
#include "SolDevice.hpp"

namespace SolEngine
{
    class SolBuffer : private IDisposable
    {
    public:
        SolBuffer(SolDevice &rDevice, const VkDeviceSize &instanceSize, const uint32_t instanceCount, const VkBufferUsageFlags usageFlags, const VkMemoryPropertyFlags memPropertyFlags, const VkDeviceSize minOffsetAlignment = 1);
        ~SolBuffer();

        VkResult Map(const VkDeviceSize size = VK_WHOLE_SIZE, const VkDeviceSize offset = 0);
        void Unmap();

        // Buffer
        void				   WriteToBuffer(void *pData, const VkDeviceSize size = VK_WHOLE_SIZE, const VkDeviceSize offset = 0);
        VkResult			   FlushBuffer(const VkDeviceSize size = VK_WHOLE_SIZE, const VkDeviceSize offset = 0);
        VkDescriptorBufferInfo DescriptorBufferInfo(const VkDeviceSize size = VK_WHOLE_SIZE, const VkDeviceSize offset = 0);
        VkResult			   InvalidateBuffer(const VkDeviceSize size = VK_WHOLE_SIZE, const VkDeviceSize offset = 0);

        // Index
        void                   WriteToIndex(void *pData, const size_t index);
        VkResult               FlushIndex(const size_t index);
        VkDescriptorBufferInfo DescriptorIndexInfo(const size_t index);
        VkResult               InvalidateIndex(const size_t index);

        VkBuffer               GetBuffer()              const { return _buffer; }
        void *                 GetMappedMemory()        const { return _pMappedData; }
        VkDeviceSize           GetBufferSize()          const { return _bufferSize; }
        size_t                 GetInstanceCount()       const { return _instanceCount; }
        VkDeviceSize           GetInstanceSize()        const { return _instanceSize; }
        VkDeviceSize           GetAlignmentSize()       const { return _alignmentSize; }
        VkBufferUsageFlags     GetUsageFlags()          const { return _usageFlags; }
        VkMemoryPropertyFlags  GetMemoryPropertyFlags() const { return _memoryPropertyFlags; }

    private:
        /// <summary>
        /// Returns the minimum instance size required to be compatible with minOffsetAlignment
        /// </summary>
        /// <param name="instanceSize"></param>
        /// <param name="minOffsetAlignment"></param>
        /// <returns></returns>
        static VkDeviceSize GetMinimumAlignmentSize(const VkDeviceSize instanceSize, const VkDeviceSize minOffsetAlignment);

        // Inherited via IDisposable
        virtual void Dispose() override;

        SolDevice &    _rDevice;
        void *         _pMappedData  { nullptr };
        VkBuffer       _buffer       { VK_NULL_HANDLE };
        VkDeviceMemory _bufferMemory { VK_NULL_HANDLE };

        VkDeviceSize          _bufferSize;
        uint32_t              _instanceCount{ 0 };
        VkDeviceSize          _instanceSize;
        VkDeviceSize          _alignmentSize;
        VkBufferUsageFlags    _usageFlags;
        VkMemoryPropertyFlags _memoryPropertyFlags;
    };
}
