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
        uint32_t imageCount;
        VkSurfaceFormatKHR surfaceImageFormat;
        VkExtent2D swapchainExtent;

        CreateSwapchain(&imageCount, &surfaceImageFormat, &swapchainExtent);
        CreateSwapchainImages(imageCount, surfaceImageFormat, swapchainExtent);
        CreateSwapchainImageViews();
        CreateDepthResources();

        // Old Swapchain no longer needed
        _pOldSwapchain = nullptr;
    }

    SolVulkanSwapchain::~SolVulkanSwapchain()
    {
        Dispose();
    }

    void SolVulkanSwapchain::Dispose()
    {
        const VkDevice vkDevice = _rSolDevice.Device();

        // Image Views
        {
            for (const VkImageView &imageView : _vkSwapchainImageViews)
            {
                vkDestroyImageView(vkDevice, imageView, NULL);
            }

            _vkSwapchainImageViews.clear();
        }

        // Swapchain, if possible
        {
            if (_vkSwapchain != nullptr)
            {
                vkDestroySwapchainKHR(vkDevice, _vkSwapchain, NULL);

                _vkSwapchain = nullptr;
            }
        }
        
        // Depth Resources
        {
            for (size_t i(0); i < _vkDepthImages.size(); ++i)
            {
                vkDestroyImageView(vkDevice, _vkDepthImageViews.at(i), NULL);
                vkDestroyImage(vkDevice, _vkDepthImages.at(i), NULL);
                vkFreeMemory(vkDevice, _vkDepthImageMemories.at(i), NULL);
            }

            _vkDepthImageViews.clear();
            _vkDepthImages.clear();
            _vkDepthImageMemories.clear();
        }
    }

    void SolVulkanSwapchain::CreateSwapchain(uint32_t *pOutImageCount,
                                             VkSurfaceFormatKHR *pOutSurfaceImageFormat, 
                                             VkExtent2D *pOutSwapchainExtent)
    {
        const SwapchainSupportDetails swapchainSupportDetails = _rSolDevice.QueryPhysicalDeviceSwapchainSupport();
        QueueFamilyIndices            queueFamilies           = _rSolDevice.QueryPhysicalDeviceQueueFamilies();
        const std::vector<uint32_t>   queueFamilyIndices
        {
            queueFamilies.graphicsFamily, 
            queueFamilies.presentFamily 
        };

        *pOutSurfaceImageFormat = ChooseImageFormat(swapchainSupportDetails.imageFormats);
        *pOutSwapchainExtent    = ChooseExtent(swapchainSupportDetails.surfaceCapabilities);    // Retrieve and use the actual back/front buffer widths
        *pOutImageCount         = swapchainSupportDetails.surfaceCapabilities.minImageCount + 1;

        const VkPresentModeKHR presentMode = ChoosePresentMode(swapchainSupportDetails.presentModes);

        if (swapchainSupportDetails.surfaceCapabilities.maxImageCount > 0 &&
            *pOutImageCount > swapchainSupportDetails.surfaceCapabilities.maxImageCount)
        {
            *pOutImageCount = swapchainSupportDetails.surfaceCapabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo
        {
            .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface          = _rSolDevice.Surface(),
            .minImageCount    = *pOutImageCount,
            .imageFormat      = pOutSurfaceImageFormat->format,
            .imageColorSpace  = pOutSurfaceImageFormat->colorSpace,
            .imageExtent      = *pOutSwapchainExtent,
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
            swapchainCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());  // Optional
            swapchainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices.data();                         // Optional
        }

        const VkResult result = vkCreateSwapchainKHR(_rSolDevice.Device(),
                                                     &swapchainCreateInfo,
                                                     NULL,
                                                     &_vkSwapchain);

        // Was this successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Swapchain.");
    }

    void SolVulkanSwapchain::CreateSwapchainImages(uint32_t &rImageCount, 
                                                   const VkSurfaceFormatKHR &surfaceImageFormat, 
                                                   const VkExtent2D &swapchainExtent)
    {
        // We only specified a minimum number of images in the swapchain, so the implementation is
        // allowed to create a swapchain with more. That's why we'll first query the final number of
        // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
        // retrieve the handles.
        VkResult result = vkGetSwapchainImagesKHR(_rSolDevice.Device(),
                                                  _vkSwapchain,
                                                  &rImageCount,
                                                  NULL);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to retrieve Swapchain Images.");

        _vkSwapchainImages.resize(rImageCount);

        result = vkGetSwapchainImagesKHR(_rSolDevice.Device(),
                                         _vkSwapchain,
                                         &rImageCount,
                                         _vkSwapchainImages.data());

        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Swapchain Images.");

        _vkSwapchainImageFormat = surfaceImageFormat.format;
        _vkSwapchainExtent      = swapchainExtent;
    }

    void SolVulkanSwapchain::CreateSwapchainImageViews()
    {
        const size_t swapchainImageCount = _vkSwapchainImages.size();

        _vkSwapchainImageViews.resize(swapchainImageCount);

        for (size_t i(0); i < swapchainImageCount; ++i)
        {
            const VkImageViewCreateInfo imageViewCreateInfo
            {
                .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image    = _vkSwapchainImages.at(i),
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format   = _vkSwapchainImageFormat,
                .components
                {
                    .r = VK_COMPONENT_SWIZZLE_R,
                    .g = VK_COMPONENT_SWIZZLE_G,
                    .b = VK_COMPONENT_SWIZZLE_B,
                    .a = VK_COMPONENT_SWIZZLE_A
                },
                .subresourceRange
                {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel   = 0,
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                }
            };

            const VkResult result = vkCreateImageView(_rSolDevice.Device(),
                                                      &imageViewCreateInfo, 
                                                      NULL,
                                                      &_vkSwapchainImageViews.at(i));

            DBG_ASSERT_VULKAN_MSG(result, "Failed to create Texture Image View.");
        }
    }

    void SolVulkanSwapchain::CreateDepthResources()
    {
        VkFormat depthFormat = FindDepthFormat();
        size_t swapchainImageCount = SwapchainImageCount();

        _vkDepthImages.resize(swapchainImageCount);
        _vkDepthImageMemories.resize(swapchainImageCount);
        _vkDepthImageViews.resize(swapchainImageCount);

        for (size_t i(0); i < swapchainImageCount; ++i)
        {
            VkImageCreateInfo imageCreateInfo
            {
                .sType     = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .flags     = NULL,
                .imageType = VK_IMAGE_TYPE_2D,
                .format    = depthFormat,
                .extent
                {
                    .width  = _vkSwapchainExtent.width,
                    .height = _vkSwapchainExtent.height,
                    .depth  = 1
                },
                .mipLevels     = 1,
                .arrayLayers   = 1,
                .samples       = VK_SAMPLE_COUNT_1_BIT,
                .tiling        = VK_IMAGE_TILING_OPTIMAL,
                .usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            };

            VkImage& rCurrentDepthImage = _vkDepthImages.at(i);

            _rSolDevice.CreateImageWithInfo(imageCreateInfo, 
                                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                            rCurrentDepthImage,
                                            _vkDepthImageMemories.at(i));

            VkImageViewCreateInfo imageViewCreateInfo
            {
                .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image    = rCurrentDepthImage,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format   = depthFormat,
                .subresourceRange
                {
                    .aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT,
                    .baseMipLevel   = 0,
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                }
            };

            const VkResult result = vkCreateImageView(_rSolDevice.Device(),
                                                      &imageViewCreateInfo, 
                                                      NULL, 
                                                      &_vkDepthImageViews.at(i));

            DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Image View.");
        }
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
        const char *presentModeFormat = "Present mode: %s\n";

        for (const VkPresentModeKHR &availablePresentMode : availablePresentModes)
        {

            switch (availablePresentMode)
            {
            // Mailbox
            case VK_PRESENT_MODE_MAILBOX_KHR:
            {
                DebugHelpers::DPrintf(presentModeFormat, "VK_PRESENT_MODE_MAILBOX_KHR");

                return availablePresentMode;
            }
            // No V-Sync
            case VK_PRESENT_MODE_IMMEDIATE_KHR:
            {
                DebugHelpers::DPrintf(presentModeFormat, "VK_PRESENT_MODE_IMMEDIATE_KHR");

                return availablePresentMode;
            }
            default:
                break;
            }
        }

        DebugHelpers::DPrintf(presentModeFormat, "VK_PRESENT_MODE_FIFO_KHR");

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SolVulkanSwapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        if (capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)())
        {
            return capabilities.currentExtent;
        }

        VkExtent2D actualExtent = _windowExtent;

        actualExtent.width = (std::max)(capabilities.minImageExtent.width,
                                        (std::min)(capabilities.maxImageExtent.width, 
                                                   actualExtent.width));

        actualExtent.height = (std::max)(capabilities.minImageExtent.height,
                                         (std::min)(capabilities.maxImageExtent.height, 
                                                    actualExtent.height));

        return actualExtent;
    }

    VkFormat SolVulkanSwapchain::FindDepthFormat()
    {
        return _rSolDevice.FindSupportedFormat(_vkDepthFormatCandidates,
                                               _vkDepthImageTiling, 
                                               _vkDepthFormatFeatureFlags);
    }
};