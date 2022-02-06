#pragma once

namespace SolEngine::GUI::Data
{
	struct DiagnosticData
	{
		// Frame time data
		float deltaTimeSeconds;
		float totalTimeSeconds;

		// Geometry data
		size_t vertexCount;
		size_t triCount;
	};
}
