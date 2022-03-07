#pragma once
#include "Constants.hpp"
#include "MemoryHelpers.hpp"
#include "DiagnosticData.hpp"

using namespace Utility;
using namespace SolEngine::Data;
using namespace SolEngine::Enumeration;
using namespace SolEngine::GUI::Data;

namespace SolEngine::DOD
{
    typedef unsigned char NeighbourCount_t;

    struct Cells
    {
        ~Cells()
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
            memoryAllocatedBytes += AlignedMallocContiguousArray(pCellStates, MAX_CUBES_COUNT * CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguousArray(pLiveNeighbourCounts, MAX_CUBES_COUNT * CUBE_VERTEX_COUNT);

            return memoryAllocatedBytes;
        }

        void Free()
        {
            FreeAlignedMallocArray(pXVertices);              // X-Positions
            FreeAlignedMallocArray(pYVertices);              // Y-Positions
            FreeAlignedMallocArray(pZVertices);              // Z-Positions
            FreeAlignedMallocArray(pCellStates);             // Cell States
            FreeAlignedMallocArray(pLiveNeighbourCounts);    // Live Neighbours

            _wasFreed = true;
        }

        float*             pXVertices          { nullptr }; // All cubes vertices along x-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float*             pYVertices          { nullptr }; // All cubes vertices along y-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float*             pZVertices          { nullptr }; // All cubes vertices along z-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        bool*              pCellStates         { nullptr }; // Stores all cell states (false = dead, true = alive)
        NeighbourCount_t*  pLiveNeighbourCounts{ nullptr }; // Stores all live neighbours relative to the node

    private:
        bool _wasFreed{ false };    // Memory leak flag
    };
}
