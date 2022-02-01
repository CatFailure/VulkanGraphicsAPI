#pragma once
#include "GridNodes.hpp"

using namespace Utility;

namespace SolEngine::DOD
{
    class GridSystem
    {
    public:
        GridSystem() = delete;
        GridSystem(const glm::vec3 &dimensions);
        GridSystem(const size_t scalarDimensions);

    private:
        bool IsWithinAxisNodeCountLimit(const size_t count) const { return !(count > MAX_AXIS_NODE_COUNT); }

        glm::vec3 _dimensions{ 0.f };
        GridNodes _gridNodes{};
    };
}
