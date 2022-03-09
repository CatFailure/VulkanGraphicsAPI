#pragma once

namespace SolEngine::Data
{
	struct DiagnosticData
	{
		float MemoryUsedPercentage() const 
		{ 
			const float returnVal = ((float)memoryUsedBytes / memoryAllocatedBytes) * 100.f; 

			return returnVal;
		}

		// Frame time data
		float deltaTimeSeconds{ 0.f };
		float totalTimeSeconds{ 0.f };

		// Memory data
		size_t memoryAllocatedBytes{ 0U };
		size_t memoryUsedBytes	   { 0U };

		// Geometry data
		size_t vertexCount{ 0U };
		size_t triCount   { 0U };
	};
}
