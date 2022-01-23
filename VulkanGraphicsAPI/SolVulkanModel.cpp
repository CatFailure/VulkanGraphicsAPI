#include "pch.hpp"
#include "SolVulkanModel.hpp"

namespace SolEngine
{
    SolVulkanModel::SolVulkanModel(SolVulkanDevice &rSolDevice, 
                                   const std::vector<Vertex> &vertices)
        : _rSolDevice(rSolDevice)
    {
        CreateVertexBuffers(vertices);
    }

    SolVulkanModel::~SolVulkanModel()
    {
        Dispose();
    }

    void SolVulkanModel::Bind(const VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[]{ _vkVertexBuffer };
        VkDeviceSize offsets[]{ 0 };

        vkCmdBindVertexBuffers(commandBuffer,
                               0, 
                               1, 
                               buffers,
                               offsets);
    }

    void SolVulkanModel::Draw(const VkCommandBuffer commandBuffer)
    {
        vkCmdDraw(commandBuffer, 
                  _vertexCount,
                  1,
                  0,
                  0);
    }

    void SolVulkanModel::Dispose()
    {
        const VkDevice &device = _rSolDevice.Device();

        vkDestroyBuffer(device,
                        _vkVertexBuffer, 
                        NULL);

        vkFreeMemory(device, 
                     _vkVertexBufferMemory,
                     NULL);
    }

    void SolVulkanModel::CreateVertexBuffers(const std::vector<Vertex> &vertices)
    {
        _vertexCount = static_cast<uint32_t>(vertices.size());

        DBG_ASSERT_MSG(!(_vertexCount < 3), "Vertex count must be at least 3.");

        const VkDeviceSize bufferSize = sizeof(vertices.at(0)) * _vertexCount;

        _rSolDevice.CreateBuffer(bufferSize, 
                                       VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,                                           // Create a buffer that will hold Vertex Input Data
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,  // Host = CPU, Device = GPU
                                       _vkVertexBuffer,
                                       _vkVertexBufferMemory);

        // Create a region of host memory mapped to device memory
        // and point pBufferData to beginning of mapped memory range
        void *pBufferData;

        vkMapMemory(_rSolDevice.Device(), 
                    _vkVertexBufferMemory,
                    0,
                    bufferSize, 
                    0,
                    &pBufferData);

        // Copy vertices data into the host mapped memory region
        memcpy(pBufferData, 
               vertices.data(), 
               static_cast<uint32_t>(bufferSize));

        vkUnmapMemory(_rSolDevice.Device(), 
                      _vkVertexBufferMemory);
    }
}