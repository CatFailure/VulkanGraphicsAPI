#pragma once
#include "Constants.hpp"

using namespace SolEngine::Data;

namespace SolEngine::DOD
{
    struct GridNodes
    {
        GridNodes() = default;

        GridNodes(const glm::vec3 dimensions)
        {
            for (glm::int32 x = 0; x < dimensions.x; ++x)
            {
                xPositions[x] = x;
            }

            for (glm::int32 y = 0; y < dimensions.y; ++y)
            {
                yPositions[y] = -y;
            }

            for (glm::int32 z = 0; z < dimensions.z; ++z)
            {
                zPositions[z] = z;
            }
        }

        GridNodes(const glm::int32 scalarDimensions)
        {
            for (glm::int32 i = 0; i < scalarDimensions; ++i)
            {
                xPositions[i] = i;
                yPositions[i] = -i; 
                zPositions[i] = i;
            }
        }

        alignas(16) glm::int32 xPositions[MAX_AXIS_NODE_COUNT]{ 0 };
        alignas(16) glm::int32 yPositions[MAX_AXIS_NODE_COUNT]{ 0 };
        alignas(16) glm::int32 zPositions[MAX_AXIS_NODE_COUNT]{ 0 };

        alignas(16) float isoValues[MAX_NODE_COUNT]{ 0 };
        alignas(16) bool  isAlives[MAX_NODE_COUNT];
    };
}
