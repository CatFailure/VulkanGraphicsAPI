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

    SolGrid::~SolGrid()
    {
        nodes.Free();
    }

    size_t SolGrid::GetNodeCount() const
    {
        const glm::uint dimensionsVolume = _dimensions.x * _dimensions.y * _dimensions.z;

        return (size_t)(dimensionsVolume * (double)_step);
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

    void SolGrid::TraverseGrid(const TraverseCubesCallback_t& callback)
    {
        // We have to index this way to account for resolution (step)
        uint32_t zIndex(0);
        for (float z(_minBounds.z); z < _maxBounds.z; z += _step)
        {
            uint32_t yIndex(0);
            for (float y(_minBounds.y); y > _maxBounds.y; y -= _step)
            {
                uint32_t xIndex(0);
                for (float x(_minBounds.x); x < _maxBounds.x; x += _step)
                {
                    callback(xIndex, 
                             yIndex, 
                             zIndex);

                    ++xIndex;
                }

                ++yIndex;
            }

            ++zIndex;
        }
    }

    void SolGrid::InitialiseNodes()
    {
        _rDiagnosticData.memoryAllocatedBytes = nodes.AllocateDataArrays();

        size_t bytesInUse = 0;

        bytesInUse += GenerateVertices<Axis::X>(nodes.pXVertices, _minBounds.x, _maxBounds.x, _step);
        bytesInUse += GenerateVertices<Axis::Y>(nodes.pYVertices, _minBounds.y, _maxBounds.y, _step);
        bytesInUse += GenerateVertices<Axis::Z>(nodes.pZVertices, _minBounds.z, _maxBounds.z, _step);
        bytesInUse += DefaultLiveNeighbours(nodes.pLiveNeighbours, GetNodeCount());

        _rDiagnosticData.memoryUsedBytes = bytesInUse;
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