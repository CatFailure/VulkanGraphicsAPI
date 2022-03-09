#pragma once
#include "DebugHelpers.hpp"
#include "GridHelpers.hpp"
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
		typedef std::function<void(const uint32_t, const uint32_t, const uint32_t)> TraverseCubesCallback_t;

		SolGrid(GridSettings& rGridData, DiagnosticData& rDiagnosticData);
		~SolGrid();

		void Initialise();

		bool IsGridDataValid() const { return _isGridDataValid; }

		const glm::uvec3& GetDimensions()	    const { return _rGridSettings.dimensions; }
		float			  GetStep()				const { return _rGridSettings.step; }
		glm::vec3		  GetScaledDimensions() const { return (glm::vec3)GetDimensions() / GetStep(); }

		void TraverseGrid(const TraverseCubesCallback_t &callback);

		Cells cells;

	private:
		void InitialiseNodes();
		void SetBoundsWithDimensions(const glm::uvec3& dimensions);
		bool AreCellLimitsExceeded(const glm::uvec3& dimensions, const float step);
		bool IsMaxCellsPerAxisExceeded(const uint32_t axisSize, const float step);

		GridSettings&   _rGridSettings;
		DiagnosticData& _rDiagnosticData;

		glm::vec3 _minBounds { 0 };
		glm::vec3 _maxBounds { 0 };

		bool _isGridDataValid{ false };
	};
}
