#pragma once
#include <glm/glm.hpp>

namespace SolEngine::GUI::Data
{
	struct GridData
	{
		glm::uint GetGridVolume() const { return dimensions.x * dimensions.y * dimensions.z; }
		size_t	  GetNodeCount()  const { return (size_t)(GetGridVolume() * (double)step); }

		glm::uvec3 dimensions{ 10 };	// Adjusts the size of the grid
		float	   step		 { 1.f };	// Adjusts the resolution of the grid
	};
}
