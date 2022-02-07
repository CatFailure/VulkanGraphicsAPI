#pragma once
#include "Constants.hpp"
#include "MemoryHelpers.hpp"

using namespace Utility;
using namespace SolEngine::Data;

namespace SolEngine::DOD
{
    struct Cubes
    {
        Cubes() = delete;

        Cubes(const glm::vec3 &minBounds, 
              const glm::vec3 &maxBounds)
        {
            AllocateDataArrays();

            /*GenerateVertices<Axis::X>(ppXPositions, minBounds.x, maxBounds.x, STEP);
            GenerateVertices<Axis::Y>(ppYPositions, minBounds.y, maxBounds.y, STEP);
            GenerateVertices<Axis::Z>(ppZPositions, minBounds.z, maxBounds.z, STEP);*/
        }

        Cubes(const glm::vec3 &dimensions)
            : Cubes(glm::vec3(0, 0, 0), 
                    glm::vec3(dimensions.x, 
                              -dimensions.y, 
                              dimensions.z))
        {}

        Cubes(const float scalarDimensions)
            : Cubes({scalarDimensions, scalarDimensions, scalarDimensions})
        {}

        ~Cubes()
        {
            Dispose();
        }

        void AllocateDataArrays()
        {
            AllocateContiguous2DArray(ppXPositions, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            AllocateContiguous2DArray(ppYPositions, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            AllocateContiguous2DArray(ppZPositions, MAX_CUBES_PER_AXIS_COUNT, CUBE_VERTEX_COUNT);
            AllocateContiguous2DArray(ppIsoValues, MAX_CUBES_COUNT, CUBE_VERTEX_COUNT);
        }

        void Dispose()
        {
            // X-Positions
            SafeDisposeArray(ppXPositions[0]);
            SafeDisposeArray(ppXPositions);

            // Y-Positions
            SafeDisposeArray(ppYPositions[0]);
            SafeDisposeArray(ppYPositions);

            // Z-Positions
            SafeDisposeArray(ppZPositions[0]);
            SafeDisposeArray(ppZPositions);

            // Iso Values
            SafeDisposeArray(ppIsoValues[0]);
            SafeDisposeArray(ppIsoValues);
        }

        static constexpr float STEP{ 1.f };  // Adjusts the resolution of the nodes

        // https://stackoverflow.com/a/29375830
        float **ppXPositions;
        float **ppYPositions;
        float **ppZPositions;
        float **ppIsoValues;    // Index first subscript using _3DTo1DIndex
    };
}
