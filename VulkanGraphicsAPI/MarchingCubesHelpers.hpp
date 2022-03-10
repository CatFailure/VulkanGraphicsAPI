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
                                 const glm::vec3 &dimensions)
    {
        // Always 0
        if (xIndex == 0 && 
            yIndex == 0 && 
            zIndex == 0)
        {
            return 0;
        }

        const glm::vec3 sqrDimensions = dimensions * dimensions;
        const uint32_t returnIndex = (uint32_t)((zIndex * sqrDimensions.x) + (yIndex * dimensions.y) + xIndex);

        // Convert 3D array indexes into a 1D array index
        return returnIndex;
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
}
