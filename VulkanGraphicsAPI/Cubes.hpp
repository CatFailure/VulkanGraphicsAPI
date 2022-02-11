#pragma once
#include "Constants.hpp"
#include "MemoryHelpers.hpp"
#include "DiagnosticData.hpp"
#include "IDisposable.hpp"

using namespace Utility;
using namespace SolEngine::Data;
using namespace SolEngine::GUI::Data;
using namespace SolEngine::Interface;

namespace SolEngine::DOD
{
    struct Cubes : private IDisposable
    {
        Cubes(DiagnosticData &rDiagnosticData)
            : _rDiagnosticData(rDiagnosticData) 
        {
            AllocateDataArrays(); 
        }

        ~Cubes() { Dispose(); }

        void AllocateDataArrays()
        {
            size_t memoryAllocatedBytes(0);

            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pAllXVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pAllYVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pAllZVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            memoryAllocatedBytes += AlignedMallocContiguous2DArray(pAllIsoValues, MAX_CUBES_COUNT, CUBE_VERTEX_COUNT);

            _rDiagnosticData.memoryAllocatedBytes = memoryAllocatedBytes;
        }

        static constexpr float STEP{ 1.f };  // Adjusts the resolution of the nodes

        float *pAllXVertices;   // All cubes vertices along x-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float *pAllYVertices;   // All cubes vertices along y-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float *pAllZVertices;   // All cubes vertices along z-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float *pAllIsoValues;   // Stores all isoValues for every cubes vertices [iso_index * CUBE_VERTEX_COUNT + vertex_index]

    private:
        // Inherited via IDisposable
        virtual void Dispose() override
        {
            FreeAlignedMallocArray(pAllXVertices);  // X-Positions
            FreeAlignedMallocArray(pAllYVertices);  // Y-Positions
            FreeAlignedMallocArray(pAllZVertices);  // Z-Positions
            FreeAlignedMallocArray(pAllIsoValues);  // Iso Values
        }

        DiagnosticData &_rDiagnosticData;
    };
}
