#pragma once
#include <stdlib.h>

#include "Constants.hpp"

using namespace SolEngine::Data;

namespace SolEngine::DOD
{
    struct Cubes
    {
        Cubes() = delete;

        Cubes(const glm::vec3 &dimensions)
        {
            InitializePositionsArrays();

            size_t index(0);
            for (float x = 0; x < dimensions.x; x += STEP)
            {
                const float adjX  = x + 1.f;
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

            index = 0;
            for (float y = 0; y < dimensions.y; y += STEP)
            {
                const float adjY  = -y - 1.f;
                float *pYVertices = ppYPositions[index];

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
            for (float z = 0; z < dimensions.z; z += STEP)
            {
                const float adjZ  = z + 1.f;
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
        }

        Cubes(const float scalarDimensions)
            : Cubes({scalarDimensions, scalarDimensions, scalarDimensions})
        {}

        ~Cubes()
        {
            Dispose();
        }

        void InitializePositionsArrays()
        {
            // https://stackoverflow.com/a/29375830
            ppXPositions    = new float *[MAX_CUBES_PER_AXIS_COUNT];
            ppXPositions[0] = new float[MAX_CUBES_PER_AXIS_COUNT * CUBE_VERTEX_COUNT];

            ppYPositions    = new float *[MAX_CUBES_PER_AXIS_COUNT];
            ppYPositions[0] = new float[MAX_CUBES_PER_AXIS_COUNT * CUBE_VERTEX_COUNT];

            ppZPositions    = new float *[MAX_CUBES_PER_AXIS_COUNT];
            ppZPositions[0] = new float[MAX_CUBES_PER_AXIS_COUNT * CUBE_VERTEX_COUNT];

            ppIsoValues     = new float *[MAX_CUBES_COUNT];
            ppIsoValues[0]  = new float[MAX_CUBES_COUNT * CUBE_VERTEX_COUNT];

            for (size_t i = 1; i < MAX_CUBES_PER_AXIS_COUNT; ++i)
            {
                ppXPositions[i] = ppXPositions[i - 1] + CUBE_VERTEX_COUNT;
                ppYPositions[i] = ppYPositions[i - 1] + CUBE_VERTEX_COUNT;
                ppZPositions[i] = ppZPositions[i - 1] + CUBE_VERTEX_COUNT;
            }

            for (size_t i = 1; i < MAX_CUBES_COUNT; ++i)
            {
                ppIsoValues[i] = ppIsoValues[i - 1] + CUBE_VERTEX_COUNT;
            }
        }

        void Dispose()
        {
            // X-Positions
            delete[] ppXPositions[0];
            ppXPositions[0] = nullptr;

            delete[] ppXPositions;
            ppXPositions = nullptr;

            // Y-Positions
            delete[] ppYPositions[0];
            ppYPositions[0] = nullptr;

            delete[] ppYPositions;
            ppYPositions = nullptr;

            // Z-Positions
            delete[] ppZPositions[0];
            ppZPositions[0] = nullptr;

            delete[] ppZPositions;
            ppZPositions = nullptr;

            // Iso Values
            delete[] ppIsoValues[0];
            ppIsoValues[0] = nullptr;

            delete[] ppIsoValues;
            ppIsoValues = nullptr;
        }

        static constexpr float STEP{ 1.f };  // Adjusts the resolution of the nodes

        // https://stackoverflow.com/a/29375830
        alignas(16) float **ppXPositions;
        alignas(16) float **ppYPositions;
        alignas(16) float **ppZPositions;
        alignas(16) float **ppIsoValues;    // Index first subscript using _3DTo1DIndex
    };
}
