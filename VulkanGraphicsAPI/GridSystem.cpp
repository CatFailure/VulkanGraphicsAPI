#include "pch.hpp"
#include "GridSystem.hpp"

namespace SolEngine::DOD
{
    GridSystem::GridSystem(const glm::uvec3 &dimensions)
    {
        SetDimensions(dimensions);
    }

    GridSystem::GridSystem(const size_t scalarDimensions)
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
    }

    void GridSystem::SetDimensions(const size_t scalarDimensions)
    {
        DBG_ASSERT_MSG(IsWithinAxisNodeCountLimit(scalarDimensions), 
                       "Too many Nodes!");

        _dimensions = glm::uvec3(scalarDimensions);
        _gridNodes  = GridNodes(scalarDimensions);
    }
}