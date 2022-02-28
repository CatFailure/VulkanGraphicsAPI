#pragma once
#include "DebugHelpers.hpp"
#include "GridHelpers.hpp"
#include "Nodes.hpp"

using namespace Utility;
using namespace SolEngine::DOD;

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

		SolGrid(const glm::vec3& dimensions, DiagnosticData& rDiagnosticData);
		~SolGrid();

		size_t GetNodeCount() const;
		glm::vec3 GetDimensions() const { return _dimensions; }
		float GetStep() const { return _step; }

		SolGrid& SetDimensions(const glm::uvec3& dimensions, const float step);

		void TraverseGrid(const TraverseCubesCallback_t &callback);

		Nodes nodes;

	private:
		void InitialiseNodes();
		void SetBoundsWithDimensions(const glm::uvec3& dimensions);
		bool AreDimensionsWithinMaxCubeLimits(const glm::uvec3& dimensions, const float step);
		bool IsAxisSizeWithinMaxCubeLimit(const uint32_t axisSize, const float step);

		DiagnosticData &_rDiagnosticData;
		float _step{ .5f };	// TEMP

		glm::uvec3 _dimensions{ 0 };
		glm::vec3  _minBounds { 0 };
		glm::vec3  _maxBounds { 0 };
	};
}
