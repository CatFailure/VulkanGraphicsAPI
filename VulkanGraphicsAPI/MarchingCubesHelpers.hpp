#pragma once
#include <glm/glm.hpp>

namespace Utility
{
    static constexpr float SPHERE_RADIUS{ 2.5f };

    static size_t CoordTo1DArrayIndex(const glm::uint x, 
                                      const glm::uint y, 
                                      const glm::uint z,
                                      const glm::uvec3 &dimensions)
    {
        const glm::uvec3 sqrDimensions = dimensions * dimensions;

        return ((float)z * sqrDimensions.x) + (y * dimensions.y) + x;
    }

    static void CoordToIsoValue(const glm::uint x, 
                                const glm::uint y, 
                                const glm::uint z, 
                                float *pOutIsoValue, 
                                const glm::uvec3 &dimensions)
    {
        const float sqrX = (float)x * x;
        const float sqrY = (float)y * y;
        const float sqrZ = (float)z * z;

        *pOutIsoValue = SPHERE_RADIUS - sqrtf(sqrX + sqrY + sqrZ);
    }

    static void CoordsToIsoValues(const glm::uint *pXPositions, 
                                  const glm::uint *pYPositions, 
                                  const glm::uint *pZPositions, 
                                  float *pOutIsoValues,
                                  const glm::uvec3 &dimensions)
    {
        for (size_t z = 0; z < dimensions.z; ++z)
        {
            for (size_t y = 0; y < dimensions.y; ++y)
            {
                for (size_t x = 0; x < dimensions.x; ++x)
                {
                    const float sqrX = (float)x * x;
                    const float sqrY = (float)y * y;
                    const float sqrZ = (float)z * z;
                    const size_t isoValueIndex = CoordTo1DArrayIndex((float)x, (float)y, (float)z, dimensions);

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
