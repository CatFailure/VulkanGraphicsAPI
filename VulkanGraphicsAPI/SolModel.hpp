#pragma once
#include "SolBuffer.hpp"

using namespace SolEngine::Data;

namespace SolEngine
{
    // Takes vertex data created by or read in a file on the CPU
    // Then allocate and copy the data over to the GPU to be rendered
    class SolModel : public IDisposable
    {
    public:
        SolModel(SolDevice &rSolDevice, const Vertex *pVertices, const Index_t *pIndices);
        ~SolModel();

        void Bind(const VkCommandBuffer commandBuffer);
        void Draw(const VkCommandBuffer commandBuffer);

        // Inherited via IDisposable
        virtual void Dispose() override;

    private:
        void CreateVertexBuffers(const Vertex *pVertices);

        void CreateIndexBuffer(const Index_t *pIndices);

        SolDevice &_rSolDevice;

        std::unique_ptr<SolBuffer> _pVertexBuffer;
        std::unique_ptr<SolBuffer> _pIndexBuffer;

        uint32_t _instanceCount{ 1U };
    };
}
