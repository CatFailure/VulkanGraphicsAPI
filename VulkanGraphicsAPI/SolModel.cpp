#include "pch.hpp"
#include "SolModel.hpp"

namespace SolEngine
{
    SolModel::SolModel(SolDevice &rSolDevice, 
                       const Vertex *pVertices, 
                       const size_t vertexCount, 
                       const Index_t *pIndices, 
                       const size_t indexCount)
        : _rSolDevice(rSolDevice)
    {
        CreateVertexBuffers(pVertices, vertexCount);
        CreateIndexBuffer(pIndices, vertexCount);
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

        // TODO:
        // Right now index buffers are a 32-bit number, 
        // but since this will only be rendering cubes - 
        // we may be able to reduce this to a smaller data type.
        vkCmdBindIndexBuffer(commandBuffer, 
                             _pIndexBuffer->GetBuffer(),
                             0, 
                             VK_INDEX_TYPE_UINT8_EXT);
    }

    void SolModel::Draw(const VkCommandBuffer commandBuffer)
    {
        /* TODO:
        * Right now, a model is drawn one mesh at a time with each mesh owning it's vertices and indices.
        * However, using a large Grid of vertices, we can manipulate the Vertex offset to draw multiple meshes 
        * from a massive shared array of vertices with the indices determined by the TriTable!
        */
        vkCmdDrawIndexed(commandBuffer, 
                         CUBE_INDEX_COUNT, 
                         _instanceCount, 
                         0,
                         0,     // Vertex offset - VERY IMPORTANT FOR MARCHING CUBES.
                         0);
    }

    void SolModel::Dispose()
    {}

    void SolModel::CreateVertexBuffers(const Vertex *pVertices, const size_t vertexCount)
    {
        const size_t vertexSize = sizeof(Vertex);
        const VkDeviceSize bufferSize = vertexSize * vertexCount;

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
                                vertexCount, 
                                VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer((void *)pVertices);

        // Create buffer in Device Local Memory
        _pVertexBuffer = std::make_unique<SolBuffer>(_rSolDevice,
                                                     vertexSize, 
                                                     vertexCount,
                                                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,  // Create a buffer to hold Vertex Input data
                                                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);                                  // Use Device Local Memory

        // Copy over data in the staging buffer to device local memory...
        _rSolDevice.CopyBuffer(stagingBuffer.GetBuffer(), 
                               _pVertexBuffer->GetBuffer(), 
                               bufferSize);
    }

    void SolModel::CreateIndexBuffer(const Index_t *pIndices, const size_t indexCount)
    {
        const size_t indexSize = sizeof(Index_t);
        const VkDeviceSize bufferSize = indexSize * indexCount;

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
                                indexCount, 
                                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        stagingBuffer.Map();
        stagingBuffer.WriteToBuffer((void *)pIndices);

        // Create buffer in Device Local Memory
        _pIndexBuffer = std::make_unique<SolBuffer>(_rSolDevice,
                                                    indexSize, 
                                                    indexCount,
                                                    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,  // Create a buffer to hold Vertex Input data
                                                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);                                 // Use Device Local Memory

        // Copy over data in the staging buffer to device local memory...
        _rSolDevice.CopyBuffer(stagingBuffer.GetBuffer(),
                               _pIndexBuffer->GetBuffer(), 
                               bufferSize);
    }
}