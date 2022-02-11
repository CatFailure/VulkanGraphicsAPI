#pragma once
#include "Cubes.hpp"
#include "SolGameObject.hpp"
#include "IMonoBehaviour.hpp"
#include "MarchingCubesHelpers.hpp"
#include "DiagnosticData.hpp"
#include "MarchingCubesData.hpp"

using namespace SolEngine::DOD;
using namespace SolEngine::GUI::Data;
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
        typedef std::function<void(const uint32_t, const uint32_t, const uint32_t)> TraverseCubesCallback_t;

        MarchingCubesManager(SolDevice &rDevice, DiagnosticData &rDiagnosticData, MarchingCubesData &rMarchingCubesData);
        MarchingCubesManager(SolDevice &rDevice, DiagnosticData &rDiagnosticData, MarchingCubesData &rMarchingCubesData, const glm::vec3 &dimensions);
        MarchingCubesManager(SolDevice &rDevice, DiagnosticData &rDiagnosticData, MarchingCubesData &rMarchingCubesData, const int scalarDimensions);

        SolGameObject &GetGameObject() { return _marchingCubesObject; }

        void SetDimensions(const glm::vec3 &dimensions);
        void SetDimensions(const int scalarDimensions);

        std::shared_ptr<SolModel> CreateModel();

        // Inherited via IMonoBehaviour
        virtual void Update(const float deltaTime) override;

    private:
        bool IsWithinMaxCubeCount(const float axisSize) const { return !((uint32_t)(axisSize / _rMarchingCubesData.step) > MAX_CUBES_PER_AXIS_COUNT); }

        uint32_t GenerateIsoValues();
        void March();
        uint32_t GetCubeIndex(const float *pIsoValues);
        void CreateVertices(const Index_t *pEdgeIndices, const float *pIsoValues, const uint32_t xIndex, const uint32_t yIndex, const uint32_t zIndex);
        glm::vec3 GetEdgeVertexPosition(const bool isInterpolated, const float *pIsoValues, const uint32_t xIndex, const uint32_t yIndex, 
                                        const uint32_t zIndex, const std::pair<Index_t, Index_t> &cornerIndices);

        void TraverseAllCubes(const TraverseCubesCallback_t &callback);

        SolDevice &        _rSolDevice;
        DiagnosticData &   _rDiagnosticData;
        MarchingCubesData &_rMarchingCubesData;

        std::unique_ptr<Cubes> _pCubes{ nullptr };
        SolGameObject _marchingCubesObject;

        float _isoLevel      { -2.5f };

        glm::vec3 _dimensions{ 0 };
        glm::vec3 _minBounds { 0 };
        glm::vec3 _maxBounds { 0 };

        std::vector<Vertex> _vertices; // TEMP
    };
}
