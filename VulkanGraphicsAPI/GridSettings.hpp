#pragma once
#include <glm/glm.hpp>

#include "ISerializable.hpp"
#include "Helpers.hpp"

using namespace Utility;
using namespace SolEngine::Interface;

namespace SolEngine::Settings
{
	struct GridSettings : public ISerializable
	{
		virtual bool Deserialize(const rapidjson::Value& obj) override
		{
			if (!JsonHasMembers(obj, PROP_DIMENSIONS))
			{
				DBG_ASSERT_MSG(false, "[GridSettings] Object Missing Members!");

				return false;
			}

			const auto dimensionsObj = obj[PROP_DIMENSIONS].GetObj();

			if (!JsonHasMembers(dimensionsObj, 
								PROP_DIMENSIONS_X, 
								PROP_DIMENSIONS_Y,
								PROP_DIMENSIONS_Z))
			{
				DBG_ASSERT_MSG(false, "[GridSettings] Dimensions Object Missing Members!");

				return false;
			}

			dimensions =
			{
				dimensionsObj[PROP_DIMENSIONS_X].GetUint(),
				dimensionsObj[PROP_DIMENSIONS_Y].GetUint(),
				dimensionsObj[PROP_DIMENSIONS_Z].GetUint()
			};

			return true;    // OK!
		}

		void Reset()
		{
			GridSettings defaultGridSettings{};

			dimensions = defaultGridSettings.dimensions;
		}

		size_t GetNodeCount() const 
		{
			return (size_t)((double)dimensions.x * dimensions.y * dimensions.z);
		}

		bool isGridDimensionsChangeRequested{ false };

		glm::uvec3 dimensions{ 30U };	// Adjusts the size of the grid

	private:
		static constexpr const char* PROP_DIMENSIONS  { "dimensions" };
		static constexpr const char* PROP_DIMENSIONS_X{ "x" };
		static constexpr const char* PROP_DIMENSIONS_Y{ "y" };
		static constexpr const char* PROP_DIMENSIONS_Z{ "z" };
	};
}
