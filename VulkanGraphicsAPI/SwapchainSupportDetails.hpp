#pragma once

namespace SolEngine::Data
{
    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR        surfaceCapabilities;
        std::vector<VkSurfaceFormatKHR> imageFormats;
        std::vector<VkPresentModeKHR>   presentModes;
    };
}
