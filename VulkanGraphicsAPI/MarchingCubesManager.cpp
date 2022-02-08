#include "pch.hpp"
#include "MarchingCubesManager.hpp"

namespace SolEngine::Manager
{
    MarchingCubesManager::MarchingCubesManager(SolDevice &rDevice)
        : _rSolDevice(rDevice)
    {}

    MarchingCubesManager::MarchingCubesManager(SolDevice &rDevice, 
                                               const glm::vec3 &dimensions)
        : _rSolDevice(rDevice)
    {
        SetDimensions(dimensions);
    }

    MarchingCubesManager::MarchingCubesManager(SolDevice &rDevice, 
                                               const int scalarDimensions)
        : _rSolDevice(rDevice)
    {
        SetDimensions(scalarDimensions);
    }

    void MarchingCubesManager::SetDimensions(const glm::vec3 &dimensions)
    {
        DBG_ASSERT_MSG(IsWithinMaxCubeCount(dimensions.x), 
                       "Too many Cubes!");

        DBG_ASSERT_MSG(IsWithinMaxCubeCount(dimensions.y), 
                       "Too many Cubes!");

        DBG_ASSERT_MSG(IsWithinMaxCubeCount(dimensions.z), 
                       "Too many Cubes!");

        _dimensions = dimensions;

        DimensionsToBounds(dimensions, 
                           &_minBounds, 
                           &_maxBounds);

        GenerateVertices<Axis::X>(_cubes.pAllXVertices, _minBounds.x, _maxBounds.x, Cubes::STEP);
        GenerateVertices<Axis::Y>(_cubes.pAllYVertices, _minBounds.y, _maxBounds.y, Cubes::STEP);
        GenerateVertices<Axis::Z>(_cubes.pAllZVertices, _minBounds.z, _maxBounds.z, Cubes::STEP);

        GenerateIsoValues();
        March();
    }

    void MarchingCubesManager::SetDimensions(const int scalarDimensions)
    {
        SetDimensions(glm::uvec3(scalarDimensions));
    }

    std::shared_ptr<SolModel> MarchingCubesManager::CreateModel()
    {
        if (_vertices.empty())
        {
            return nullptr;
        }

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
                         (const uint32_t xIndex, 
                          const uint32_t yIndex, 
                          const uint32_t zIndex) 
                         {
                             const uint32_t isoValuesIndex = _3DTo1DIndex(xIndex, 
                                                                          yIndex, 
                                                                          zIndex, 
                                                                          _dimensions,
                                                                          Cubes::STEP);

                             // Grab vertices
                             const float *pXVertices = &_cubes.pAllXVertices[xIndex * CUBE_VERTEX_COUNT];
                             const float *pYVertices = &_cubes.pAllYVertices[yIndex * CUBE_VERTEX_COUNT];
                             const float *pZVertices = &_cubes.pAllZVertices[zIndex * CUBE_VERTEX_COUNT];
                             float       *pIsoValues = &_cubes.pAllIsoValues[isoValuesIndex * CUBE_VERTEX_COUNT];

                             VerticesToIsoValues(pXVertices, 
                                                 pYVertices, 
                                                 pZVertices, 
                                                 pIsoValues);

                             ++isoValuesGeneratedCount;
                         });

        printf_s("Generated: %u Iso Values\nMin value: %f\nMaxValue: %f\n", 
                 isoValuesGeneratedCount, 
                 MinIsoValueGenerated, 
                 MaxIsoValueGenerated);
    }

    void MarchingCubesManager::March()
    {
        TraverseAllCubes([this](const uint32_t xIndex, 
                                const uint32_t yIndex, 
                                const uint32_t zIndex) 
                         {
                             const uint32_t isoValuesIndex = _3DTo1DIndex(xIndex, 
                                                                          yIndex, 
                                                                          zIndex, 
                                                                          _dimensions,
                                                                          Cubes::STEP);

                             const float *pIsoValues = &_cubes.pAllIsoValues[isoValuesIndex * CUBE_VERTEX_COUNT];

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
                                                          const uint32_t xIndex, 
                                                          const uint32_t yIndex, 
                                                          const uint32_t zIndex, 
                                                          const std::pair<Index_t, Index_t> &cornerIndices)
    {
        const uint32_t xRowWidth   = xIndex * CUBE_VERTEX_COUNT;
        const uint32_t yRowWidth   = yIndex * CUBE_VERTEX_COUNT;
        const uint32_t zRowWidth   = zIndex * CUBE_VERTEX_COUNT;

        const float *pXVertices = &_cubes.pAllXVertices[xRowWidth];
        const float *pYVertices = &_cubes.pAllYVertices[yRowWidth];
        const float *pZVertices = &_cubes.pAllZVertices[zRowWidth];

        const Index_t indexA = cornerIndices.first;
        const Index_t indexB = cornerIndices.second;

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
        uint32_t zIndex(0);
        for (float z(_minBounds.z); z < _maxBounds.z; z += Cubes::STEP)
        {
            uint32_t yIndex(0);
            for (float y(_minBounds.y); y > _maxBounds.y; y -= Cubes::STEP)
            {
                uint32_t xIndex(0);
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