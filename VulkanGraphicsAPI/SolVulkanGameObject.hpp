#pragma once
#include "SolVulkanModel.hpp"

using namespace SolEngine::Data;

namespace SolEngine
{
	//struct Transform2D
	//{
	//	glm::vec2 position{};
	//	glm::vec2 scale{ 1.f, 1.f };
	//	float rotation;

	//	glm::mat2 Mat2() const 
	//	{
	//		const float rotSin = glm::sin(rotation);
	//		const float rotCos = glm::cos(rotation);

	//		const glm::mat2 rotationMat
	//		{
	//			{ rotCos,  rotSin}, 
	//			{ -rotSin, rotCos}
	//		};

	//		const glm::mat2 scaleMat
	//		{
	//			{ scale.x, 0 },
	//			{ 0, scale.y }
	//		};

	//		return rotationMat * scaleMat;
	//	}
	//};

	struct Transform
	{
		glm::vec3 position{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		glm::mat4 Mat4() const
		{
			glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, 
												 position);

			// Rotate using tait-bryan angles convention: https://commons.wikimedia.org/wiki/Tait-Bryan_angles
			transform = glm::rotate(transform, rotation.y, VECTOR3_AXIS_Y);
			transform = glm::rotate(transform, rotation.x, VECTOR3_AXIS_X);
			transform = glm::rotate(transform, rotation.z, VECTOR3_AXIS_Z);

			transform = glm::scale(transform, scale);
			
			return transform;
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

		Transform transform;
	private:
		SolVulkanGameObject(const id_t id)
			: _id(id)
		{}

		id_t _id;
		std::shared_ptr<SolVulkanModel> _pSolVulkanModel;
		glm::vec3 _colour{};
	};
}
