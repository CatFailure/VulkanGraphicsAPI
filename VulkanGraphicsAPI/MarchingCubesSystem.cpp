#include "MarchingCubesSystem.hpp"

namespace SolEngine::System
{
    MarchingCubesSystem::MarchingCubesSystem(MarchingCubesData& rMarchingCubesData, 
                                             SolDevice& rSolDevice)
        : _rMarchingCubesData(rMarchingCubesData),
          _rSolDevice(rSolDevice),
          _marchingCubesObject(SolGameObject::CreateGameObject())
    {}

 //   void MarchingCubesSystem::GenerateIsoValues(SolGrid& rSolGrid, 
 //                                               DiagnosticData& rDiagnosticData)
	//{
 //       rSolGrid.TraverseGrid([&](const uint32_t xIndex, 
 //                                 const uint32_t yIndex, 
 //                                 const uint32_t zIndex) 
 //                             {
 //                                 const Nodes& rGridNodes = rSolGrid.nodes;

 //                                 const uint32_t isoValuesIndex = _3DTo1DIndex(xIndex, 
 //                                                                              yIndex, 
 //                                                                              zIndex, 
 //                                                                              rSolGrid.GetDimensions(),
 //                                                                              rSolGrid.GetStep());
 //                                 // Grab vertices
 //                                 const float* pXVertices = &rGridNodes.pXVertices[xIndex * CUBE_VERTEX_COUNT];
 //                                 const float* pYVertices = &rGridNodes.pYVertices[yIndex * CUBE_VERTEX_COUNT];
 //                                 const float* pZVertices = &rGridNodes.pZVertices[zIndex * CUBE_VERTEX_COUNT];
 //                                 float*       pIsoValues = &rGridNodes.pIsoValues[isoValuesIndex * CUBE_VERTEX_COUNT];

 //                                 VerticesToIsoValues(pXVertices, 
 //                                                     pYVertices, 
 //                                                     pZVertices, 
 //                                                     pIsoValues);

 //                                 _isoValuesGeneratedCount += CUBE_VERTEX_COUNT;
 //                             });

 //       printf_s("Generated: %zu Iso Values\nMin value: %f\nMaxValue: %f\n", 
 //                _isoValuesGeneratedCount, 
 //                MinIsoValueGenerated, 
 //                MaxIsoValueGenerated);

 //       rDiagnosticData.memoryUsedBytes += sizeof(float) * _isoValuesGeneratedCount;
	//}

    //void MarchingCubesSystem::March(SolGrid& rSolGrid, 
    //                                DiagnosticData& rDiagnosticData)
    //{
    //    _vertices.clear();

    //    rSolGrid.TraverseGrid([&](const uint32_t xIndex, 
    //                              const uint32_t yIndex, 
    //                              const uint32_t zIndex) 
    //                          {
    //                              const uint32_t isoValuesIndex = _3DTo1DIndex(xIndex, 
    //                                                                           yIndex, 
    //                                                                           zIndex, 
    //                                                                           rSolGrid.GetDimensions(),
    //                                                                           rSolGrid.GetStep());

    //                              const float* pIsoValues = &rSolGrid.nodes.pIsoValues[isoValuesIndex * CUBE_VERTEX_COUNT];

    //                              // Calculate the cube index to pull from the Tri-table
    //                              const uint32_t cubeIndex = GetCubeIndex(pIsoValues);

    //                              // Look up the triangulation for the cubeIndex
    //                              const Index_t* pEdgeIndices = TRI_TABLE[cubeIndex];

    //                              CreateVertices(rSolGrid.nodes,
    //                                             pEdgeIndices, 
    //                                             pIsoValues, 
    //                                             xIndex, 
    //                                             yIndex, 
    //                                             zIndex);
    //                          });

    //    UpdateGameObjectModel();

    //    const size_t vertexCount = _vertices.size();
    //    const size_t triCount = vertexCount / 3;

    //    rDiagnosticData.vertexCount = vertexCount;
    //    rDiagnosticData.triCount = triCount;

    //    printf_s("Created: %zu Vertices\n", vertexCount);
    //    printf_s("Created: %zu Tris\n", triCount);
    //}

    void MarchingCubesSystem::March(SolGrid& rSolGrid, 
                                    DiagnosticData& rDiagnosticData)
    {
        _vertices.clear();

        const glm::uvec3 gridDimensions  = rSolGrid.GetDimensions();
        const float      gridStep        = rSolGrid.GetStep();
        const bool*      pGridCellStates = rSolGrid.nodes.pCellStates;

        rSolGrid.TraverseGrid([&](const uint32_t xIndex, 
                                  const uint32_t yIndex, 
                                  const uint32_t zIndex) 
                              {
                                  const uint32_t adjXIndex = xIndex + 1U;
                                  const uint32_t adjYIndex = yIndex + 1U;
                                  const uint32_t adjZIndex = zIndex + 1U;

                                  const uint32_t nodeStateIndex = _3DTo1DIndex(xIndex, 
                                                                               yIndex, 
                                                                               zIndex, 
                                                                               gridDimensions,
                                                                               gridStep);

                                  const bool cubeNodeStates[CUBE_VERTEX_COUNT]
                                  {
                                      pGridCellStates[_3DTo1DIndex(xIndex,    yIndex,    zIndex,    gridDimensions, gridStep)],
                                      pGridCellStates[_3DTo1DIndex(adjXIndex, yIndex,    zIndex,    gridDimensions, gridStep)],
                                      pGridCellStates[_3DTo1DIndex(adjXIndex, yIndex,    adjZIndex, gridDimensions, gridStep)],
                                      pGridCellStates[_3DTo1DIndex(xIndex,    yIndex,    adjZIndex, gridDimensions, gridStep)],
                                      pGridCellStates[_3DTo1DIndex(xIndex,    adjYIndex, zIndex,    gridDimensions, gridStep)],
                                      pGridCellStates[_3DTo1DIndex(adjXIndex, adjYIndex, zIndex,    gridDimensions, gridStep)],
                                      pGridCellStates[_3DTo1DIndex(adjXIndex, adjYIndex, adjZIndex, gridDimensions, gridStep)],
                                      pGridCellStates[_3DTo1DIndex(xIndex,    adjYIndex, adjZIndex, gridDimensions, gridStep)]
                                  };

                                  // Calculate the cube index to pull from the Tri-table
                                  const uint32_t cubeIndex = GetCubeIndex(cubeNodeStates);

                                  // Look up the triangulation for the cubeIndex
                                  const Index_t* pEdgeIndices = TRI_TABLE[cubeIndex];

                                  CreateVertices(rSolGrid.nodes,
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

    uint32_t MarchingCubesSystem::GetCubeIndex(const float* pIsoValues)
    {
        uint32_t cubeIndex(0);

        for (uint32_t i(0); i < CUBE_VERTEX_COUNT; ++i)
        {
            if (!(pIsoValues[i] < _rMarchingCubesData.isoLevel))
            {
                continue;
            }

            cubeIndex |= 1 << i;
        }

        return cubeIndex;
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

    void MarchingCubesSystem::CreateVertices(Nodes& rNodes, 
                                             const Index_t* pEdgeIndices, 
                                             const float* pIsoValues, 
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
                                                           _rMarchingCubesData.isInterpolated,
                                                           pIsoValues,
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

    void MarchingCubesSystem::CreateVertices(Nodes& rNodes,
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

    glm::vec3 MarchingCubesSystem::GetEdgeVertexPosition(Nodes& rNodes,
                                                         const bool isInterpolated, 
                                                         const float* pIsoValues, 
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

        if (_rMarchingCubesData.isInterpolated)
        {
            const glm::vec3 vertexA
            {
                pXVertices[indexA],
                pYVertices[indexA], 
                pZVertices[indexA] 
            };

            const glm::vec3 vertexB
            {
                pXVertices[indexB],
                pYVertices[indexB],
                pZVertices[indexB] 
            };

            const glm::vec3 vertexDistance = vertexB - vertexA;

            const float interpScalar = CalculateInterpolationScalar(pIsoValues[indexA], 
                                                                    pIsoValues[indexB], 
                                                                    _rMarchingCubesData.isoLevel);

            return vertexA + (vertexDistance * interpScalar);
        }

        const float half = 0.5f;

        return glm::vec3((pXVertices[indexA] + pXVertices[indexB]) * half,
                         (pYVertices[indexA] + pYVertices[indexB]) * half, 
                         (pZVertices[indexA] + pZVertices[indexB]) * half);
    }

    glm::vec3 MarchingCubesSystem::GetEdgeVertexPosition(Nodes& rNodes, 
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