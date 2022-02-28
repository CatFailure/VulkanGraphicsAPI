#pragma once
#include "DebugHelpers.hpp"
#include "GridHelpers.hpp"
#include "Nodes.hpp"
#include "IDisposable.hpp"

using namespace Utility;
using namespace SolEngine::DOD;
using namespace SolEngine::Interface;

namespace SolEngine
{
	class SolGrid
	{
	public:
		SolGrid(const glm::vec3& dimensions, DiagnosticData& rDiagnosticData);
		~SolGrid();

		SolGrid& SetDimensions(const glm::uvec3& dimensions, const float step);

	private:
		void InitialiseNodes();
		void SetBoundsWithDimensions(const glm::uvec3& dimensions);
		bool AreDimensionsWithinMaxCubeLimits(const glm::uvec3& dimensions, const float step);
		bool IsAxisSizeWithinMaxCubeLimit(const uint32_t axisSize, const float step);

		DiagnosticData &_rDiagnosticData;
		float _step{ 1.f };	// TEMP

		glm::uvec3 _dimensions{ 0 };
		glm::vec3  _minBounds { 0 };
		glm::vec3  _maxBounds { 0 };

		Nodes _nodes;
	};
}
