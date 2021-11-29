#include "pch.hpp"
#include "SolVulkanSwapchain.hpp"

namespace SolEngine
{
    SolVulkanSwapchain::SolVulkanSwapchain(SolVulkanDevice &rDevice,
                                           const VkExtent2D &windowExtent)
        : SolVulkanSwapchain(rDevice, windowExtent, nullptr)
    {}

    SolVulkanSwapchain::SolVulkanSwapchain(SolVulkanDevice &rDevice, 
                                           const VkExtent2D &windowExtent, 
                                           std::shared_ptr<SolVulkanSwapchain> pOldSwapchain)
        : _rSolDevice(rDevice),
          _windowExtent(windowExtent),
          _pOldSwapchain(pOldSwapchain)
    {
        CreateSwapchain();

        // Old Swapchain no longer needed
        _pOldSwapchain = nullptr;
    }

    void SolVulkanSwapchain::CreateSwapchain()
    {
        SwapchainSupportDetails swapchainSupportDetails = _rSolDevice.QueryPhysicalDeviceSwapchainSupport();
        QueueFamilyIndices      queueFamilies             = _rSolDevice.QueryPhysicalDeviceQueueFamilies();
        std::vector<uint32_t> queueFamilyIndices{ queueFamilies.graphicsFamily, queueFamilies.presentFamily };

        VkSurfaceFormatKHR      surfaceImageFormat      = ChooseImageFormat(swapchainSupportDetails.imageFormats);
        VkPresentModeKHR        presentMode             = ChoosePresentMode(swapchainSupportDetails.presentModes);
        VkExtent2D              imageExtent             = ChooseExtent(swapchainSupportDetails.surfaceCapabilities);    // Retrieve and use the actual back/front buffer widths
        uint32_t                imageCount              = swapchainSupportDetails.surfaceCapabilities.minImageCount + 1;

        if (swapchainSupportDetails.surfaceCapabilities.maxImageCount > 0 &&
            imageCount > swapchainSupportDetails.surfaceCapabilities.maxImageCount)
        {
            imageCount = swapchainSupportDetails.surfaceCapabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo
        {
            .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface          = _rSolDevice.Surface(),
            .minImageCount    = imageCount,
            .imageFormat      = surfaceImageFormat.format,
            .imageColorSpace  = surfaceImageFormat.colorSpace,
            .imageExtent      = imageExtent,
            .imageArrayLayers = 1,
            .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform     = swapchainSupportDetails.surfaceCapabilities.currentTransform,
            .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode      = presentMode,
            .clipped          = VK_TRUE,    // Clipping outside of extents?
            .oldSwapchain     = _pOldSwapchain == nullptr ?                         // Provide old swapchain if possible
                                    VK_NULL_HANDLE : _pOldSwapchain->_vkSwapchain
        };

        if (queueFamilies.AreFamiliesEqual())
        {
            swapchainCreateInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0;      // Optional
            swapchainCreateInfo.pQueueFamilyIndices   = NULL;   // Optional
        }
        else 
        {
            swapchainCreateInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();  // Optional
            swapchainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices.data();  // Optional
        }

        VkResult result = vkCreateSwapchainKHR(_rSolDevice.Device(),
                                               &swapchainCreateInfo,
                                               NULL,
                                               &_vkSwapchain);

        // Was this successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Swapchain.");
    }

    VkSurfaceFormatKHR SolVulkanSwapchain::ChooseImageFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
        for (const VkSurfaceFormatKHR &availableFormat : availableFormats)
        {
            if (availableFormat.format != VK_FORMAT_B8G8R8A8_SRGB ||
                availableFormat.colorSpace != VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                continue;
            }

            return availableFormat;
        }

        return availableFormats.at(0);
    }

    VkPresentModeKHR SolVulkanSwapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
    {
        // Mailbox
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
            {
                std::cout << "Present mode: Mailbox" << std::endl;
            
                return availablePresentMode;
            }
        }

        // No V-Sync
        for (const auto &availablePresentMode : availablePresentModes) 
        {
            if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) 
            {
                std::cout << "Present mode: Immediate" << std::endl;

                return availablePresentMode;
            }
        }

        std::cout << "Present mode: V-Sync" << std::endl;

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SolVulkanSwapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        std::numeric_limits<uint32_t>::max();

        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }

        VkExtent2D actualExtent = _windowExtent;

        actualExtent.width = std::max(capabilities.minImageExtent.width,
                                      std::min(capabilities.maxImageExtent.width, actualExtent.width));

        actualExtent.height = std::max(capabilities.minImageExtent.height,
                                       std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
};