#pragma once

namespace SolEngine::GUI::Data
{
	struct DiagnosticData
	{
		// Frame time data
		float deltaTimeSeconds;
		float totalTimeSeconds;

		// Memory data
		size_t memoryAllocatedBytes;
		size_t memoryUsedBytes;
		float memoryUsedPercentage;

		// Geometry data
		size_t vertexCount;
		size_t triCount;
	};
}
