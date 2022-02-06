#include "pch.hpp"
#include "MarchingCubesManager.hpp"

namespace SolEngine::Manager
{
    MarchingCubesManager::MarchingCubesManager(SolDevice &rDevice)
        : _rSolDevice(rDevice)
    {}

    MarchingCubesManager::MarchingCubesManager(SolDevice &rDevice, 
                                               const glm::uvec3 &dimensions)
        : _rSolDevice(rDevice)
    {
        SetDimensions(dimensions);
    }

    MarchingCubesManager::MarchingCubesManager(SolDevice &rDevice, 
                                               const glm::uint scalarDimensions)
        : _rSolDevice(rDevice)
    {
        SetDimensions(scalarDimensions);
    }

    void MarchingCubesManager::SetDimensions(const glm::uvec3 &dimensions)
    {
        DBG_ASSERT_MSG(IsWithinMaxCubeCount(dimensions.x), 
                       "Too many Cubes!");

        DBG_ASSERT_MSG(IsWithinMaxCubeCount(dimensions.y), 
                       "Too many Cubes!");

        DBG_ASSERT_MSG(IsWithinMaxCubeCount(dimensions.z), 
                       "Too many Cubes!");

        DimensionsToBounds(dimensions, 
                           &_minBounds, 
                           &_maxBounds);

        _dimensions = dimensions;
        _pCubes      = std::make_unique<Cubes>(_minBounds, _maxBounds);

        GenerateIsoValues();
        March();
    }

    void MarchingCubesManager::SetDimensions(const glm::uint scalarDimensions)
    {
        SetDimensions(glm::uvec3(scalarDimensions));
    }

    //void MarchingCubesManager::GetCubeVerticesAt(const glm::uvec3 &position, 
    //                                             float *pOutXVertices, 
    //                                             float *pOutYVertices, 
    //                                             float *pOutZVertices) const
    //{
    //    auto test = *_pCubes->ppXPositions[position.x];

    //    memcpy_s(pOutXVertices, CUBE_VERTEX_COUNT, _pCubes->ppXPositions[position.x], CUBE_VERTEX_COUNT);

    //    //std::copy(std::begin(_cubes.ppHeapXPositions[position.x]),
    //    //          std::end(_cubes.pXPositions[position.x]), 
    //    //          *pOutXVertices);

    //    //std::copy(std::begin(_cubes.pYPositions[position.y]),
    //    //          std::end(_cubes.pYPositions[position.y]), 
    //    //          pOutYVertices);

    //    //std::copy(std::begin(_cubes.pZPositions[position.z]),
    //    //          std::end(_cubes.pZPositions[position.z]), 
    //    //          pOutZVertices);
    //}

    std::shared_ptr<SolModel> MarchingCubesManager::CreateModel()
    {
        return std::make_shared<SolModel>(_rSolDevice, 
                                          _vertices.data(), 
                                          (uint32_t)_vertices.size());
    }

    void MarchingCubesManager::Update(const float deltaTime)
    {

    }

    void MarchingCubesManager::GenerateIsoValues()
    {
        uint32_t isoValuesGeneratedCount(0);
        TraverseAllCubes([this, &isoValuesGeneratedCount]
                         (const size_t xIndex, 
                          const size_t yIndex, 
                          const size_t zIndex) 
                         {
                             const size_t isoValuesIndex = _3DTo1DIndex(xIndex, 
                                                                        yIndex, 
                                                                        zIndex, 
                                                                        _dimensions);
                         
                             VerticesToIsoValues(_pCubes->ppXPositions[xIndex], 
                                                 _pCubes->ppYPositions[yIndex], 
                                                 _pCubes->ppZPositions[zIndex], 
                                                 _pCubes->ppIsoValues[isoValuesIndex]);

                             ++isoValuesGeneratedCount;
                         });

        printf_s("Generated: %u Iso Values\n", 
                 isoValuesGeneratedCount);
    }

    void MarchingCubesManager::March()
    {
        TraverseAllCubes([this](const size_t xIndex, 
                                const size_t yIndex, 
                                const size_t zIndex) 
                         {
                             const size_t isoValuesIndex = _3DTo1DIndex(xIndex, 
                                                                        yIndex, 
                                                                        zIndex, 
                                                                        _dimensions);

                             const float *pIsoValues = _pCubes->ppIsoValues[isoValuesIndex];

                             // Calculate the cube index to pull from the Tri-table
                             const uint32_t cubeIndex = GetCubeIndex(pIsoValues);

                             // Look up the triangulation for the cubeIndex
                             const Index_t *pEdgeIndices = TRI_TABLE[cubeIndex];

                             CreateVertices(pEdgeIndices, 
                                            pIsoValues, 
                                            xIndex, 
                                            yIndex, 
                                            zIndex);
                         });
    }

    uint32_t MarchingCubesManager::GetCubeIndex(const float *pIsoValues)
    {
        uint32_t cubeIndex(0);

        for (uint32_t i(0); i < CUBE_VERTEX_COUNT; ++i)
        {
            if (!(pIsoValues[i] < _isoLevel))
            {
                continue;
            }

            cubeIndex |= 1 << i;
        }

        return cubeIndex;
    }

    void MarchingCubesManager::CreateVertices(const Index_t *pEdgeIndices, 
                                              const float *pIsoValues, 
                                              const size_t xIndex,
                                              const size_t yIndex,
                                              const size_t zIndex)
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
            const glm::vec3 vertex = GetEdgeVertexPosition(_isInterpolated,
                                                           pIsoValues,
                                                           xIndex,
                                                           yIndex,
                                                           zIndex, 
                                                           cornerIndices);

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

    glm::vec3 MarchingCubesManager::GetEdgeVertexPosition(const bool isInterpolated, 
                                                          const float *pIsoValues, 
                                                          const size_t xIndex, 
                                                          const size_t yIndex, 
                                                          const size_t zIndex, 
                                                          const std::pair<Index_t, Index_t> &cornerIndices)
    {
        const float *pXVertices = _pCubes->ppXPositions[xIndex];
        const float *pYVertices = _pCubes->ppYPositions[yIndex];
        const float *pZVertices = _pCubes->ppZPositions[zIndex];
        const Index_t indexA    = cornerIndices.first;
        const Index_t indexB    = cornerIndices.second;

        if (_isInterpolated)
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
                                                                    _isoLevel);

            return vertexA + (vertexDistance * interpScalar);
        }

        const float half = 0.5f;

        return glm::vec3((pXVertices[indexA] + pXVertices[indexB]) * half,
                         (pYVertices[indexA] + pYVertices[indexB]) * half, 
                         (pZVertices[indexA] + pZVertices[indexB]) * half);
    }

    void MarchingCubesManager::TraverseAllCubes(const TraverseCubesCallback_t &callback)
    {
        // We have to index this way to account for resolution (step)
        size_t zIndex(0);
        for (float z(_minBounds.z); z < _maxBounds.z; z += Cubes::STEP)
        {
            size_t yIndex(0);
            for (float y(_minBounds.y); y > _maxBounds.y; y -= Cubes::STEP)
            {
                size_t xIndex(0);
                for (float x(_minBounds.x); x < _maxBounds.x; x += Cubes::STEP)
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
}