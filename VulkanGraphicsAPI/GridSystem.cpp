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

        TraverseGridNodes([this](const glm::int32 x, 
                                 const glm::int32 y,
                                 const glm::int32 z) 
        {
            const size_t isoValueIndex = CoordTo1DArrayIndex(x, y, z, _dimensions);

            CoordToIsoValue(x, y, z, 
                            &_gridNodes.isoValues[isoValueIndex], 
                            _dimensions);
        });
    }

    void GridSystem::SetDimensions(const glm::uint scalarDimensions)
    {
        SetDimensions(glm::uvec3(scalarDimensions));
    }

    float GridSystem::GetIsoValueAtCoord(const glm::int32 x, 
                                         const glm::int32 y, 
                                         const glm::int32 z) const
    {
        const size_t index = CoordTo1DArrayIndex(x, y, z, _dimensions);

        return _gridNodes.isoValues[index];
    }

    void GridSystem::TraverseGridNodes(const TraverseNodesCallback_t &callback)
    {
        // Currently dimensions and coordinates are 1:1.
        // In the future we could adjust the "resolution" of the grid,
        // allowing for floating point coordinates that aren't 1:1
        for (glm::uint z = 0; z < _dimensions.z; ++z)
        {
            for (glm::uint y = 0; y < _dimensions.y; ++y)
            {
                for (glm::uint x = 0; x < _dimensions.x; ++x)
                {
                    callback(x, y, z);
                }
            }
        }
    }
}