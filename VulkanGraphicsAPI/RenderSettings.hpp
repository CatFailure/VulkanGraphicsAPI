#pragma once
#include <vulkan/vulkan.hpp>

#include "ISerializable.hpp"
#include "DebugHelpers.hpp"
#include "Helpers.hpp"

using namespace Utility;
using namespace SolEngine::Interface;

namespace SolEngine::Settings
{
    struct RenderSettings : public ISerializable
    {
		virtual bool Deserialize(const rapidjson::Value& obj) override
		{
            if (!JsonHasMembers(obj, 
                                PROP_POLY_MODE,
                                PROP_CULL_MODE))
            {
                DBG_ASSERT_MSG(false, "[RenderSettings] Object Missing Members!");

                return false;
            }

            polygonMode = (VkPolygonMode)obj[PROP_POLY_MODE].GetUint();
            cullMode    = (VkCullModeFlagBits)obj[PROP_CULL_MODE].GetUint();

			return true;    // OK!
		}

        bool               isRendererOutOfDate{ true };
        VkPolygonMode      polygonMode        { VK_POLYGON_MODE_LINE };
        VkCullModeFlagBits cullMode           { VK_CULL_MODE_BACK_BIT };

    private:
        static constexpr const char* PROP_POLY_MODE{ "poly_mode" };
        static constexpr const char* PROP_CULL_MODE{ "cull_mode" };
    };
}
