#include "pch.hpp"
#include "GridSystem.hpp"

namespace SolEngine::DOD
{
    GridSystem::GridSystem(const glm::vec3 &dimensions)
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

    GridSystem::GridSystem(const size_t scalarDimensions)
    {
        DBG_ASSERT_MSG(IsWithinAxisNodeCountLimit(scalarDimensions), 
                       "Too many Nodes!");

        _dimensions = glm::vec3(scalarDimensions);
        _gridNodes  = GridNodes(scalarDimensions);
    }
}