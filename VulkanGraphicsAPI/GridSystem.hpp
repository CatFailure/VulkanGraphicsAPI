#pragma once
#include "GridNodes.hpp"

using namespace SolEngine::Interface;
using namespace Utility;

/*
* Instead of looping through all the grid nodes THEN generating the isoSurfaces.
* Create a "cube" at a node to generate the isoSurface, draw it, then move onto the next node.
*/

namespace SolEngine::DOD
{
    class GridSystem : public IMonoBehaviour
    {
    public:
        typedef std::function<void(const float, const float, const float)> TraverseNodesCallback_t;

        GridSystem() = default;
        GridSystem(const glm::uvec3 &dimensions);
        GridSystem(const glm::uint scalarDimensions);

        void SetDimensions(const glm::uvec3 &dimensions);
        void SetDimensions(const glm::uint scalarDimensions);

        float GetIsoValueAtCoord(const float x, const float y, const float z) const;
        void TraverseGridNodes(const TraverseNodesCallback_t &callback);
        void CalculateIsoValues();

        // Inherited via IMonoBehaviour
        virtual void Update(const float deltaTime) override;

    private:
        bool IsWithinAxisNodeCountLimit(const size_t count) const { return !((count / _nodes.step) > MAX_AXIS_NODE_COUNT); }

        glm::uvec3 _dimensions{ 0 };
        GridNodes _nodes{};
    };
}
