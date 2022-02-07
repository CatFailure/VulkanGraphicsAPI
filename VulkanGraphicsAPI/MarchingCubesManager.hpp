#pragma once
#include "Cubes.hpp"
#include "SolDevice.hpp"
#include "SolModel.hpp"

using namespace SolEngine::DOD;
using namespace SolEngine::Interface;
using namespace Utility;

/*
* Instead of looping through all the grid nodes THEN generating the isoSurfaces.
* Create a "cube" at a node to generate the isoSurface, draw it, then move onto the next node.
*/

namespace SolEngine::Manager
{
    class MarchingCubesManager : public IMonoBehaviour
    {
    public:
        /// <summary>
        /// Arg0 = X-Index
        /// Arg1 = Y-Index
        /// Arg2 = Z-Index
        /// </summary>
        typedef std::function<void(const size_t, const size_t, const size_t)> TraverseCubesCallback_t;

        MarchingCubesManager(SolDevice &rDevice);
        MarchingCubesManager(SolDevice &rDevice, const glm::uvec3 &dimensions);
        MarchingCubesManager(SolDevice &rDevice, const glm::uint scalarDimensions);

        void SetDimensions(const glm::uvec3 &dimensions);
        void SetDimensions(const glm::uint scalarDimensions);

        std::shared_ptr<SolModel> CreateModel();

        // Inherited via IMonoBehaviour
        virtual void Update(const float deltaTime) override;

    private:
        bool IsWithinMaxCubeCount(const size_t count) const { return !((count / Cubes::STEP) > MAX_CUBES_COUNT); }

        void GenerateIsoValues();
        void March();
        uint32_t GetCubeIndex(const float *pIsoValues);
        void CreateVertices(const Index_t *pEdgeIndices, const float *pIsoValues, const size_t xIndex, const size_t yIndex, const size_t zIndex);
        glm::vec3 GetEdgeVertexPosition(const bool isInterpolated, const float *pIsoValues, const size_t xIndex, const size_t yIndex, 
                                        const size_t zIndex, const std::pair<Index_t, Index_t> &cornerIndices);

        void TraverseAllCubes(const TraverseCubesCallback_t &callback);

        SolDevice &_rSolDevice;

        float _isoLevel      { 2.f };
        bool  _isInterpolated{ true };

        glm::uvec3             _dimensions{ 0 };
        glm::vec3              _minBounds { 0 };
        glm::vec3              _maxBounds { 0 };

        std::unique_ptr<Cubes> _pCubes{ nullptr };
        std::vector<Vertex>    _vertices;  // TEMP
    };
}
