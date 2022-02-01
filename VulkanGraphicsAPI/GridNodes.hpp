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
            for (size_t x = 0; x < dimensions.x; ++x)
            {
                xPositions[x] = x;
            }

            for (size_t y = 0; y < dimensions.y; ++y)
            {
                yPositions[y] = -y;
            }

            for (size_t z = 0; z < dimensions.z; ++z)
            {
                zPositions[z] = z;
            }
        }

        GridNodes(const size_t scalarDimensions)
        {
            for (size_t i = 0; i < scalarDimensions; ++i)
            {
                xPositions[i] = i;
                yPositions[i] = -i; 
                zPositions[i] = i;
            }
        }

        alignas(16) float xPositions[MAX_AXIS_NODE_COUNT]{ 0 };
        alignas(16) float yPositions[MAX_AXIS_NODE_COUNT]{ 0 };
        alignas(16) float zPositions[MAX_AXIS_NODE_COUNT]{ 0 };

        alignas(16) bool  isAlives[MAX_AXIS_NODE_COUNT];
    };
}
