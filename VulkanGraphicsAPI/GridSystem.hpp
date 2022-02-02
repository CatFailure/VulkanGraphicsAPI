#pragma once
#include "GridNodes.hpp"

using namespace Utility;

namespace SolEngine::DOD
{
    class GridSystem
    {
    public:
        GridSystem() = default;
        GridSystem(const glm::uvec3 &dimensions);
        GridSystem(const glm::uint scalarDimensions);

        void SetDimensions(const glm::uvec3 &dimensions);
        void SetDimensions(const glm::uint scalarDimensions);

        float GetIsoValueAtCoord(const size_t &x, const size_t &y, const size_t &z) const;

    private:
        bool IsWithinAxisNodeCountLimit(const size_t count) const { return !(count > MAX_AXIS_NODE_COUNT); }

        glm::uvec3 _dimensions{ 0 };
        GridNodes _gridNodes{};
    };
}
