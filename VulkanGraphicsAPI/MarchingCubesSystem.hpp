#pragma once
#include "MarchingCubesHelpers.hpp"
#include "MarchingCubesData.hpp"
#include "SolGrid.hpp"
#include "SolGameObject.hpp"

using namespace Utility;
using namespace SolEngine::GUI::Data;

namespace SolEngine::System
{
	class MarchingCubesSystem
	{
	public:
		MarchingCubesSystem(MarchingCubesData& rMarchingCubesData, SolDevice& rSolDevice);

		const SolGameObject& GetGameObject() const { return _marchingCubesObject; }
		size_t GetIsoValuesGeneratedCount() const { return _isoValuesGeneratedCount; }

		void GenerateIsoValues(SolGrid& rSolGrid, DiagnosticData& rDiagnosticData);
		void March(SolGrid& rSolGrid, DiagnosticData& rDiagnosticData);

	private:
		uint32_t GetCubeIndex(const float* pIsoValues);
		void CreateVertices(Nodes& rNodes, const Index_t* pEdgeIndices, const float* pIsoValues, const uint32_t xIndex, const uint32_t yIndex, const uint32_t zIndex);
		glm::vec3 GetEdgeVertexPosition(Nodes& rNodes, const bool isInterpolated, const float* pIsoValues, const uint32_t xIndex, const uint32_t yIndex, 
										const uint32_t zIndex, const std::pair<Index_t, Index_t>& cornerIndices);
		void UpdateGameObjectModel();

		SolDevice&			_rSolDevice;
		MarchingCubesData&  _rMarchingCubesData;
		SolGameObject       _marchingCubesObject;

		size_t				_isoValuesGeneratedCount{ 0 };
		std::vector<Vertex> _vertices;
	};
}