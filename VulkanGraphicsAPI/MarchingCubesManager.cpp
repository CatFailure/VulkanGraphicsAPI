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
        DBG_ASSERT_MSG(IsWithinAxisNodeCountLimit(dimensions.x), 
                       "Too many Nodes!");

        DBG_ASSERT_MSG(IsWithinAxisNodeCountLimit(dimensions.y), 
                       "Too many Nodes!");

        DBG_ASSERT_MSG(IsWithinAxisNodeCountLimit(dimensions.z), 
                       "Too many Nodes!");

        _dimensions = dimensions;
        _nodes      = GridNodes(dimensions);

        CalculateIsoValues();
        GenerateIsoSurfaces();
    }

    void MarchingCubesManager::SetDimensions(const glm::uint scalarDimensions)
    {
        SetDimensions(glm::uvec3(scalarDimensions));
    }

    float MarchingCubesManager::GetIsoValueAtCoord(const float x, 
                                                   const float y, 
                                                   const float z) const
    {
        const size_t index = _3DTo1DIndex(x, y, z, 
                                          _dimensions, 
                                          _nodes.step);

        return _nodes.isoValues[index];
    }

    void MarchingCubesManager::GetCubeIsoValuesAtCoord(const float x, 
                                                       const float y, 
                                                       const float z, 
                                                       float *pOutIsoValues,
                                                       Vertex *pOutCubeVertices) const
    {
        // This is definitely not the most efficient way to do this, but it'll do for now
        const float adjX = x + 1;
        const float adjY = y - 1;
        const float adjZ = z + 1;

        pOutCubeVertices[0] = {{x,    y,    z}};
        pOutCubeVertices[1] = {{adjX, y,    z}};
        pOutCubeVertices[2] = {{adjX, y,    adjZ}};
        pOutCubeVertices[3] = {{x,    y,    adjZ}};
        pOutCubeVertices[4] = {{x,    adjY, z}};
        pOutCubeVertices[5] = {{adjX, adjY, z}};
        pOutCubeVertices[6] = {{adjX, adjY, adjZ}};
        pOutCubeVertices[7] = {{x,    adjY, adjZ}};

        pOutIsoValues[0] = GetIsoValueAtCoord(x,    y,    z);
        pOutIsoValues[1] = GetIsoValueAtCoord(adjX, y,    z);
        pOutIsoValues[2] = GetIsoValueAtCoord(adjX, y,    adjZ);
        pOutIsoValues[3] = GetIsoValueAtCoord(x,    y,    adjZ);
        pOutIsoValues[4] = GetIsoValueAtCoord(x,    adjY, z);
        pOutIsoValues[5] = GetIsoValueAtCoord(adjX, adjY, z);
        pOutIsoValues[6] = GetIsoValueAtCoord(adjX, adjY, adjZ);
        pOutIsoValues[7] = GetIsoValueAtCoord(x,    adjY, adjZ);
    }

    void MarchingCubesManager::TraverseGridNodes(const TraverseNodesCallback_t& callback)
    {
        const float step = _nodes.step;

        size_t zIndex(0);
        for (float z = 0; z < _dimensions.z; z += step)
        {
            size_t yIndex(0);
            for (float y = 0; y < _dimensions.y; y += step)
            {
                size_t xIndex(0);
                for (float x = 0; x < _dimensions.x; x += step)
                {
                    callback(_nodes.xPositions[xIndex], 
                             _nodes.yPositions[yIndex],
                             _nodes.zPositions[zIndex]);

                    ++xIndex;
                }

                ++yIndex;
            }

            ++zIndex;
        }
    }

    void MarchingCubesManager::CalculateIsoValues()
    {
        TraverseGridNodes([this](const float x, 
                                 const float y,
                                 const float z) 
                          {
                              const size_t isoValueIndex = _3DTo1DIndex(x, y, z,
                                                                        _dimensions, 
                                                                        _nodes.step);

                              CoordToIsoValue(x, y, z, 
                                              &_nodes.isoValues[isoValueIndex], 
                                              _dimensions);
                          });
    }

    void MarchingCubesManager::Update(const float deltaTime)
    {
    }

    void MarchingCubesManager::GenerateIsoSurfaces()
    {
        TraverseGridNodes([this](const float x, 
                                 const float y,
                                 const float z)
            {
                float cubeIsoValues[CUBE_VERTEX_COUNT];
                Vertex cubeVertices[CUBE_VERTEX_COUNT];

                GetCubeIsoValuesAtCoord(x, y, z, cubeIsoValues, cubeVertices);

                const uint32_t cubeIndex = GetCubeIndex(cubeIsoValues);
                const Index_t *cubeEdgeIndices = TRI_TABLE[cubeIndex];

                for (size_t i = 0; i < TRI_TABLE_INDEX_COUNT; ++i)
                {
                    const Index_t edgeIndex = cubeEdgeIndices[i];
                    const std::pair<Index_t, Index_t> cornerIndices = CornerIndicesFromEdgeIndex(edgeIndex);
                    const glm::vec3 vertexPos = (cubeVertices[cornerIndices.first].position + cubeVertices[cornerIndices.second].position) * .5f;
                }
            });
    }

    void MarchingCubesManager::CreateIsoModel()
    {
    }

    uint32_t MarchingCubesManager::GetCubeIndex(const float *pIsoCubeValues) const
    {
        uint32_t cubeIndex = 0;

        for (uint32_t i = 0; i < CUBE_VERTEX_COUNT; ++i)
        {
            if (!(pIsoCubeValues[i] < _isoLevel))
            {
                continue;
            }

            cubeIndex |= 1 << i;
        }

        return cubeIndex;
    }
}