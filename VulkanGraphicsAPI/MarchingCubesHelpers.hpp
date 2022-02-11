#pragma once
#include <glm/glm.hpp>

#include "Constants.hpp"
#include "Axis.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Enumeration;

namespace Utility
{
    static float MinIsoValueGenerated(1000.f), MaxIsoValueGenerated(-1000.f);

    static uint32_t _3DTo1DIndex(const uint32_t xIndex, 
                                 const uint32_t yIndex, 
                                 const uint32_t zIndex,
                                 const glm::vec3 &dimensions,
                                 const float step)
    {
        // Always 0
        if (xIndex == 0 && 
            yIndex == 0 && 
            zIndex == 0)
        {
            return 0;
        }

        const glm::vec3 scaledDimensions = dimensions / step;
        const glm::vec3 sqrDimensions = scaledDimensions * scaledDimensions;

        // Convert 3D array indexes into a 1D array index
        return (uint32_t)((zIndex * sqrDimensions.x) + (yIndex * scaledDimensions.y) + xIndex);
    }

    /// <summary>
    /// Converts a 3D coordinate in world space into an isoValue.
    /// </summary>
    static void CoordToIsoValue(const float x, 
                                const float y, 
                                const float z, 
                                float *pOutIsoValue)
    {
        const float sqrX = x * x;
        const float sqrY = y * y;
        const float sqrZ = z * z;
        const float generatedIsoValue = SPHERE_RADIUS - sqrtf(sqrX + sqrY + sqrZ);

        if (generatedIsoValue < MinIsoValueGenerated)
        {
            MinIsoValueGenerated = generatedIsoValue;
        }
        else if (generatedIsoValue > MaxIsoValueGenerated)
        {
            MaxIsoValueGenerated = generatedIsoValue;
        }

        // Creates a sphere
        *pOutIsoValue = generatedIsoValue;
    }

    static void VerticesToIsoValues(const float *pXVertices, 
                                    const float *pYVertices, 
                                    const float *pZVertices,
                                    float *pOutIsoValues)
    {
        for (size_t i = 0; i < CUBE_VERTEX_COUNT; ++i)
        {
            CoordToIsoValue(*(pXVertices + i), 
                            *(pYVertices + i), 
                            *(pZVertices + i), 
                            pOutIsoValues + i);

        }
    }

    static float CalculateInterpolationScalar(const float isoValueA, 
                                              const float isoValueB, 
                                              const float isoLevel)
    {
        return (isoLevel - isoValueA) / (isoValueB - isoValueA);
    }

    static std::pair<Index_t, Index_t> CornerIndicesFromEdgeIndex(const Index_t edgeIndex)
    {
        switch (edgeIndex)
        {
        case 0:
            return { 0, 1 };
        case 1:
            return { 1, 2 };
        case 2:
            return { 2, 3 };
        case 3:
            return { 3, 0 };
        case 4:
            return { 4, 5 };
        case 5:
            return { 5, 6 };
        case 6:
            return { 6, 7 };
        case 7:
            return { 7, 4 };
        case 8:
            return { 0, 4 };
        case 9:
            return { 1, 5 };
        case 10:
            return { 2, 6 };
        case 11:
            return { 3, 7 };
        default:
            return { -1, -1 };
        }
    }

    static void DimensionsToBounds(const glm::vec3 &dimensions, 
                                   glm::vec3 *pOutMinBounds, 
                                   glm::vec3 *pOutMaxBounds)
    {
        const glm::vec3 halfExtents = dimensions * 0.5f;    // Get half dimensions

        *pOutMinBounds = glm::vec3(-halfExtents.x, halfExtents.y, -halfExtents.z);
        *pOutMaxBounds = glm::vec3(halfExtents.x, -halfExtents.y, halfExtents.z);
    }

    /// <summary>
    /// Generates Vertices along an axis
    /// </summary>
    /// <returns>Bytes used.</returns>
    template<Axis>
    static size_t GenerateVertices(float *pOutPositions,
                                   const float minValue, 
                                   const float maxValue,
                                   const float step) = delete;

    template<>
    static size_t GenerateVertices<Axis::X>(float *pOutXPositions,
                                            const float minValue, 
                                            const float maxValue,
                                            const float step)
    {
        const size_t floatSizeBytes = sizeof(float);
        size_t       bytesInUse     = 0;
        uint32_t     rowIndex       = 0;

        for (float xPos = minValue; xPos < maxValue; xPos += step)
        {
            const float adjXPos     = xPos + step;
            const uint32_t rowWidth = rowIndex * CUBE_VERTEX_COUNT;

            pOutXPositions[rowWidth]     = xPos;
            pOutXPositions[rowWidth + 1] = adjXPos;
            pOutXPositions[rowWidth + 2] = adjXPos;
            pOutXPositions[rowWidth + 3] = xPos;
            pOutXPositions[rowWidth + 4] = xPos;
            pOutXPositions[rowWidth + 5] = adjXPos;
            pOutXPositions[rowWidth + 6] = adjXPos;
            pOutXPositions[rowWidth + 7] = xPos;

            ++rowIndex;
            bytesInUse += (floatSizeBytes * CUBE_VERTEX_COUNT);
        }

        printf_s("Generated %u X-Positions.\n", rowIndex);

        return bytesInUse;
    }

    template<>
    static size_t GenerateVertices<Axis::Y>(float *pOutYPositions,
                                            const float minValue, 
                                            const float maxValue,
                                            const float step)
    {
        const size_t floatSizeBytes = sizeof(float);
        size_t       bytesInUse     = 0;
        uint32_t     rowIndex       = 0;

        for (float yPos = minValue; yPos > maxValue; yPos -= step)
        {
            const float adjYPos     = yPos - step;
            const uint32_t rowWidth = rowIndex * CUBE_VERTEX_COUNT;

            pOutYPositions[rowWidth]     = yPos;
            pOutYPositions[rowWidth + 1] = yPos;
            pOutYPositions[rowWidth + 2] = yPos;
            pOutYPositions[rowWidth + 3] = yPos;
            pOutYPositions[rowWidth + 4] = adjYPos;
            pOutYPositions[rowWidth + 5] = adjYPos;
            pOutYPositions[rowWidth + 6] = adjYPos;
            pOutYPositions[rowWidth + 7] = adjYPos;

            ++rowIndex;
            bytesInUse += (floatSizeBytes * CUBE_VERTEX_COUNT);
        }

        printf_s("Generated %u Y-Positions.\n", rowIndex);

        return bytesInUse;
    }

    template<>
    static size_t GenerateVertices<Axis::Z>(float *pOutZPositions,
                                            const float minValue, 
                                            const float maxValue,
                                            const float step)
    {
        const size_t floatSizeBytes = sizeof(float);
        size_t       bytesInUse     = 0;
        uint32_t     rowIndex       = 0;

        for (float zPos = minValue; zPos < maxValue; zPos += step)
        {
            const float adjZPos     = zPos + step;
            const uint32_t rowWidth = rowIndex * CUBE_VERTEX_COUNT;

            pOutZPositions[rowWidth]     = zPos;
            pOutZPositions[rowWidth + 1] = zPos;
            pOutZPositions[rowWidth + 2] = adjZPos;
            pOutZPositions[rowWidth + 3] = adjZPos;
            pOutZPositions[rowWidth + 4] = zPos;
            pOutZPositions[rowWidth + 5] = zPos;
            pOutZPositions[rowWidth + 6] = adjZPos;
            pOutZPositions[rowWidth + 7] = adjZPos;

            ++rowIndex;
            bytesInUse += (floatSizeBytes * CUBE_VERTEX_COUNT);
        }

        printf_s("Generated %u Z-Positions.\n", rowIndex);

        return bytesInUse;
    }
}
