#pragma once
#include "Constants.hpp"

using namespace SolEngine::Data;

namespace SolEngine::DOD
{
    struct GridNodes
    {
        alignas(16) float xPositions[MAX_GRID_NODES]{ 0 };
        alignas(16) float yPositions[MAX_GRID_NODES]{ 0 };
        alignas(16) float zPositions[MAX_GRID_NODES]{ 0 };

        alignas(16) bool  isAlives[MAX_GRID_NODES];
    };
}
