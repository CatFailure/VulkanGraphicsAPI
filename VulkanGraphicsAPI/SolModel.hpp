#pragma once
#include "SolBuffer.hpp"

using namespace SolEngine::Data;

namespace SolEngine
{
    typedef uint32_t Index_t;

    // Takes vertex data created by or read in a file on the CPU
    // Then allocate and copy the data over to the GPU to be rendered
    class SolModel : public IDisposable
    {
    public:
        SolModel(SolDevice &rSolDevice, const std::vector<Vertex> &vertices, const std::vector<Index_t> &indices);
        ~SolModel();

        void Bind(const VkCommandBuffer commandBuffer);
        void Draw(const VkCommandBuffer commandBuffer);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void CreateVertexBuffers(const std::vector<Vertex> &vertices);
        void CreateIndexBuffer(const std::vector<Index_t> &indices);

        SolDevice &_rSolDevice;

        /*VkBuffer	   _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;*/
        std::unique_ptr<SolBuffer> _pVertexBuffer;
        uint32_t	   _vertexCount;

        bool           _hasIndexBuffer{ false };    // Allows a model to only contain vertex info with no indices
        std::unique_ptr<SolBuffer> _pIndexBuffer;
        /*VkBuffer	   _indexBuffer;
        VkDeviceMemory _indexBufferMemory;*/

        uint32_t	   _indexCount;

        uint32_t _instanceCount{ 1U };
    };
}
