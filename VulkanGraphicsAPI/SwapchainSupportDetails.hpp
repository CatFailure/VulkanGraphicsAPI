#pragma once
#include <vulkan/vulkan.hpp>

namespace SolEngine::Data
{
    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        surfaceCapabilities;
        std::vector<VkSurfaceFormatKHR> imageFormats;
        std::vector<VkPresentModeKHR>   presentModes;
    };
}
