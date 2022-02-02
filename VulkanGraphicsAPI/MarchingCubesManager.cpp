#include "pch.hpp"
#include "MarchingCubesManager.hpp"

namespace SolEngine::Manager
{
    MarchingCubesManager::MarchingCubesManager(const glm::uvec3 &dimensions)
        : _gridSystem(dimensions)
    {}

    MarchingCubesManager::MarchingCubesManager(const glm::uint scalarDimensions)
        : _gridSystem(scalarDimensions)
    {}
}