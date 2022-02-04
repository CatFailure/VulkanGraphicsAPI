#pragma once
#include "Constants.hpp"

using namespace SolEngine::Data;

namespace SolEngine::DOD
{
    struct Cubes
    {
        Cubes() = default;
        Cubes(const glm::vec3 &dimensions)
        {
            size_t index(0);
            for (float x = 0; x < dimensions.x; x += step)
            {
                const float adjX = x + 1.f;
                float *pXVertices = xPositions[index];

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

            index = 0;
            for (float y = 0; y < dimensions.y; y += step)
            {
                const float adjY = -y - 1.f;
                float *pYVertices = yPositions[index];

                pYVertices[0] = -y;
                pYVertices[1] = -y;
                pYVertices[2] = -y;
                pYVertices[3] = -y;
                pYVertices[4] = adjY;
                pYVertices[5] = adjY;
                pYVertices[6] = adjY;
                pYVertices[7] = adjY;

                ++index;
            }

            index = 0;
            for (float z = 0; z < dimensions.z; z += step)
            {
                const float adjZ = z + 1.f;
                float *pZVertices = zPositions[index];

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
        }

        Cubes(const float scalarDimensions)
            : Cubes({scalarDimensions, scalarDimensions, scalarDimensions})
        {}

        float step{ 1.f };  // Adjusts the resolution of the nodes

        alignas(16) float xPositions[MAX_CUBES_PER_AXIS_COUNT][CUBE_VERTEX_COUNT]{ 0 };
        alignas(16) float yPositions[MAX_CUBES_PER_AXIS_COUNT][CUBE_VERTEX_COUNT]{ 0 };
        alignas(16) float zPositions[MAX_CUBES_PER_AXIS_COUNT][CUBE_VERTEX_COUNT]{ 0 };

        alignas(16) float isoValues[MAX_CUBES_COUNT][CUBE_VERTEX_COUNT]{ 0 };   // Index using _3DTo1DIndex
    };
}
