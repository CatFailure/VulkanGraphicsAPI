#pragma once
#include "SolGrid.hpp"
#include "SolGameObject.hpp"

namespace SolEngine::System
{
	class MarchingCubesSystem
	{
	public:
		MarchingCubesSystem(SolDevice& rSolDevice, SolGrid& rSolGrid, DiagnosticData& rDiagnosticData);

		SolGameObject& GetGameObject()					  { return _marchingCubesObject; }
		size_t		   GetIsoValuesGeneratedCount() const { return _isoValuesGeneratedCount; }

		void March();

	private:
		uint32_t  GetCubeIndex(const bool* pNodeStates);
		void	  GetCubeIsoValues(bool* pOutCubeIsoValues, const bool* pGridCellStates, 
								   const int xIndex, const int yIndex, const int zIndex, 
								   const glm::vec3& scaledGridDimensions);

		void	  CreateVertices(Cells& rNodes, const Index_t* pEdgeIndices, const int xIndex, const int yIndex, const int zIndex);
		glm::vec3 GetEdgeVertexPosition(Cells& rNodes, const int xIndex, const int yIndex, const int zIndex,
										const std::pair<Index_t, Index_t>& cornerIndices);
		void UpdateGameObjectModel();

		SolDevice&		_rSolDevice;
		SolGrid&		_rSolGrid;
		DiagnosticData& _rDiagnosticData;
		SolGameObject	_marchingCubesObject;

		size_t				_isoValuesGeneratedCount{ 0 };
		size_t				_verticesUsedCount		{ 0U };
		std::vector<Vertex> _vertices				{};

		size_t _vertexMemoryAllocatedBytes{ 0U };
		size_t _vertexMemoryUsedBytes	  { 0U };
	};
}