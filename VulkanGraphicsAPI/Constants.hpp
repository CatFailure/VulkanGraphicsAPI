#pragma once
#include <glm/glm.hpp>

namespace SolEngine::Data
{
	static constexpr glm::vec3 VEC3_AXIS_X{ 1.f, 0.f, 0.f };
	static constexpr glm::vec3 VEC3_AXIS_Y{ 0.f, -1.f, 0.f };
	static constexpr glm::vec3 VEC3_AXIS_Z{ 0.f, 0.f, 1.f };

	static constexpr size_t MAX_GRID_NODES{ 1 << 24 };
}