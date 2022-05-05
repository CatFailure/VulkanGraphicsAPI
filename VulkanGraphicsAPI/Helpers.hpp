#pragma once
#include <glm/glm.hpp>
#include <time.h>

#include "Axis.hpp"
#include "Constants.hpp"
#include "Typedefs.hpp"
#include "RandomNumberGenerator.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Enumeration;

namespace Utility
{

    /// <summary>
    /// Clamps a value between a limit.
    /// </summary>
    static int Clamp(const int value,
                     const int min, 
                     const int max)
    {
        return (int)fmin(max, fmax(min, value));
    }

    /// <summary>
    /// Clamps a value between a limit.
    /// </summary>
    static float Clamp(const float value,
                       const float min, 
                       const float max)
    {
        return fminf(max, fmaxf(min, value));
    }

    static int ForceEven(const int value)
    {
        // yup
        return value & ~1;
    }

    static int _3DTo1DIndex(const int xIndex, 
                            const int yIndex, 
                            const int zIndex,
                            const glm::uvec3& dimensions)
    {
        const glm::uvec3 validNeighbourDimensions = dimensions - glm::uvec3(1U);

        if (xIndex < 0 ||
            yIndex < 0 ||
            zIndex < 0 ||
            (uint32_t)xIndex > validNeighbourDimensions.x ||
            (uint32_t)yIndex > validNeighbourDimensions.y ||
            (uint32_t)zIndex > validNeighbourDimensions.z)
        {
            // Out-of-range
            return -1;
        }

        // Always 0
        if (xIndex == 0 && 
            yIndex == 0 && 
            zIndex == 0)
        {
            return 0;
        }

        // Convert 3D array indexes into a 1D array index
        // https://stackoverflow.com/a/34363187
        const uint32_t dimensionsX = dimensions.x;
        const int returnIndex = (int)((zIndex * dimensionsX * dimensions.y) + (yIndex * dimensionsX) + xIndex);

        return returnIndex;
    }

    /// <summary>
    /// Generates Vertices along an axis
    /// </summary>
    /// <returns>Bytes used.</returns>
    template<Axis>
    static size_t GenerateVertices(int* pOutPositions,
                                   const int minValue, 
                                   const int maxValue) = delete;

    template<>
    static size_t GenerateVertices<Axis::X>(int* pOutXPositions,
                                            const int minValue, 
                                            const int maxValue)
    {
        size_t   bytesInUse = 0;
        uint32_t rowIndex   = 0;

        for (int xPos = minValue; xPos < maxValue; ++xPos)
        {
            const int      adjXPos  = xPos + 1;
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
            bytesInUse += (INT_SIZE_BYTES * CUBE_VERTEX_COUNT);
        }

        printf_s("Generated %u X-Positions.\n", rowIndex);

        return bytesInUse;
    }

    template<>
    static size_t GenerateVertices<Axis::Y>(int* pOutYPositions,
                                            const int minValue, 
                                            const int maxValue)
    {
        size_t   bytesInUse = 0;
        uint32_t rowIndex   = 0;

        for (int yPos = minValue; yPos > maxValue; --yPos)
        {
            const int      adjYPos  = yPos - 1;
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
            bytesInUse += (INT_SIZE_BYTES * CUBE_VERTEX_COUNT);
        }

        printf_s("Generated %u Y-Positions.\n", rowIndex);

        return bytesInUse;
    }

    template<>
    static size_t GenerateVertices<Axis::Z>(int* pOutZPositions,
                                            const int minValue, 
                                            const int maxValue)
    {
        size_t   bytesInUse = 0;
        uint32_t rowIndex   = 0;

        for (int zPos = minValue; zPos < maxValue; ++zPos)
        {
            const int      adjZPos  = zPos + 1;
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
            bytesInUse += (INT_SIZE_BYTES * CUBE_VERTEX_COUNT);
        }

        printf_s("Generated %u Z-Positions.\n", rowIndex);

        return bytesInUse;
    }

    static size_t GenerateRandomStates(bool* pOutNodeStates,
                                       const size_t count)
    {
        const size_t stateSizeBytes = sizeof(bool);
        size_t       bytesInUse     = 0;

        for (size_t i = 0; i < count; ++i)
        {
            pOutNodeStates[i] = RandomNumberGenerator::GetRandomBool();

            bytesInUse += stateSizeBytes;
        }

        return bytesInUse;
    }

    static size_t DefaultLiveNeighbours(unsigned char* pOutLiveNeighbours,
                                        const size_t count)
    {
        const size_t neighbourCountSizeBytes = sizeof(NeighbourCount_t);
        size_t       bytesInUse              = 0;

        for (size_t i = 0; i < count; ++i)
        {
            pOutLiveNeighbours[i] = 0;

            bytesInUse += neighbourCountSizeBytes;
        }

        return bytesInUse;
    }

    /// <summary>
    /// Converts a 3D coordinate in world space into an isoValue.
    /// </summary>
    static void CoordToIsoValue(const float x, 
                                const float y, 
                                const float z, 
                                float* pOutIsoValue)
    {
        const float sqrX = x * x;
        const float sqrY = y * y;
        const float sqrZ = z * z;
        const float generatedIsoValue = SPHERE_RADIUS - sqrtf(sqrX + sqrY + sqrZ);

        // Creates a sphere
        *pOutIsoValue = generatedIsoValue;
    }

    static void VerticesToIsoValues(const float* pXVertices, 
                                    const float* pYVertices, 
                                    const float* pZVertices,
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

    static void DimensionsToBounds(const glm::vec3& dimensions, 
                                   glm::vec3* pOutMinBounds, 
                                   glm::vec3* pOutMaxBounds)
    {
        const glm::vec3 halfExtents = dimensions * 0.5f;    // Get half dimensions

        *pOutMinBounds = glm::vec3(-halfExtents.x, halfExtents.y, -halfExtents.z);
        *pOutMaxBounds = glm::vec3(halfExtents.x, -halfExtents.y, halfExtents.z);
    }

    template<typename _Ty>
    static void SafeDispose(_Ty*& p)
    {
        delete p;
        p = nullptr;
    }

    template<typename _Ty>
    static void FreeAlignedMallocArray(_Ty* pArr)
    {
        _aligned_free(pArr);

        printf_s("%s - Disposed: %p.\n", __FUNCTION__, pArr);

        pArr = nullptr;
    }

    template<typename _Ty>
    static size_t AlignedMallocContiguousArray(_Ty*& prArr,
                                               const size_t size,
                                               const size_t align = 16U)
    {
        const size_t typeSizeBytes  = sizeof(_Ty);
        const size_t arraySizeBytes = size * align;

        prArr = (_Ty*)_aligned_malloc(arraySizeBytes, align);

        DBG_ASSERT_MSG((prArr != nullptr), "_aligned_malloc Failed!");
        printf_s("%s - Allocated: %zu bytes.\n", __FUNCTION__, arraySizeBytes);

        return arraySizeBytes;
    }

    template<typename _Ty>
    static size_t AlignedMallocContiguous2DArray(_Ty*& prArr, 
                                                 const size_t rowCount, 
                                                 const size_t columnCount,
                                                 const size_t align = 16U)
    {
        // Must be indexed via my_matrix[row_index * row_length + column_index];
        const size_t typeSizeBytes  = sizeof(_Ty);
        const size_t arraySizeBytes = rowCount * columnCount * align;

        prArr = (_Ty *)_aligned_malloc(arraySizeBytes, align);

        DBG_ASSERT_MSG((prArr != nullptr), "_aligned_malloc Failed!");
        printf_s("%s - Allocated: %zu bytes.\n", __FUNCTION__, arraySizeBytes);

        return arraySizeBytes;
    }
}