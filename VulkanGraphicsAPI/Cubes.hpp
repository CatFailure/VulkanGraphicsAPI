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
            GeneratePositions(minBounds, maxBounds);
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

        void GeneratePositions(const glm::vec3 &minBounds, const glm::vec3 &maxBounds)
        {
            uint32_t index(0);
            for (float x = minBounds.x; x < maxBounds.x; x += STEP)
            {
                const float adjX  = x + STEP;
                float *pXVertices = ppXPositions[index];

                pXVertices[0] = x;
                pXVertices[1] = adjX;
                pXVertices[2] = adjX;
                pXVertices[3] = x;
                pXVertices[4] = x;
                pXVertices[5] = adjX;
                pXVertices[6] = adjX;
                pXVertices[7] = x;

                ++index;
            }

            printf_s("Generated %u X-Positions.\n", index);

            index = 0;
            for (float y = minBounds.y; y > maxBounds.y; y -= STEP)
            {
                const float adjY  = y - STEP;
                float *pYVertices = ppYPositions[index];

                pYVertices[0] = y;
                pYVertices[1] = y;
                pYVertices[2] = y;
                pYVertices[3] = y;
                pYVertices[4] = adjY;
                pYVertices[5] = adjY;
                pYVertices[6] = adjY;
                pYVertices[7] = adjY;

                ++index;
            }

            printf_s("Generated %u Y-Positions.\n", index);

            index = 0;
            for (float z = minBounds.z; z < maxBounds.z; z += STEP)
            {
                const float adjZ  = z + STEP;
                float *pZVertices = ppZPositions[index];

                pZVertices[0] = z;
                pZVertices[1] = z;
                pZVertices[2] = adjZ;
                pZVertices[3] = adjZ;
                pZVertices[4] = z;
                pZVertices[5] = z;
                pZVertices[6] = adjZ;
                pZVertices[7] = adjZ;

                ++index;
            }

            printf_s("Generated %u Z-Positions.\n", index);
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
