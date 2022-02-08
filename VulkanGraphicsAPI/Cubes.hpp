#pragma once
#include "Constants.hpp"
#include "MemoryHelpers.hpp"
#include "IDisposable.hpp"

using namespace Utility;
using namespace SolEngine::Data;
using namespace SolEngine::Interface;

namespace SolEngine::DOD
{
    struct Cubes : private IDisposable
    {
        Cubes() = delete;

        Cubes(const glm::vec3 &minBounds, 
              const glm::vec3 &maxBounds)
        {
            AllocateDataArrays();
        }

        Cubes(const glm::vec3 &dimensions)
            : Cubes(glm::vec3(0, 0, 0), 
                    glm::vec3(dimensions.x, 
                              -dimensions.y, 
                              dimensions.z))
        {}

        Cubes(const float scalarDimensions)
            : Cubes({ scalarDimensions, 
                      scalarDimensions, 
                      scalarDimensions })
        {}

        ~Cubes()
        {
            Dispose();
        }

        void AllocateDataArrays()
        {
            AlignedMallocContiguous2DArray(pAllXVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            AlignedMallocContiguous2DArray(pAllYVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            AlignedMallocContiguous2DArray(pAllZVertices, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            AlignedMallocContiguous2DArray(pAllIsoValues, MAX_CUBES_COUNT, CUBE_VERTEX_COUNT);
        }

        static constexpr float STEP{ 1.f };  // Adjusts the resolution of the nodes

        float *pAllXVertices; // All cubes vertices along x-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float *pAllYVertices; // All cubes vertices along y-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float *pAllZVertices; // All cubes vertices along z-axis [position_index * CUBE_VERTEX_COUNT + vertex_index]
        float *pAllIsoValues;  // Stores all isoValues for every cubes vertices

    private:
        // Inherited via IDisposable
        virtual void Dispose() override
        {
            FreeAlignedMallocArray(pAllXVertices);    // X-Positions
            FreeAlignedMallocArray(pAllYVertices);    // Y-Positions
            FreeAlignedMallocArray(pAllZVertices);    // Z-Positions
            FreeAlignedMallocArray(pAllIsoValues);     // Iso Values
        }
    };
}
