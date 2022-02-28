#pragma once
#include "Constants.hpp"
#include "MemoryHelpers.hpp"
#include "DiagnosticData.hpp"
#include "IDisposable.hpp"
#include "LiveNeighbours.hpp"

using namespace Utility;
using namespace SolEngine::Data;
using namespace SolEngine::Enumeration;
using namespace SolEngine::GUI::Data;
using namespace SolEngine::Interface;

namespace SolEngine::DOD
{
    struct Nodes : private IDisposable
    {
        Nodes(DiagnosticData &rDiagnosticData)
            : _rDiagnosticData(rDiagnosticData) 
        {
            AllocateDataArrays(); 
        }

        ~Nodes() { Dispose(); }

        void AllocateDataArrays()
        {
            size_t memoryAllocatedBytes(0);

            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pXVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pYVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pZVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pIsoValues, MAX_CUBES_COUNT, CUBE_VERTEX_COUNT);

            _rDiagnosticData.memoryAllocatedBytes = memoryAllocatedBytes;
        }

        float*          pXVertices     { nullptr }; // All cubes vertices along x-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float*          pYVertices     { nullptr }; // All cubes vertices along y-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float*          pZVertices     { nullptr }; // All cubes vertices along z-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float*          pIsoValues     { nullptr }; // Stores all isoValues for every cubes vertices [iso_index * CUBE_VERTEX_COUNT + vertex_index]
        LiveNeighbours* pLiveNeighbours{ nullptr }; // Stores all live neighbours relative to the node

    private:
        // Inherited via IDisposable
        virtual void Dispose() override
        {
            FreeAlignedMallocArray(pXVertices);              // X-Positions
            FreeAlignedMallocArray(pYVertices);              // Y-Positions
            FreeAlignedMallocArray(pZVertices);              // Z-Positions
            FreeAlignedMallocArray(pIsoValues);              // Iso Values
            FreeAlignedMallocArray(pLiveNeighbourCounts);    // Neighbour Counts
        }

        DiagnosticData &_rDiagnosticData;
    };
}
