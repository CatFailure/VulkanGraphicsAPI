#pragma once
#include <glm/glm.hpp>

#include "Axis.hpp"
#include "Constants.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Enumeration;

namespace Utility
{
    static inline bool RandomBool()
    {
        return rand() > (RAND_MAX / 2);
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

    static size_t GenerateRandomStates(bool* pOutNodeStates,
                                       const size_t count)
    {
        const size_t stateSizeBytes = sizeof(bool);
        size_t       bytesInUse     = 0;

        for (size_t i = 0; i < count; ++i)
        {
            pOutNodeStates[i] = RandomBool();

            bytesInUse += stateSizeBytes;
        }

        return bytesInUse;
    }

    static size_t DefaultLiveNeighbours(unsigned char* pOutLiveNeighbours,
                                        const size_t count)
    {
        const size_t neighbourCountSizeBytes = sizeof(unsigned char);
        size_t       bytesInUse    = 0;

        for (size_t i = 0; i < count; ++i)
        {
            pOutLiveNeighbours[i] = 0;

            bytesInUse += neighbourCountSizeBytes;
        }

        return bytesInUse;
    }
}
