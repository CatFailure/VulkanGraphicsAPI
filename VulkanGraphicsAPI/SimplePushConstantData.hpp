#pragma once
namespace SolEngine::Data
{
	struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.0f };
		glm::vec2 offset;
		alignas(16) glm::vec3 colour;	// TEMP: Needs to be aligned in memory as the shader expects it, as by default it's tightly packed.
	};
}
