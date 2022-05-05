#pragma once

namespace SolEngine::Data
{
	struct DiagnosticData
	{
		size_t GetTotalMemoryAllocatedBytes() const { return gridMemoryAllocatedBytes + verticesMemoryAllocatedBytes; }
		size_t GetTotalMemoryUsedBytes()	  const { return gridMemoryUsedBytes + verticesMemoryUsedBytes; }
		float  GetMemoryUsedPercentage()      const { return ((float)GetTotalMemoryUsedBytes() / GetTotalMemoryAllocatedBytes()) * 100.f; }

		// Frame time data
		float deltaTimeSeconds{ 0.f };
		float totalTimeSeconds{ 0.f };

		// Memory data
		size_t gridMemoryAllocatedBytes	   { 0U };
		size_t gridMemoryUsedBytes		   { 0U };

		size_t verticesMemoryAllocatedBytes{ 0U };
		size_t verticesMemoryUsedBytes	   { 0U };

		// Geometry data
		size_t vertexCount{ 0U };
		size_t triCount   { 0U };
	};
}
