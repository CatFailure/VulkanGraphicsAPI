#pragma once
#include "SolDevice.hpp"

using namespace SolEngine::Data;

namespace SolEngine
{
    // Takes vertex data created by or read in a file on the CPU
    // Then allocate and copy the data over to the GPU to be rendered
    class SolModel : public IDisposable
    {
    public:
        SolModel(SolDevice &rSolDevice, const std::vector<Vertex> &vertices);
        ~SolModel();

        void Bind(const VkCommandBuffer commandBuffer);
        void Draw(const VkCommandBuffer commandBuffer);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void CreateVertexBuffers(const std::vector<Vertex> &vertices);

        SolDevice &_rSolDevice;

        VkBuffer	   _vkVertexBuffer;
        VkDeviceMemory _vkVertexBufferMemory;
        uint32_t	   _vertexCount;
    };
}