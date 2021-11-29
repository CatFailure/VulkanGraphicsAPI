#pragma once
#include "SolVulkanDevice.hpp"

namespace SolEngine
{
    class SolVulkanSwapchain
    {
    public:
        SolVulkanSwapchain(SolVulkanDevice &rDevice, const VkExtent2D &windowExtent);
        SolVulkanSwapchain(SolVulkanDevice &rDevice, const VkExtent2D &windowExtent, std::shared_ptr<SolVulkanSwapchain> pOldSwapchain);
    private:
        void CreateSwapchain();

        VkSurfaceFormatKHR ChooseImageFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        SolVulkanDevice &_rSolDevice;
        VkExtent2D _windowExtent;

        VkSwapchainKHR _vkSwapchain{ NULL };
        std::shared_ptr<SolVulkanSwapchain> _pOldSwapchain;

        std::vector<VkImage> _vkSwapchainImages;
        std::vector<VkImageView> _vkSwapchainImageViews;
    };
}
