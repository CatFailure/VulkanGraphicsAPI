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
            size_t index(0);
            for (float xPos = 0; xPos < dimensions.x; xPos += step)
            {
                xPositions[index] = (float)xPos;

                ++index;
            }

            index = 0;
            for (float yPos = 0; yPos < dimensions.y; yPos += step)
            {
                yPositions[index] = -(float)yPos;

                ++index;
            }

            index = 0;
            for (float zPos = 0; zPos < dimensions.z; zPos += step)
            {
                zPositions[index] = (float)zPos;

                ++index;
            }
        }

        GridNodes(const glm::int32 scalarDimensions)
        {
            size_t index(0);
            for (float pos = 0; pos < scalarDimensions; pos += step)
            {
                xPositions[index] = pos;
                yPositions[index] = -pos; 
                zPositions[index] = pos;

                ++index;
            }
        }

        float step{ 1.f };  // Adjusts the resolution of the nodes

        alignas(16) float xPositions[MAX_AXIS_NODE_COUNT]{ 0 };
        alignas(16) float yPositions[MAX_AXIS_NODE_COUNT]{ 0 };
        alignas(16) float zPositions[MAX_AXIS_NODE_COUNT]{ 0 };

        alignas(16) float isoValues[MAX_NODE_COUNT]{ 0 };
        alignas(16) bool  isAlives[MAX_NODE_COUNT];
    };
}
