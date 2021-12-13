#pragma once
#include "SolVulkanModel.hpp"

namespace SolEngine
{
	struct Transform
	{
		glm::vec2 position{};
		glm::mat2 Mat2() const { return glm::mat2{ 1.0f }; }
	};

	class SolVulkanGameObject
	{
	public:

		std::shared_ptr<SolVulkanModel> GetModel() const { return _pSolVulkanModel; }
		glm::vec3 GetColour() const { return _colour; }

		void SetModel(std::shared_ptr<SolVulkanModel> pModel) { _pSolVulkanModel = pModel; }
		void SetColour(const glm::vec3 &colour) { _colour = colour; }

		Transform _transform;
	private:
		std::shared_ptr<SolVulkanModel> _pSolVulkanModel;
		glm::vec3 _colour;
	};
}
