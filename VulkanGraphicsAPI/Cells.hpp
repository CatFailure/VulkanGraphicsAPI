#pragma once
#include "Constants.hpp"
#include "MemoryHelpers.hpp"
#include "DiagnosticData.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Enumeration;

namespace SolEngine::DOD
{
    struct Cells
    {
        ~Cells()
        {
            DBG_ASSERT_MSG(_wasFreed, 
                           "Grid Nodes were not freed correctly - will cause Memory Leak!");
        }

        size_t AllocateDataArrays()
        {
            // Malloc alignments
            const size_t intAlign           { 4U };
            const size_t boolAlign          { 1U };
            const size_t neighbourStateAlign{ 1U };

            size_t memoryAllocatedBytes(0);

            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pXVertices, MAX_CELLS_PER_AXIS_COUNT, CUBE_VERTEX_COUNT, intAlign);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pYVertices, MAX_CELLS_PER_AXIS_COUNT, CUBE_VERTEX_COUNT, intAlign);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pZVertices, MAX_CELLS_PER_AXIS_COUNT, CUBE_VERTEX_COUNT, intAlign);

            memoryAllocatedBytes += AlignedMallocContiguousArray(pCellStates,          MAX_CUBE_VERTEX_COUNT, boolAlign);
            memoryAllocatedBytes += AlignedMallocContiguousArray(pLiveNeighbourCounts, MAX_CUBE_VERTEX_COUNT, neighbourStateAlign);

            return memoryAllocatedBytes;
        }

        void Free()
        {
            FreeAlignedMallocArray(pXVertices);             // X-Positions
            FreeAlignedMallocArray(pYVertices);             // Y-Positions
            FreeAlignedMallocArray(pZVertices);             // Z-Positions
            FreeAlignedMallocArray(pCellStates);            // Cell States
            FreeAlignedMallocArray(pLiveNeighbourCounts);   // Live Neighbours

            _wasFreed = true;
        }

        int*              pXVertices          { nullptr }; // All cubes vertices along x-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        int*              pYVertices          { nullptr }; // All cubes vertices along y-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        int*              pZVertices          { nullptr }; // All cubes vertices along z-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        bool*             pCellStates         { nullptr }; // Stores all cell states (false = dead, true = alive)
        NeighbourCount_t* pLiveNeighbourCounts{ nullptr }; // Stores all live neighbours relative to the node

    private:
        bool _wasFreed{ false };    // Memory leak flag
    };
}
