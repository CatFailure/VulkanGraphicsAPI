#pragma once
#define GLM_FORCE_RADIANS	// Make glm functions expect angles in radians instead of degrees

#include <glm/gtc/matrix_transform.hpp>

namespace SolEngine::Math
{
	/// <summary>
	/// Transform with Position * Ry * Rx * Tz * Scale - using Tait-Bryan angles convention: 
    /// https://commons.wikimedia.org/wiki/Tait-Bryan_angles
	/// Rotations correspond to Tait-Bryan angles of Y(1), X(2), Z(3).
	/// Interpret Right-to-Left: Extrinsic Rotations (Global Axes)
	/// Interpret Left-to-Right: Intrinsic Rotations (Local Axes)
	/// </summary>
	glm::mat4 FastTransformMatrix(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation);
}

