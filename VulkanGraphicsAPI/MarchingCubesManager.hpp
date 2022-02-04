#pragma once
#include "GridNodes.hpp"
#include "SolDevice.hpp"

using namespace SolEngine::DOD;
using namespace SolEngine::Interface;
using namespace Utility;

/*
* Instead of looping through all the grid nodes THEN generating the isoSurfaces.
* Create a "cube" at a node to generate the isoSurface, draw it, then move onto the next node.
*/

namespace SolEngine::Manager
{
    class MarchingCubesManager : public IMonoBehaviour
    {
    public:
        typedef std::function<void(const float, const float, const float)> TraverseNodesCallback_t;

        MarchingCubesManager(SolDevice &rDevice);
        MarchingCubesManager(SolDevice &rDevice, const glm::uvec3 &dimensions);
        MarchingCubesManager(SolDevice &rDevice, const glm::uint scalarDimensions);

        void SetDimensions(const glm::uvec3 &dimensions);
        void SetDimensions(const glm::uint scalarDimensions);

        float GetIsoValueAtCoord(const float x, const float y, const float z) const;
        void TraverseGridNodes(const TraverseNodesCallback_t &callback);
        void CalculateIsoValues();
        void GenerateIsoSurfaces();

        // Inherited via IMonoBehaviour
        virtual void Update(const float deltaTime) override;

    private:
        bool IsWithinAxisNodeCountLimit(const size_t count) const { return !((count / _nodes.step) > MAX_AXIS_NODE_COUNT); }

        SolDevice &_rSolDevice;

        glm::uvec3 _dimensions{ 0 };
        GridNodes _nodes{};
    };
}
