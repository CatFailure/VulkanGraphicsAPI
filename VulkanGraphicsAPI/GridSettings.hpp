#pragma once
#include <glm/glm.hpp>

namespace SolEngine::Settings
{
	struct GridSettings
	{
		void Reset()
		{
			GridSettings defaultGridSettings{};

			dimensions = defaultGridSettings.dimensions;
		}

		size_t GetNodeCount() const 
		{
			return (size_t)((double)dimensions.x * dimensions.y * dimensions.z);
		}

		bool isGridDimensionsChangeRequested{ false };

		glm::uvec3 dimensions{ 30U };	// Adjusts the size of the grid
	};
}
