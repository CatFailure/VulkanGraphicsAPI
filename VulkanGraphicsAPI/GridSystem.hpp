#pragma once
#include "GridNodes.hpp"

using namespace Utility;

namespace SolEngine::DOD
{
    class GridSystem
    {
    public:
        typedef std::function<void(const glm::uint, const glm::uint, const glm::uint)> TraverseNodesCallback_t;

        GridSystem() = default;
        GridSystem(const glm::uvec3 &dimensions);
        GridSystem(const glm::uint scalarDimensions);

        void SetDimensions(const glm::uvec3 &dimensions);
        void SetDimensions(const glm::uint scalarDimensions);

        float GetIsoValueAtCoord(const glm::uint x, const glm::uint y, const glm::uint z) const;
        void TraverseGridNodes(const TraverseNodesCallback_t &callback);

    private:
        bool IsWithinAxisNodeCountLimit(const size_t count) const { return !(count > MAX_AXIS_NODE_COUNT); }

        glm::uvec3 _dimensions{ 0 };
        GridNodes _gridNodes{};
    };
}
