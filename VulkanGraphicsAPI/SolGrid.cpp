#include "SolGrid.hpp"

namespace SolEngine
{
    SolGrid::SolGrid(const glm::vec3& dimensions, 
                     DiagnosticData& rDiagnosticData)
        : _rDiagnosticData(rDiagnosticData)
    {
        SetDimensions(dimensions, _step);
        InitialiseNodes();
    }

    SolGrid& SolGrid::SetDimensions(const glm::uvec3& dimensions, 
                                          const float step)
    {
        const char* limitExceededMessage = "Grid dimensions exceed maximum node count!";

        DBG_ASSERT_MSG(AreDimensionsWithinMaxCubeLimits(dimensions, 
                                                        step), 
                       limitExceededMessage);

        _dimensions = dimensions;

        SetBoundsWithDimensions(dimensions);

        return *this;
    }

    void SolGrid::InitialiseNodes()
    {
        _rDiagnosticData.memoryAllocatedBytes = _nodes.AllocateDataArrays();
    }

    void SolGrid::SetBoundsWithDimensions(const glm::uvec3& dimensions)
    {
        const glm::vec3 halfExtents = (glm::vec3)dimensions * 0.5f;    // Get half dimensions

        _minBounds = glm::vec3(-halfExtents.x, halfExtents.y, -halfExtents.z);
        _maxBounds = glm::vec3(halfExtents.x, -halfExtents.y, halfExtents.z);
    }

    bool SolGrid::AreDimensionsWithinMaxCubeLimits(const glm::uvec3& dimensions, 
                                                      const float step)
    {
        return IsAxisSizeWithinMaxCubeLimit(dimensions.x, step) && 
               IsAxisSizeWithinMaxCubeLimit(dimensions.y, step) && 
               IsAxisSizeWithinMaxCubeLimit(dimensions.z, step);
    }

    bool SolGrid::IsAxisSizeWithinMaxCubeLimit(const uint32_t axisSize, 
                                                  const float step)
    {
        const uint32_t scaledAxisSize = (uint32_t)(axisSize / step);

        return !(scaledAxisSize > MAX_CUBES_PER_AXIS_COUNT);
    }
}