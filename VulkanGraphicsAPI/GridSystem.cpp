#include "pch.hpp"
#include "GridSystem.hpp"

namespace SolEngine::DOD
{
    GridSystem::GridSystem(const glm::uvec3 &dimensions)
    {
        SetDimensions(dimensions);
    }

    GridSystem::GridSystem(const glm::uint scalarDimensions)
    {
        SetDimensions(scalarDimensions);
    }

    void GridSystem::SetDimensions(const glm::uvec3 &dimensions)
    {
        DBG_ASSERT_MSG(IsWithinAxisNodeCountLimit(dimensions.x), 
                       "Too many Nodes!");

        DBG_ASSERT_MSG(IsWithinAxisNodeCountLimit(dimensions.y), 
                       "Too many Nodes!");

        DBG_ASSERT_MSG(IsWithinAxisNodeCountLimit(dimensions.z), 
                       "Too many Nodes!");

        _dimensions = dimensions;
        _gridNodes  = GridNodes(dimensions);

        CoordsToIsoValues(_gridNodes.xPositions,
                          _gridNodes.yPositions, 
                          _gridNodes.zPositions, 
                          _gridNodes.isoValues,
                          _dimensions);
    }

    void GridSystem::SetDimensions(const glm::uint scalarDimensions)
    {
        SetDimensions(glm::uvec3(scalarDimensions));
    }

    float GridSystem::GetIsoValueAtCoord(const size_t &x, 
                                         const size_t &y, 
                                         const size_t &z) const
    {
        const size_t index = CoordTo1DArrayIndex(x, y, z, _dimensions);

        return _gridNodes.isoValues[index];
    }
}