#include "MarchingCubesSystem.hpp"

namespace SolEngine::System
{
    MarchingCubesSystem::MarchingCubesSystem(SolDevice& rSolDevice, 
                                             SolGrid& rSolGrid,
                                             DiagnosticData& rDiagnosticData)
        : _rSolDevice(rSolDevice),
          _rSolGrid(rSolGrid),
          _rDiagnosticData(rDiagnosticData),
          _marchingCubesObject(SolGameObject::CreateGameObject())
    {}

    void MarchingCubesSystem::March()
    {
        // Delete all previous vertices
        _vertices.clear();

        const glm::uvec3  gridDimensions  = _rSolGrid.GetDimensions();
        const bool*       pGridCellStates = _rSolGrid.cells.pCellStates;

        _rSolGrid.TraverseAllGridCells([&](const int xIndex, 
                                           const int yIndex, 
                                           const int zIndex) 
                                       {
                                           bool cubeIsoValues[CUBE_VERTEX_COUNT]{ 0 };

                                           // Retrieve the cubes "Iso Values" using the grid cell states
                                           GetCubeIsoValues(cubeIsoValues, 
                                                            pGridCellStates,
                                                            xIndex, 
                                                            yIndex, 
                                                            zIndex, 
                                                            gridDimensions);
                                       
                                           // Calculate the cube index to pull from the Tri-table
                                           const uint32_t cubeIndex = GetCubeIndex(cubeIsoValues);
                                       
                                           // Look up the triangulation for the cubeIndex
                                           const Index_t* pEdgeIndices = TRI_TABLE[cubeIndex];

                                           if (*pEdgeIndices == -1)
                                           {
                                               return;
                                           }
                                       
                                           CreateVertices(_rSolGrid.cells,
                                                          pEdgeIndices, 
                                                          xIndex, 
                                                          yIndex, 
                                                          zIndex);
                                       });

        UpdateGameObjectModel();

        const size_t vertexCount = _vertices.size();
        const size_t triCount    = vertexCount / 3U;

        _rDiagnosticData.vertexCount = vertexCount;
        _rDiagnosticData.triCount    = triCount;

        //printf_s("Created: %zu Vertices\n", vertexCount);
        //printf_s("Created: %zu Tris\n", triCount);
    }

    uint32_t MarchingCubesSystem::GetCubeIndex(const bool* pNodeStates)
    {
        uint32_t cubeIndex(0);

        for (uint32_t i(0); i < CUBE_VERTEX_COUNT; ++i)
        {
            if (!pNodeStates[i])
            {
                continue;
            }

            cubeIndex |= 1 << i;
        }

        return cubeIndex;
    }

    void MarchingCubesSystem::GetCubeIsoValues(bool* pOutCubeIsoValues, 
                                               const bool* pGridCellStates, 
                                               const int xIndex, 
                                               const int yIndex, 
                                               const int zIndex, 
                                               const glm::vec3& gridDimensions)
    {
        const int adjOffset = 1;
        const int adjXIndex = xIndex + adjOffset;
        const int adjYIndex = yIndex + adjOffset;
        const int adjZIndex = zIndex + adjOffset;

        if (!(adjXIndex < gridDimensions.x) ||
            !(adjYIndex < gridDimensions.y) ||
            !(adjZIndex < gridDimensions.z))
        {
            // Out-of-range
            return;
        }

        // Retrieve a "Cube" of cell states
        pOutCubeIsoValues[0] = pGridCellStates[_3DTo1DIndex(xIndex,    yIndex,    zIndex,    gridDimensions)];
        pOutCubeIsoValues[1] = pGridCellStates[_3DTo1DIndex(adjXIndex, yIndex,    zIndex,    gridDimensions)];
        pOutCubeIsoValues[2] = pGridCellStates[_3DTo1DIndex(adjXIndex, yIndex,    adjZIndex, gridDimensions)];
        pOutCubeIsoValues[3] = pGridCellStates[_3DTo1DIndex(xIndex,    yIndex,    adjZIndex, gridDimensions)];
        pOutCubeIsoValues[4] = pGridCellStates[_3DTo1DIndex(xIndex,    adjYIndex, zIndex,    gridDimensions)];
        pOutCubeIsoValues[5] = pGridCellStates[_3DTo1DIndex(adjXIndex, adjYIndex, zIndex,    gridDimensions)];
        pOutCubeIsoValues[6] = pGridCellStates[_3DTo1DIndex(adjXIndex, adjYIndex, adjZIndex, gridDimensions)];
        pOutCubeIsoValues[7] = pGridCellStates[_3DTo1DIndex(xIndex,    adjYIndex, adjZIndex, gridDimensions)];
    }

    void MarchingCubesSystem::CreateVertices(Cells& rNodes,
                                             const Index_t* pEdgeIndices, 
                                             const int xIndex, 
                                             const int yIndex, 
                                             const int zIndex)
    {
        const glm::uvec3 gridDimensions = _rSolGrid.GetDimensions();

        for (uint32_t i(0); i < TRI_TABLE_INDEX_COUNT; ++i)
        {
            if (pEdgeIndices[i] == -1)
            {
                return;
            }

            // Find the 2 corners that create the edge
            const std::pair<Index_t, Index_t> cornerIndices = 
                CornerIndicesFromEdgeIndex(pEdgeIndices[i]);

            // Find edge midpoint
            const glm::vec3 vertexPosition = GetEdgeVertexPosition(rNodes,
                                                                   xIndex,
                                                                   yIndex,
                                                                   zIndex, 
                                                                   cornerIndices);

            const glm::vec3 vertexColour
            {
                (float)xIndex / gridDimensions.x,  // r
                (float)yIndex / gridDimensions.y,  // g
                (float)zIndex / gridDimensions.z   // b
            };

            // Compact Voxel Array
            // Push back vertex...
            _vertices.push_back(Vertex(vertexPosition, vertexColour));
        }
    }

    glm::vec3 MarchingCubesSystem::GetEdgeVertexPosition(Cells& rNodes, 
                                                         const int xIndex,
                                                         const int yIndex, 
                                                         const int zIndex, 
                                                         const std::pair<Index_t, Index_t>& cornerIndices)
    {
        const int xRowWidth = xIndex * CUBE_VERTEX_COUNT;
        const int yRowWidth = yIndex * CUBE_VERTEX_COUNT;
        const int zRowWidth = zIndex * CUBE_VERTEX_COUNT;

        const int* pXVertices = &rNodes.pXVertices[xRowWidth];
        const int* pYVertices = &rNodes.pYVertices[yRowWidth];
        const int* pZVertices = &rNodes.pZVertices[zRowWidth];

        const Index_t indexA = cornerIndices.first;
        const Index_t indexB = cornerIndices.second;

        const float half = 0.5f;

        return glm::vec3((pXVertices[indexA] + pXVertices[indexB]) * half,
                         (pYVertices[indexA] + pYVertices[indexB]) * half, 
                         (pZVertices[indexA] + pZVertices[indexB]) * half);
    }

    void MarchingCubesSystem::UpdateGameObjectModel()
    {
        // Any vertices to work with?
        if (_vertices.empty())
        {
            return;
        }

        std::shared_ptr<SolModel> marchingCubeModel = 
            std::make_shared<SolModel>(_rSolDevice, 
                                       _vertices.data(), 
                                       (uint32_t)_vertices.size());

        // Any model to work with?
        if (marchingCubeModel == nullptr)
        {
            return;
        }

        _marchingCubesObject.SetModel(marchingCubeModel);
    }
}