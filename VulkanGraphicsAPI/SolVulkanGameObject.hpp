#pragma once
#include "SolVulkanModel.hpp"

namespace SolEngine
{
	struct Transform2D
	{
		glm::vec2 position{};
		glm::vec2 scale{ 1.f, 1.f };
		float rotation;

		glm::mat2 Mat2() const 
		{
			const float rotSin = glm::sin(rotation);
			const float rotCos = glm::cos(rotation);

			const glm::mat2 rotationMat
			{
				{ rotCos,  rotSin}, 
				{ -rotSin, rotCos}
			};

			const glm::mat2 scaleMat
			{
				{ scale.x, 0 },
				{ 0, scale.y }
			};

			return rotationMat * scaleMat;
		}
	};

	class SolVulkanGameObject
	{
	public:
		typedef uint32_t id_t;

		static SolVulkanGameObject CreateGameObject()
		{
			static id_t currentId(0);

			return SolVulkanGameObject(currentId++);
		}

		id_t							GetID()		const { return _id; }
		std::shared_ptr<SolVulkanModel> GetModel()  const { return _pSolVulkanModel; }
		glm::vec3						GetColour() const { return _colour; }

		void SetModel(std::shared_ptr<SolVulkanModel> pModel) { _pSolVulkanModel = pModel; }
		void SetColour(const glm::vec3 &colour)				  { _colour = colour; }

		Transform2D transform2D;
	private:
		SolVulkanGameObject(const id_t id)
			: _id(id)
		{}

		id_t _id;
		std::shared_ptr<SolVulkanModel> _pSolVulkanModel;
		glm::vec3 _colour;
	};
}
