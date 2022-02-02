#pragma once
#include "GridNodes.hpp"

using namespace Utility;

namespace SolEngine::DOD
{
    class GridSystem
    {
    public:
        typedef std::function<void(const glm::int32, const glm::int32, const glm::int32)> TraverseNodesCallback_t;

        GridSystem() = default;
        GridSystem(const glm::uvec3 &dimensions);
        GridSystem(const glm::uint scalarDimensions);

        void SetDimensions(const glm::uvec3 &dimensions);
        void SetDimensions(const glm::uint scalarDimensions);

        float GetIsoValueAtCoord(const glm::int32 x, const glm::int32 y, const glm::int32 z) const;
        void TraverseGridNodes(const TraverseNodesCallback_t &callback);

    private:
        bool IsWithinAxisNodeCountLimit(const size_t count) const { return !(count > MAX_AXIS_NODE_COUNT); }

        glm::uvec3 _dimensions{ 0 };
        GridNodes _gridNodes{};
    };
}
