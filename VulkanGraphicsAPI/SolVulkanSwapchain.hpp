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
        void CreateSwapchain(uint32_t *pOutImageCount, VkSurfaceFormatKHR *pOutSurfaceImageFormat, VkExtent2D *pOutSwapchainExtent);
        void CreateSwapchainImages(uint32_t &rImageCount, const VkSurfaceFormatKHR &surfaceImageFormat, const VkExtent2D &swapchainExtent);
        void CreateSwapchainImageViews();

        VkSurfaceFormatKHR ChooseImageFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        SolVulkanDevice &_rSolDevice;
        VkExtent2D _windowExtent;

        VkSwapchainKHR _vkSwapchain{ NULL };
        std::shared_ptr<SolVulkanSwapchain> _pOldSwapchain;

        std::vector<VkImage> _vkSwapchainImages;
        std::vector<VkImageView> _vkSwapchainImageViews;

        VkFormat _vkSwapchainImageFormat;
        VkExtent2D _vkSwapchainImageExtent;
    };
}
