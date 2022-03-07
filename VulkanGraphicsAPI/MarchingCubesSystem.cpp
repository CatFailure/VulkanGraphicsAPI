#include "MarchingCubesSystem.hpp"

namespace SolEngine::System
{
    MarchingCubesSystem::MarchingCubesSystem(SolDevice& rSolDevice, 
                                             SolGrid& rSolGrid)
        : _rSolDevice(rSolDevice),
          _rSolGrid(rSolGrid),
          _marchingCubesObject(SolGameObject::CreateGameObject())
    {}

    void MarchingCubesSystem::March(DiagnosticData& rDiagnosticData)
    {
        // Delete all previous vertices
        _vertices.clear();

        const uint32_t   adjOffset            = 1U;
        const glm::vec3  scaledGridDimensions = _rSolGrid.GetScaledDimensions();
        const bool*      pGridCellStates      = _rSolGrid.cells.pCellStates;

        _rSolGrid.TraverseAllGridCells([&](const uint32_t xIndex, 
                                           const uint32_t yIndex, 
                                           const uint32_t zIndex) 
                                       {
                                           const uint32_t adjXIndex = xIndex + adjOffset;
                                           const uint32_t adjYIndex = yIndex + adjOffset;
                                           const uint32_t adjZIndex = zIndex + adjOffset;

                                           if (!(adjXIndex < scaledGridDimensions.x) ||
                                               !(adjYIndex < scaledGridDimensions.y) ||
                                               !(adjZIndex < scaledGridDimensions.z))
                                           {
                                               // Out-of-range
                                               return;
                                           }
                                       
                                           // Retrieve a cubes vertex states
                                           const bool cubeNodeStates[CUBE_VERTEX_COUNT]
                                           {
                                               pGridCellStates[_3DTo1DIndex(xIndex,    yIndex,    zIndex,    scaledGridDimensions)],
                                               pGridCellStates[_3DTo1DIndex(adjXIndex, yIndex,    zIndex,    scaledGridDimensions)],
                                               pGridCellStates[_3DTo1DIndex(adjXIndex, yIndex,    adjZIndex, scaledGridDimensions)],
                                               pGridCellStates[_3DTo1DIndex(xIndex,    yIndex,    adjZIndex, scaledGridDimensions)],
                                               pGridCellStates[_3DTo1DIndex(xIndex,    adjYIndex, zIndex,    scaledGridDimensions)],
                                               pGridCellStates[_3DTo1DIndex(adjXIndex, adjYIndex, zIndex,    scaledGridDimensions)],
                                               pGridCellStates[_3DTo1DIndex(adjXIndex, adjYIndex, adjZIndex, scaledGridDimensions)],
                                               pGridCellStates[_3DTo1DIndex(xIndex,    adjYIndex, adjZIndex, scaledGridDimensions)]
                                           };
                                       
                                           // Calculate the cube index to pull from the Tri-table
                                           const uint32_t cubeIndex = GetCubeIndex(cubeNodeStates);
                                       
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
        const size_t triCount = vertexCount / 3;

        rDiagnosticData.vertexCount = vertexCount;
        rDiagnosticData.triCount = triCount;

        printf_s("Created: %zu Vertices\n", vertexCount);
        printf_s("Created: %zu Tris\n", triCount);
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

    void MarchingCubesSystem::CreateVertices(Cells& rNodes,
                                             const Index_t* pEdgeIndices, 
                                             const uint32_t xIndex, 
                                             const uint32_t yIndex, 
                                             const uint32_t zIndex)
    {
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
            const glm::vec3 vertex = GetEdgeVertexPosition(rNodes,
                                                           xIndex,
                                                           yIndex,
                                                           zIndex, 
                                                           cornerIndices);

            // Compact Voxel Array
            // Push back vertex...
            _vertices.push_back(
                {
                    vertex, 
                    { 
                        (float)xIndex / zIndex, 
                        (float)yIndex / xIndex, 
                        (float)zIndex / yIndex 
                    } 
                });
        }
    }

    glm::vec3 MarchingCubesSystem::GetEdgeVertexPosition(Cells& rNodes, 
                                                         const uint32_t xIndex,
                                                         const uint32_t yIndex, 
                                                         const uint32_t zIndex, 
                                                         const std::pair<Index_t, Index_t>& cornerIndices)
    {
        const uint32_t xRowWidth = xIndex * CUBE_VERTEX_COUNT;
        const uint32_t yRowWidth = yIndex * CUBE_VERTEX_COUNT;
        const uint32_t zRowWidth = zIndex * CUBE_VERTEX_COUNT;

        const float* pXVertices = &rNodes.pXVertices[xRowWidth];
        const float* pYVertices = &rNodes.pYVertices[yRowWidth];
        const float* pZVertices = &rNodes.pZVertices[zRowWidth];

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