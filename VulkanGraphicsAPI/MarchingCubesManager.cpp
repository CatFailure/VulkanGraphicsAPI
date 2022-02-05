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

        _dimensions = dimensions;
        _cubes      = Cubes(dimensions);

        GenerateIsoValues();
        CalculateCubeIndex();
    }

    void MarchingCubesManager::SetDimensions(const glm::uint scalarDimensions)
    {
        SetDimensions(glm::uvec3(scalarDimensions));
    }

    void MarchingCubesManager::GetCubeVerticesAt(const glm::uvec3 &position, 
                                                 float *pOutXVertices, 
                                                 float *pOutYVertices, 
                                                 float *pOutZVertices) const
    {
        std::copy(std::begin(_cubes.xPositions[position.x]),
                  std::end(_cubes.xPositions[position.x]), 
                  pOutXVertices);

        std::copy(std::begin(_cubes.yPositions[position.y]),
                  std::end(_cubes.yPositions[position.y]), 
                  pOutYVertices);

        std::copy(std::begin(_cubes.zPositions[position.z]),
                  std::end(_cubes.zPositions[position.z]), 
                  pOutZVertices);
    }

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
        TraverseAllCubes([this](const size_t xIndex, 
                                const size_t yIndex, 
                                const size_t zIndex) 
                         {
                             const size_t isoValuesIndex = _3DTo1DIndex(xIndex, 
                                                                        yIndex, 
                                                                        zIndex, 
                                                                        _dimensions);
                         
                             VerticesToIsoValues(_cubes.xPositions[xIndex], 
                                                 _cubes.yPositions[yIndex], 
                                                 _cubes.zPositions[zIndex], 
                                                 _cubes.isoValues[isoValuesIndex]);
                         });
    }

    void MarchingCubesManager::CalculateCubeIndex()
    {
        TraverseAllCubes([this](const size_t xIndex, 
                                const size_t yIndex, 
                                const size_t zIndex) 
                         {
                             const float half = 0.5f;
                             const float *pXVertices = _cubes.xPositions[xIndex];
                             const float *pYVertices = _cubes.yPositions[yIndex];
                             const float *pZVertices = _cubes.zPositions[zIndex];
                             const size_t isoValuesIndex = _3DTo1DIndex(xIndex, 
                                                                        yIndex, 
                                                                        zIndex, 
                                                                        _dimensions);

                             const float *isoValues = _cubes.isoValues[isoValuesIndex];

                             uint32_t cubeIndex(0);
                             for (uint32_t i(0); i < CUBE_VERTEX_COUNT; ++i)
                             {
                                 if (!(isoValues[i] < _isoLevel))
                                 {
                                     continue;
                                 }

                                 cubeIndex |= 1 << i;
                             }

                             // Loop up the triangulation for the cubeIndex
                             const Index_t *pEdgeIndices = TRI_TABLE[cubeIndex];

                             for (uint32_t i(0); i < TRI_TABLE_INDEX_COUNT; ++i)
                             {
                                 if (pEdgeIndices[i] == -1)
                                 {
                                     return;
                                 }

                                 // Find edge midpoint
                                 const std::pair<Index_t, Index_t> cornerIndices = 
                                     CornerIndicesFromEdgeIndex(pEdgeIndices[i]);

                                 glm::vec3 midpoint;

                                 if (_isInterpolated)
                                 {
                                     const glm::vec3 vertexA{ pXVertices[cornerIndices.first], pYVertices[cornerIndices.first], pZVertices[cornerIndices.first] };
                                     const glm::vec3 vertexB{ pXVertices[cornerIndices.second], pYVertices[cornerIndices.second], pZVertices[cornerIndices.second] };
                                     const glm::vec3 vertexDistance = vertexB - vertexA;

                                     const float interpScalar = CalculateInterpolationScalar(isoValues[cornerIndices.first], 
                                                                                             isoValues[cornerIndices.second], 
                                                                                             _isoLevel);

                                     midpoint = vertexA + (vertexDistance * interpScalar);
                                 }
                                 else
                                 {
                                     midpoint = glm::vec3((pXVertices[cornerIndices.first] + pXVertices[cornerIndices.second]) * half,
                                                          (pYVertices[cornerIndices.first] + pYVertices[cornerIndices.second]) * half, 
                                                          (pZVertices[cornerIndices.first] + pZVertices[cornerIndices.second]) * half);
                                 }

                                 _vertices.push_back({ midpoint });
                             }
                         });
    }

    void MarchingCubesManager::TraverseAllCubes(const TraverseCubesCallback_t &callback)
    {
        const float step = _cubes.step;

        // We have to index this way to account for resolution (step)
        size_t zIndex(0);
        for (float z(0); z < _dimensions.z; z += step)
        {
            size_t yIndex(0);
            for (float y(0); y < _dimensions.y; y += step)
            {
                size_t xIndex(0);
                for (float x(0); x < _dimensions.x; x += step)
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