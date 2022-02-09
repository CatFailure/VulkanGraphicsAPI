#pragma once
#include "SolBuffer.hpp"
#include "Constants.hpp"

using namespace SolEngine::Data;

namespace SolEngine
{
    // Takes vertex data created by or read in a file on the CPU
    // Then allocate and copy the data over to the GPU to be rendered
    class SolModel : public IDisposable
    {
    public:
        SolModel(SolDevice &rSolDevice, const Vertex *pVertices, const uint32_t vertexCount, const UIndex_t *pIndices = nullptr, const uint32_t indexCount = 0);
        ~SolModel();

        void Bind(const VkCommandBuffer commandBuffer);
        void Draw(const VkCommandBuffer commandBuffer);

        // Inherited via IDisposable
        virtual void Dispose() override;
        
    private:
        void CreateVertexBuffers(const Vertex *pVertices, const uint32_t vertexCount);
        void CreateIndexBuffer(const UIndex_t *pIndices, const uint32_t indexCount = 0);

        SolDevice &_rSolDevice;

        bool _hasIndexBuffer{ false };
        std::unique_ptr<SolBuffer> _pVertexBuffer;
        std::unique_ptr<SolBuffer> _pIndexBuffer;

        uint32_t _vertexCount{ 0 };
        uint32_t _indexCount{ 0 };
        uint32_t _instanceCount{ 1U };
    };
}
