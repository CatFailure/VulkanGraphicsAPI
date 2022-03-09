#pragma once
#include <glm/glm.hpp>

namespace SolEngine::Settings
{
	struct GridSettings
	{
		size_t GetNodeCount() const 
		{
			const glm::vec3 scaledDimensions = (glm::vec3)dimensions / step;

			return (size_t)((double)scaledDimensions.x * scaledDimensions.y * scaledDimensions.z);
		}

		glm::uvec3 dimensions{ 10 };	// Adjusts the size of the grid
		float	   step		 { 1.f };	// Adjusts the resolution of the grid
	};
}
