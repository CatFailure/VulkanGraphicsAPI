#pragma once
#include <glm/glm.hpp>

namespace Utility
{
    static constexpr float SPHERE_RADIUS{ 2.5f };

    static size_t CoordTo1DArrayIndex(const glm::int32 x, 
                                      const glm::int32 y, 
                                      const glm::int32 z,
                                      const glm::uvec3 &dimensions)
    {
        const glm::uvec3 sqrDimensions = dimensions * dimensions;

        return (z * sqrDimensions.x) + (y * dimensions.y) + x;
    }

    static void CoordToIsoValue(const glm::int32 x, 
                                const glm::int32 y, 
                                const glm::int32 z, 
                                float *pOutIsoValue, 
                                const glm::uvec3 &dimensions)
    {
        const float sqrX = (float)x * x;
        const float sqrY = (float)y * y;
        const float sqrZ = (float)z * z;

        *pOutIsoValue = SPHERE_RADIUS - sqrtf(sqrX + sqrY + sqrZ);
    }

    static void CoordsToIsoValues(const glm::int32 *pXPositions, 
                                  const glm::int32 *pYPositions, 
                                  const glm::int32 *pZPositions, 
                                  float *pOutIsoValues,
                                  const glm::uvec3 &dimensions)
    {
        for (glm::uint32 z = 0; z < dimensions.z; ++z)
        {
            for (glm::uint32 y = 0; y < dimensions.y; ++y)
            {
                for (glm::uint32 x = 0; x < dimensions.x; ++x)
                {
                    const float sqrX = (float)x * x;
                    const float sqrY = (float)y * y;
                    const float sqrZ = (float)z * z;
                    const size_t isoValueIndex = CoordTo1DArrayIndex(x, y, z, dimensions);

                    pOutIsoValues[isoValueIndex] = SPHERE_RADIUS - sqrtf(sqrX + sqrY + sqrZ);
                }
            }
        }
    }

    static std::tuple<size_t, size_t> CornerIndicesFromEdgeIndex(const size_t edgeIndex)
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
