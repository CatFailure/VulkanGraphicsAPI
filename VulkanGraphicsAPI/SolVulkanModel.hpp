#pragma once
#include "SolVulkanDevice.hpp"

using namespace SolEngine::Data;

namespace SolEngine
{
    // Takes vertex data created by or read in a file on the CPU
    // Then allocate and copy the data over to the GPU to be rendered
    class SolVulkanModel : public IDisposable
    {
    public:
        SolVulkanModel(SolVulkanDevice &rSolVulkanDevice, const std::vector<Vertex> &vertices);
        ~SolVulkanModel();

        void Bind(const VkCommandBuffer &commandBuffer);
        void Draw(const VkCommandBuffer &commandBuffer);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void CreateVertexBuffers(const std::vector<Vertex> &vertices);

        SolVulkanDevice &_rSolVulkanDevice;

        VkBuffer	   _vkVertexBuffer;
        VkDeviceMemory _vkVertexBufferMemory;
        uint32_t	   _vertexCount;
    };
}
