#include "SolGrid.hpp"

namespace SolEngine
{
    SolGrid::SolGrid(GridData& rGridData, 
                     DiagnosticData& rDiagnosticData)
        : _rDiagnosticData(rDiagnosticData),
          _rGridData(rGridData)
    {
        Initialise();
    }

    SolGrid::~SolGrid()
    {
        nodes.Free();
    }

    void SolGrid::Initialise()
    {
        SetBoundsWithDimensions(_rGridData.dimensions);
        InitialiseNodes();
    }

    void SolGrid::TraverseGrid(const TraverseCubesCallback_t& callback)
    {
        const float gridStep = _rGridData.step;

        // We have to index this way to account for resolution (step)
        uint32_t zIndex(0);
        for (float z(_minBounds.z); z < _maxBounds.z; z += gridStep)
        {
            uint32_t yIndex(0);
            for (float y(_minBounds.y); y > _maxBounds.y; y -= gridStep)
            {
                uint32_t xIndex(0);
                for (float x(_minBounds.x); x < _maxBounds.x; x += gridStep)
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
        const float gridStep = _rGridData.step;
        size_t bytesInUse(0);

        _rDiagnosticData.memoryAllocatedBytes = nodes.AllocateDataArrays();

        bytesInUse += GenerateVertices<Axis::X>(nodes.pXVertices, _minBounds.x, _maxBounds.x, gridStep);
        bytesInUse += GenerateVertices<Axis::Y>(nodes.pYVertices, _minBounds.y, _maxBounds.y, gridStep);
        bytesInUse += GenerateVertices<Axis::Z>(nodes.pZVertices, _minBounds.z, _maxBounds.z, gridStep);
        bytesInUse += DefaultLiveNeighbours(nodes.pLiveNeighbours, _rGridData.GetNodeCount());

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