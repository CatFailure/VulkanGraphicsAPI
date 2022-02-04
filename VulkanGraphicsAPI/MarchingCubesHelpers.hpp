#pragma once
#include <glm/glm.hpp>

namespace Utility
{
    static constexpr float SPHERE_RADIUS{ 2.5f };

    static glm::uint32 _3DTo1DIndex(const float x, 
                                    const float y, 
                                    const float z,
                                    const glm::uvec3 &dimensions,
                                    const float step)
    {
        // Always 0
        if (x == 0 && 
            y == 0 && 
            z == 0)
        {
            return 0;
        }

        // Coordinates need to be scaled to their step to point to the correct index
        // (Will always resolve to integer types - No data will be lost!)
        const glm::uint32 scaledX      = (glm::uint32)(x / step);
        const glm::uint32 scaledAbsY   = (glm::uint32)(fabs(y) / step); // Y needs to be positive for correct index
        const glm::uint32 scaledZ      = (glm::uint32)(z / step);

        const glm::uvec3 sqrDimensions = dimensions * dimensions;

        // Convert 3D array indexes into a 1D array index
        return (scaledZ * sqrDimensions.x) + (scaledAbsY * dimensions.y) + scaledX;
    }


    /// <summary>
    /// Converts a 3D coordinate in world space into an isoValue.
    /// </summary>
    static void CoordToIsoValue(const float x, 
                                const float y, 
                                const float z, 
                                float *pOutIsoValue, 
                                const glm::uvec3 &dimensions)
    {
        const float sqrX = x * x;
        const float sqrY = y * y;
        const float sqrZ = z * z;

        // Creates a sphere
        *pOutIsoValue = SPHERE_RADIUS - sqrtf(sqrX + sqrY + sqrZ);
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
