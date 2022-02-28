#pragma once
#include "Constants.hpp"
#include "MemoryHelpers.hpp"
#include "DiagnosticData.hpp"
#include "LiveNeighbours.hpp"

using namespace Utility;
using namespace SolEngine::Data;
using namespace SolEngine::Enumeration;
using namespace SolEngine::GUI::Data;

namespace SolEngine::DOD
{
    struct Nodes
    {
        ~Nodes()
        {
            DBG_ASSERT_MSG(_wasFreed, 
                           "Grid Nodes were not freed correctly - will cause Memory Leak!");
        }

        size_t AllocateDataArrays()
        {
            size_t memoryAllocatedBytes(0);

            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pXVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pYVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pZVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pIsoValues, MAX_CUBES_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguousArray(pLiveNeighbours, MAX_CUBES_COUNT * CUBE_VERTEX_COUNT);

            return memoryAllocatedBytes;
        }

        void Free()
        {
            FreeAlignedMallocArray(pXVertices);         // X-Positions
            FreeAlignedMallocArray(pYVertices);         // Y-Positions
            FreeAlignedMallocArray(pZVertices);         // Z-Positions
            FreeAlignedMallocArray(pIsoValues);         // Iso Values
            FreeAlignedMallocArray(pLiveNeighbours);    // Live Neighbours

            _wasFreed = true;
        }

        float*          pXVertices     { nullptr }; // All cubes vertices along x-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float*          pYVertices     { nullptr }; // All cubes vertices along y-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float*          pZVertices     { nullptr }; // All cubes vertices along z-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float*          pIsoValues     { nullptr }; // Stores all isoValues for every cubes vertices [iso_index * CUBE_VERTEX_COUNT + vertex_index]
        LiveNeighbours* pLiveNeighbours{ nullptr }; // Stores all live neighbours relative to the node

    private:
        bool _wasFreed{ false };    // Memory leak flag
    };
}
