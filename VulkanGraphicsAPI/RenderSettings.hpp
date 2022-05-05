#pragma once
#include <vulkan/vulkan.hpp>

namespace SolEngine::Settings
{
    struct RenderSettings
    {
        bool               isRendererOutOfDate{ true };
        VkPolygonMode      polygonMode        { VK_POLYGON_MODE_LINE };
        VkCullModeFlagBits cullMode           { VK_CULL_MODE_BACK_BIT };
    };
}
