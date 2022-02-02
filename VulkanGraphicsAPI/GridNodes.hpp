#pragma once
#include "Constants.hpp"

using namespace SolEngine::Data;

namespace SolEngine::DOD
{
    struct GridNodes
    {
        GridNodes() = default;

        GridNodes(const glm::uvec3 dimensions)
        {
            for (glm::uint x = 0; x < dimensions.x; ++x)
            {
                xPositions[x] = x;
            }

            for (glm::uint y = 0; y < dimensions.y; ++y)
            {
                yPositions[y] = y;
            }

            for (glm::uint z = 0; z < dimensions.z; ++z)
            {
                zPositions[z] = z;
            }
        }

        GridNodes(const glm::uint scalarDimensions)
        {
            for (glm::uint i = 0; i < scalarDimensions; ++i)
            {
                xPositions[i] = i;
                yPositions[i] = i; 
                zPositions[i] = i;
            }
        }

        alignas(16) glm::uint xPositions[MAX_AXIS_NODE_COUNT]{ 0 };
        alignas(16) glm::uint yPositions[MAX_AXIS_NODE_COUNT]{ 0 };
        alignas(16) glm::uint zPositions[MAX_AXIS_NODE_COUNT]{ 0 };

        alignas(16) float isoValues[MAX_NODE_COUNT]{ 0 };
        alignas(16) bool  isAlives[MAX_NODE_COUNT];
    };
}
