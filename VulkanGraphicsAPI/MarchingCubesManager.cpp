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