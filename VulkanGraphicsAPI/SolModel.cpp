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
        VkBuffer     vertexBuffers[]{ _pVertexBuffer->GetBuffer() };
        VkDeviceSize offsets[]      { 0 };

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        if (!_hasIndexBuffer)
        {
            return;
        }

        // TODO:
        // Right now index buffers are a 32-bit number, 
        // but since this will only be rendering cubes - 
        // we may be able to reduce this to a smaller data type.
        vkCmdBindIndexBuffer(commandBuffer, 
                             _pIndexBuffer->GetBuffer(),
                             0, 
                             VK_INDEX_TYPE_UINT32);
    }

    void SolModel::Draw(const VkCommandBuffer commandBuffer)
    {
        if (_hasIndexBuffer)
        {
            vkCmdDrawIndexed(commandBuffer, 
                             _indexCount, 
                             _instanceCount, 
                             0,
                             0,
                             0);

            return;
        }

        vkCmdDraw(commandBuffer, 
                  _vertexCount, 
                  _instanceCount, 
                  0,
                  0);
    }

    void SolModel::Dispose()
    {}

    void SolModel::CreateVertexBuffers(const std::vector<Vertex> &vertices)
    {
        _vertexCount = static_cast<uint32_t>(vertices.size());

        DBG_ASSERT_MSG(!(_vertexCount < 3), 
                       "Vertex count must be at least 3.");

        const size_t vertexSize = sizeof(vertices.at(0));
        const VkDeviceSize bufferSize = vertexSize * _vertexCount;

        // We can't directly map from Host memory to Device Local Memory
        // So copy the Host data to a temp Staging Buffer on Device,
        // then copy this buffer to Device Local memory.
        // 
        // Staging buffers act as a "middle man" in the Device 
        // when copying data from Host to Device Local Memory
        //   Host (CPU)              |              Device (GPU)
        // 
        //                    Copy to temp 
        // ===============   Staging Buffer   =========================
        // | void *pData | -----------------> | Staging Buffer Memory |
        // ===============                    =========================
        //          \                                     | CopyBuffer()
        //           ----XX----                           V
        //              ¬      \            ==============================
        //   Can't map Host      ---------> | Vertex/Index Buffer Memory |
        //   Memory directly                ==============================
        //   to Device Local Memory!         (Optimal Device Local Memory)

        // Create Staging Buffer for Vertex Data
        SolBuffer stagingBuffer(_rSolDevice, 
                                vertexSize, 
                                _vertexCount, 
                                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer((void *)vertices.data());

        // Create buffer in Device Local Memory
        _pVertexBuffer = std::make_unique<SolBuffer>(_rSolDevice,
                                                     vertexSize, 
                                                     _vertexCount,
                                                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,  // Create a buffer to hold Vertex Input data
                                                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);                                  // Use Device Local Memory

        // Copy over data in the staging buffer to device local memory...
        _rSolDevice.CopyBuffer(stagingBuffer.GetBuffer(), 
                               _pVertexBuffer->GetBuffer(), 
                               bufferSize);
    }

    void SolModel::CreateIndexBuffer(const std::vector<Index_t> &indices)
    {
        _indexCount = static_cast<uint32_t>(indices.size());

        // Does the model only contain Vertex data?
        if (_indexCount == 0)
        {
            _hasIndexBuffer = false;

            return;
        }

        _hasIndexBuffer = true;

        const size_t indexSize = sizeof(indices.at(0));
        const VkDeviceSize bufferSize = indexSize * _indexCount;

        // We can't directly map from Host memory to Device Local Memory
        // So copy the Host data to a temp Staging Buffer on Device,
        // then copy this buffer to Device Local memory.
        // 
        // Staging buffers act as a "middle man" in the Device 
        // when copying data from Host to Device Local Memory
        //   Host (CPU)              |              Device (GPU)
        // 
        //                    Copy to temp 
        // ===============   Staging Buffer   =========================
        // | void *pData | -----------------> | Staging Buffer Memory |
        // ===============                    =========================
        //          \                                     | CopyBuffer()
        //           ----XX----                           V
        //              ¬      \            ==============================
        //   Can't map Host      ---------> | Vertex/Index Buffer Memory |
        //   Memory directly                ==============================
        //   to Device Local Memory!         (Optimal Device Local Memory)

        // Create Staging Buffer for Index Data
        SolBuffer stagingBuffer(_rSolDevice,
                                indexSize, 
                                _indexCount, 
                                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer((void *)indices.data());

        // Create buffer in Device Local Memory
        _pIndexBuffer = std::make_unique<SolBuffer>(_rSolDevice,
                                                    indexSize, 
                                                    _indexCount,
                                                    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,  // Create a buffer to hold Vertex Input data
                                                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);                                 // Use Device Local Memory

        // Copy over data in the staging buffer to device local memory...
        _rSolDevice.CopyBuffer(stagingBuffer.GetBuffer(),
                               _pIndexBuffer->GetBuffer(), 
                               bufferSize);
    }
}