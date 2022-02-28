#pragma once

namespace SolEngine::GUI::Data
{
	struct DiagnosticData
	{
		float MemoryUsedPercentage() const 
		{ 
			const float returnVal = ((float)memoryUsedBytes / memoryAllocatedBytes) * 100.f; 

			return returnVal;
		}

		// Frame time data
		float deltaTimeSeconds;
		float totalTimeSeconds;

		// Memory data
		size_t memoryAllocatedBytes;
		size_t memoryUsedBytes;

		// Geometry data
		size_t vertexCount;
		size_t triCount;
	};
}
