#pragma once
#include "MarchingCubesHelpers.hpp"
#include "SolGrid.hpp"
#include "SolGameObject.hpp"

using namespace Utility;

namespace SolEngine::System
{
	class MarchingCubesSystem
	{
	public:
		MarchingCubesSystem(SolDevice& rSolDevice, SolGrid& rSolGrid, DiagnosticData& rDiagnosticData);

		const SolGameObject& GetGameObject()			  const { return _marchingCubesObject; }
		size_t				 GetIsoValuesGeneratedCount() const { return _isoValuesGeneratedCount; }

		void March();

	private:
		uint32_t  GetCubeIndex(const bool* pNodeStates);
		void	  GetCubeIsoValues(bool* pOutCubeIsoValues, const bool* pGridCellStates, const uint32_t xIndex, 
								   const uint32_t yIndex, const uint32_t zIndex, const glm::vec3& scaledGridDimensions);

		void	  CreateVertices(Cells& rNodes, const Index_t* pEdgeIndices, const uint32_t xIndex, const uint32_t yIndex, const uint32_t zIndex);
		glm::vec3 GetEdgeVertexPosition(Cells& rNodes, const uint32_t xIndex, const uint32_t yIndex, const uint32_t zIndex,
										const std::pair<Index_t, Index_t>& cornerIndices);
		void UpdateGameObjectModel();

		SolDevice&		_rSolDevice;
		SolGrid&		_rSolGrid;
		DiagnosticData& _rDiagnosticData;
		SolGameObject	_marchingCubesObject;

		size_t				_isoValuesGeneratedCount{ 0 };
		std::vector<Vertex> _vertices;
	};
}