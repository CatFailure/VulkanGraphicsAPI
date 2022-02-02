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
        _nodes  = GridNodes(dimensions);

        CalculateIsoValues();
    }

    void GridSystem::SetDimensions(const glm::uint scalarDimensions)
    {
        SetDimensions(glm::uvec3(scalarDimensions));
    }

    float GridSystem::GetIsoValueAtCoord(const float x, 
                                         const float y, 
                                         const float z) const
    {
        const size_t index = _3DTo1DIndex(x, y, z, 
                                          _dimensions, 
                                          _nodes.step);

        return _nodes.isoValues[index];
    }

    void GridSystem::TraverseGridNodes(const TraverseNodesCallback_t& callback)
    {
        const float step = _nodes.step;

        size_t zIndex(0);
        for (float z = 0; z < _dimensions.z; z += step)
        {
            size_t yIndex(0);
            for (float y = 0; y < _dimensions.y; y += step)
            {
                size_t xIndex(0);
                for (float x = 0; x < _dimensions.x; x += step)
                {
                    callback(_nodes.xPositions[xIndex], 
                             _nodes.yPositions[yIndex],
                             _nodes.zPositions[zIndex]);

                    ++xIndex;
                }

                ++yIndex;
            }

            ++zIndex;
        }
    }

    void GridSystem::CalculateIsoValues()
    {
        TraverseGridNodes([this](const float x, 
                                 const float y,
                                 const float z) 
                          {
                              const size_t isoValueIndex = _3DTo1DIndex(x, y, z,
                                                                        _dimensions, 
                                                                        _nodes.step);

                              CoordToIsoValue(x, y, z, 
                                              &_nodes.isoValues[isoValueIndex], 
                                              _dimensions);
                          });
    }

    void GridSystem::Update(const float deltaTime)
    {
    }
}