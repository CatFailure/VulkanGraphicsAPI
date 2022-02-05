#pragma once
#include <glm/glm.hpp>

#include "Constants.hpp"

using namespace SolEngine::Data;

namespace Utility
{
    static constexpr float SPHERE_RADIUS{ 7.5f };

    static size_t _3DTo1DIndex(const size_t xIndex, 
                               const size_t yIndex, 
                               const size_t zIndex,
                               const glm::uvec3 &dimensions)
    {
        // Always 0
        if (xIndex == 0 && 
            yIndex == 0 && 
            zIndex == 0)
        {
            return 0;
        }

        const glm::uvec3 sqrDimensions = dimensions * dimensions;

        // Convert 3D array indexes into a 1D array index
        return (zIndex * sqrDimensions.x) + (yIndex * dimensions.y) + xIndex;
    }

    static void CopyArray(const float *pSrc, 
                          float *pDst, 
                          const size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            pDst[i] = pSrc[i];
        }
    }

    /// <summary>
    /// Converts a 3D coordinate in world space into an isoValue.
    /// </summary>
    static void CoordToIsoValue(const float x, 
                                const float y, 
                                const float z, 
                                float *pOutIsoValue)
    {
        const float centreX = SPHERE_RADIUS * 0.75f;
        const float centreY = -SPHERE_RADIUS * 0.75f;
        const float centreZ = centreX;

        const float sqrX = (x - centreX) * (x - centreX);
        const float sqrY = (y - centreY) * (y - centreY);
        const float sqrZ = (z - centreZ) * (z - centreZ);

        // Creates a sphere
        *pOutIsoValue = SPHERE_RADIUS - sqrtf(sqrX + sqrY + sqrZ);
    }

    static void VerticesToIsoValues(const float *pXVertices, 
                                    const float *pYVertices, 
                                    const float *pZVertices,
                                    float *pOutIsoValues)
    {
        for (size_t i = 0; i < CUBE_VERTEX_COUNT; ++i)
        {
            CoordToIsoValue(pXVertices[i], 
                            pYVertices[i], 
                            pZVertices[i], 
                            &pOutIsoValues[i]);
        }
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
}
