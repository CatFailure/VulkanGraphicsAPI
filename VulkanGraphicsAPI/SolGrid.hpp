#pragma once
#include "DebugHelpers.hpp"
#include "Helpers.hpp"
#include "Cells.hpp"
#include "GridSettings.hpp"

using namespace Utility;
using namespace SolEngine::DOD;
using namespace SolEngine::Settings;

namespace SolEngine
{
	class SolGrid
	{
	public:
		/// <summary>
		/// Arg0 = X-Index
		/// Arg1 = Y-Index
		/// Arg2 = Z-Index
		/// </summary>
		typedef std::function<void(const uint32_t xIndex, 
								   const uint32_t yIndex, 
								   const uint32_t zIndex)> TraverseCubesCallback_t;

		SolGrid(GridSettings& rGridData, DiagnosticData& rDiagnosticData);
		~SolGrid();

		void Reset();

		bool			  IsGridDataValid() const { return _isGridDataValid; }
		const glm::uvec3& GetDimensions()   const { return _rGridSettings.dimensions; }

		void Initialise();
		void TraverseAllGridCells(const TraverseCubesCallback_t& callback);

		Cells cells;

	private:
		void InitialiseNodes();
		void SetBoundsWithDimensions(const glm::uvec3& dimensions);
		bool AreCellLimitsExceeded(const glm::uvec3& dimensions);
		bool IsMaxCellsPerAxisExceeded(const uint32_t axisSize);

		GridSettings&   _rGridSettings;
		DiagnosticData& _rDiagnosticData;

		glm::ivec3 _minBounds { 0 };
		glm::ivec3 _maxBounds { 0 };

		bool _isGridDataValid{ false };
	};
}
