#include "SolGrid.hpp"

namespace SolEngine
{
    SolGrid::SolGrid(GridSettings& rGridData, 
                     DiagnosticData& rDiagnosticData)
        : _rDiagnosticData(rDiagnosticData),
          _rGridSettings(rGridData)
    {
        // Allocate the cells
        _rDiagnosticData.gridMemoryAllocatedBytes = cells.AllocateDataArrays();

        Initialise();
    }

    SolGrid::~SolGrid()
    {
        cells.Free();
    }

    void SolGrid::Reset()
    {
        const size_t nodeCount = _rGridSettings.GetNodeCount();

        DefaultLiveNeighbours(cells.pLiveNeighbourCounts, nodeCount);
        GenerateRandomStates(cells.pCellStates, nodeCount);
    }

    void SolGrid::Initialise()
    {
        _isGridDataValid = !AreCellLimitsExceeded(_rGridSettings.dimensions);

        // Bad grid data - will cause problems
        // so just back out...
        if (!_isGridDataValid)
        {
            return;
        }

        SetBoundsWithDimensions(_rGridSettings.dimensions);
        InitialiseNodes();
    }

    void SolGrid::TraverseAllGridCells(const TraverseCubesCallback_t& callback)
    {
        const glm::uvec3 gridDimensions = GetDimensions();

        for (uint32_t z(0U); z < gridDimensions.z; ++z)
        {
            for (uint32_t y(0U); y < gridDimensions.y; ++y)
            {
                for (uint32_t x(0U); x < gridDimensions.x; ++x)
                {
                    callback(x, y, z);
                }
            }
        }
    }

    void SolGrid::InitialiseNodes()
    {
        const size_t nodeCount = _rGridSettings.GetNodeCount();
        size_t nodesBytesInUse(0);

        nodesBytesInUse += GenerateVertices<Axis::X>(cells.pXVertices, _minBounds.x, _maxBounds.x);
        nodesBytesInUse += GenerateVertices<Axis::Y>(cells.pYVertices, _minBounds.y, _maxBounds.y);
        nodesBytesInUse += GenerateVertices<Axis::Z>(cells.pZVertices, _minBounds.z, _maxBounds.z);
        nodesBytesInUse += DefaultLiveNeighbours(cells.pLiveNeighbourCounts, nodeCount);
        nodesBytesInUse += GenerateRandomStates(cells.pCellStates, nodeCount);

        _rDiagnosticData.gridMemoryUsedBytes = nodesBytesInUse;
    }

    void SolGrid::SetBoundsWithDimensions(const glm::uvec3& dimensions)
    {
        const glm::vec3 halfExtents = (glm::vec3)dimensions * 0.5f;    // Get half dimensions

        _minBounds = glm::vec3(-halfExtents.x, halfExtents.y, -halfExtents.z);
        _maxBounds = glm::vec3(halfExtents.x, -halfExtents.y, halfExtents.z);
    }

    bool SolGrid::AreCellLimitsExceeded(const glm::uvec3& dimensions)
    {
        // If any of these fail, the whole check should fail
        return IsMaxCellsPerAxisExceeded(dimensions.x) || 
               IsMaxCellsPerAxisExceeded(dimensions.y) || 
               IsMaxCellsPerAxisExceeded(dimensions.z);
    }

    bool SolGrid::IsMaxCellsPerAxisExceeded(const uint32_t axisSize)
    {
        return axisSize > MAX_CELLS_PER_AXIS_COUNT;
    }
}