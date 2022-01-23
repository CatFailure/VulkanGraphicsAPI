#pragma once
namespace SolEngine::Data
{
	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.0f };
		alignas(16) glm::vec3 colour;
	};
}
