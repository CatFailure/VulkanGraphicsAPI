#pragma once
#include "SolModel.hpp"
#include "MatrixUtility.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Math;

namespace SolEngine
{
	struct Transform
	{
		glm::vec3 position{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		glm::mat4 TransformMatrix() const
		{
			return FastTransformMatrix(position, 
									   scale,
									   rotation);
		}
	};

	class SolGameObject
	{
	public:
		typedef uint32_t id_t;

		static SolGameObject CreateGameObject()
		{
			static id_t currentId(0);

			return SolGameObject(currentId++);
		}

		id_t					  GetID()	  const { return _id; }
		std::shared_ptr<SolModel> GetModel()  const { return _pSolVulkanModel; }
		glm::vec3				  GetColour() const { return _colour; }

		void SetModel(std::shared_ptr<SolModel> pModel) { _pSolVulkanModel = pModel; }
		void SetColour(const glm::vec3 &colour)			{ _colour = colour; }

		Transform transform;
	private:
		SolGameObject(const id_t id)
			: _id(id)
		{}

		id_t _id;
		std::shared_ptr<SolModel> _pSolVulkanModel;
		glm::vec3 _colour{};
	};
}
