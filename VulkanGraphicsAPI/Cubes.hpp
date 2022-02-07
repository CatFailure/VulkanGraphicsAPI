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
            AlignedMallocContiguous2DArray(ppXPositions, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            AlignedMallocContiguous2DArray(ppYPositions, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            AlignedMallocContiguous2DArray(ppZPositions, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            AlignedMallocContiguous2DArray(ppIsoValues, MAX_CUBES_COUNT, CUBE_VERTEX_COUNT);
        }

        static constexpr float STEP{ 1.f };  // Adjusts the resolution of the nodes

        // https://stackoverflow.com/a/29375830
        float **ppXPositions;
        float **ppYPositions;
        float **ppZPositions;
        float **ppIsoValues;    // Index first subscript using _3DTo1DIndex

    private:
        // Inherited via IDisposable
        virtual void Dispose() override
        {
            // X-Positions
            FreeAlignedMallocArray(ppXPositions[0]);
            FreeAlignedMallocArray(ppXPositions);

            // Y-Positions
            FreeAlignedMallocArray(ppYPositions[0]);
            FreeAlignedMallocArray(ppYPositions);

            // Z-Positions
            FreeAlignedMallocArray(ppZPositions[0]);
            FreeAlignedMallocArray(ppZPositions);

            // Iso Values
            FreeAlignedMallocArray(ppIsoValues[0]);
            FreeAlignedMallocArray(ppIsoValues);
        }
    };
}
