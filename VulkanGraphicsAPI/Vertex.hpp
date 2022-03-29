#pragma once
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SolEngine::Data
{
	struct Vertex
	{
		Vertex(const glm::vec3& pos, 
			   const glm::vec3& col)
			: position(pos),
			  colour(col)
		{}

		static std::vector<VkVertexInputBindingDescription> InputBindingDescriptors()
		{
			std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions(1);

			vertexInputBindingDescriptions.at(0) =
			{
				VkVertexInputBindingDescription
				{
					.binding   = 0,
					.stride    = sizeof(Vertex),
					.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
				}
			};

			return vertexInputBindingDescriptions;
		}

		static std::vector<VkVertexInputAttributeDescription> InputAttributeDescriptions()
		{
			std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions(2);

			vertexInputAttributeDescriptions =
			{
				VkVertexInputAttributeDescription	// Position
				{
					.location = 0,
					.binding  = 0,
					.format   = VK_FORMAT_R32G32B32_SFLOAT,	// X, Y, Z
					.offset   = offsetof(Vertex, position)
				},
				VkVertexInputAttributeDescription	// Colour
				{
					.location = 1,
					.binding  = 0,
					.format   = VK_FORMAT_R32G32B32_SFLOAT,	// R, G, B
					.offset   = offsetof(Vertex, colour)
				}
			};

			return vertexInputAttributeDescriptions;
		}

		glm::vec3 position{ 0, 0, 0 };
		glm::vec3 colour{ 1.f, 1.f, 1.f };
	};
}
