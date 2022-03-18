#pragma once
#include <glm/glm.hpp>

namespace SolEngine::Settings
{
	struct GridSettings
	{
		size_t GetNodeCount() const 
		{
			return (size_t)((double)dimensions.x * dimensions.y * dimensions.z);
		}

		glm::uvec3 dimensions{ 10 };	// Adjusts the size of the grid
	};
}
