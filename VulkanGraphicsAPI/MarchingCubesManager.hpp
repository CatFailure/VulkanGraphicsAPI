#pragma once
#include "GridSystem.hpp"

using namespace SolEngine::DOD;

namespace SolEngine::Manager
{
    class MarchingCubesManager
    {
    public:
        MarchingCubesManager() = default;
        MarchingCubesManager(const glm::uvec3 &dimensions);
        MarchingCubesManager(const glm::uint scalarDimensions);

    private:
        GridSystem _gridSystem;
    };
}
