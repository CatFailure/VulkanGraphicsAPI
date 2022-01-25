#include "pch.hpp"
#include "SolModel.hpp"

namespace SolEngine
{
    SolModel::SolModel(SolDevice &rSolDevice, 
                       const std::vector<Vertex> &vertices, 
                       const std::vector<Index_t> &indices)
        : _rSolDevice(rSolDevice)
    {
        CreateVertexBuffers(vertices);
        CreateIndexBuffer(indices);
    }

    SolModel::~SolModel()
    {
        Dispose();
    }

    void SolModel::Bind(const VkCommandBuffer commandBuffer)
    {
        VkBuffer     buffers[]{ _vertexBuffer };
        VkDeviceSize offsets[]{ 0 };

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (!_hasIndexBuffer)
        {
            return;
        }

        // TODO:
        // Right now index buffers are a 32-bit number, 
        // but since this will only be rendering cubes - 
        // we may be able to reduce this to a smaller data type.
        vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void SolModel::Draw(const VkCommandBuffer commandBuffer)
    {
        if (_hasIndexBuffer)
        {
            vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);

            return;
        }

        vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
    }

    void SolModel::Dispose()
    {
        // Vertex-buffer/memory
        _rSolDevice.DisposeBuffer(_vertexBuffer, _vertexBufferMemory);

        if (!_hasIndexBuffer)
        {
            return;
        }

        // Index-buffer/memory
        _rSolDevice.DisposeBuffer(_indexBuffer, _indexBufferMemory);
    }

    void SolModel::CreateVertexBuffers(const std::vector<Vertex> &vertices)
    {
        _vertexCount = static_cast<uint32_t>(vertices.size());

        DBG_ASSERT_MSG(!(_vertexCount < 3), "Vertex count must be at least 3.");

        const VkDeviceSize bufferSize = sizeof(vertices.at(0)) * _vertexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        _rSolDevice.CreateBuffer(bufferSize, 
                                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,  // Host = CPU, Device = GPU
                                 stagingBuffer,
                                 stagingBufferMemory);

        // Create a region of host memory mapped to device memory
        // and point pBufferData to beginning of mapped memory range
        void *pBufferData;

        vkMapMemory(_rSolDevice.GetDevice(), 
                    stagingBufferMemory,
                    0,
                    bufferSize, 
                    0,
                    &pBufferData);

        // Copy vertices data into the host mapped memory region
        memcpy(pBufferData, 
               vertices.data(), 
               static_cast<uint32_t>(bufferSize));

        vkUnmapMemory(_rSolDevice.GetDevice(), 
                      stagingBufferMemory);

        _rSolDevice.CreateBuffer(bufferSize, 
                                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                 _vertexBuffer,
                                 _vertexBufferMemory);

        _rSolDevice.CopyBuffer(stagingBuffer, _vertexBuffer, bufferSize);
        _rSolDevice.DisposeBuffer(stagingBuffer, stagingBufferMemory);
    }

    void SolModel::CreateIndexBuffer(const std::vector<Index_t> &indices)
    {
        _indexCount = static_cast<uint32_t>(indices.size());

        if (_indexCount == 0)
        {
            _hasIndexBuffer = false;

            return;
        }

        _hasIndexBuffer = true;

        const VkDeviceSize bufferSize = sizeof(indices.at(0)) * _indexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        _rSolDevice.CreateBuffer(bufferSize, 
                                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,  // Host = CPU, Device = GPU
                                 stagingBuffer,
                                 stagingBufferMemory);

        // Create a region of host memory mapped to device memory
        // and point pBufferData to beginning of mapped memory range
        void *pBufferData;

        vkMapMemory(_rSolDevice.GetDevice(), 
                    stagingBufferMemory,
                    0,
                    bufferSize, 
                    0,
                    &pBufferData);

        // Copy vertices data into the host mapped memory region
        memcpy(pBufferData, 
               indices.data(), 
               static_cast<uint32_t>(bufferSize));

        vkUnmapMemory(_rSolDevice.GetDevice(), 
                      stagingBufferMemory);

        _rSolDevice.CreateBuffer(bufferSize, 
                                 VK_BUFFER_USAGE_INDEX_BUFFER_BIT,                                            // Create a buffer that will hold Index Input Data
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,  // Host = CPU, Device = GPU
                                 _indexBuffer,
                                 _indexBufferMemory);

        _rSolDevice.CopyBuffer(stagingBuffer, _indexBuffer, bufferSize);
        _rSolDevice.DisposeBuffer(stagingBuffer, stagingBufferMemory);
    }
}