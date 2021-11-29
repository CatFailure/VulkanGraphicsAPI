#include "pch.hpp"
#include "Application.hpp"

namespace SolEngine
{
    Application::Application(HWND &rWindowHandle, 
                             ApplicationData &rAppData)
        : _rWinHandle(rWindowHandle),
          _rAppData(rAppData),
          _pSolVulkanDevice(std::make_unique<SolVulkanDevice>(rWindowHandle, 
                                                              rAppData))
        
    {
        VkExtent2D extent = rAppData.GetExtent();
        _pSolVulkanSwapchain = std::make_unique<SolVulkanSwapchain>(*_pSolVulkanDevice, rAppData.GetExtent());

        SetupVulkanSwapchain();
        PrintDeviceMemoryCapabilities();
        SetupVulkanDrawCommandBuffer();
    }

    void Application::Dispose()
    {
    }

    void Application::SetupVulkanSwapchain()
    {
        SetupVulkanSwapchain_CreateSwapchain();
        SetupVulkanSwapchain_CreateImages();
        SetupVulkanSwapchain_CreateImageViews();
    }

    void Application::SetupVulkanSwapchain_CreateSwapchain()
    {
        // Structure listing surface capabilities
        VkSurfaceCapabilitiesKHR surfaceCapabilities{};
        
        // Fill structure with data
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_pSolVulkanDevice->PhysicalDevice(), 
                                                  _pSolVulkanDevice->Surface(), 
                                                  &surfaceCapabilities);

        // Retrieve and use the actual back/front buffer widths
        VkExtent2D surfaceResolution = surfaceCapabilities.currentExtent;
        _surfaceBufferWidth = surfaceResolution.width;
        _surfaceBufferHeight = surfaceResolution.height;

        VkSwapchainCreateInfoKHR swapchainCreateInfo
        {
            .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface          = _pSolVulkanDevice->Surface(),
            .minImageCount    = SWAPCHAIN_BUFFER_COUNT,
            .imageFormat      = VK_FORMAT_B8G8R8A8_UNORM,
            .imageColorSpace  = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
            .imageExtent      = surfaceResolution,
            .imageArrayLayers = 1,
            .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .preTransform     = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode      = VK_PRESENT_MODE_MAILBOX_KHR,
            .clipped          = true,    // Clipping outside of extents?
            .oldSwapchain     = NULL
        };

        VkResult result = vkCreateSwapchainKHR(_pSolVulkanDevice->Device(),
                                               &swapchainCreateInfo, 
                                               NULL, 
                                               &_vkSwapchain);

        // Was this successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Swapchain.");
    }

    void Application::SetupVulkanSwapchain_CreateImages()
    {
        // How many images need to be created?
        vkGetSwapchainImagesKHR(_pSolVulkanDevice->Device(),
                                _vkSwapchain, 
                                &_swapChainImageCount,
                                NULL);

        // Make sure the images match what is expected
        const bool isCorrectSwapChainImageCount = _swapChainImageCount == SWAPCHAIN_BUFFER_COUNT;
        DBG_ASSERT(isCorrectSwapChainImageCount);

        _vkSwapchainImages = std::vector<VkImage>(_swapChainImageCount);
        
        // Link the images to the Swapchain
        VkResult result = vkGetSwapchainImagesKHR(_pSolVulkanDevice->Device(),
                                                  _vkSwapchain,
                                                  &_swapChainImageCount,
                                                  _vkSwapchainImages.data());

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Swapchain Images.");
    }

    void Application::SetupVulkanSwapchain_CreateImageViews()
    {
        _vkSwapchainImageViews = std::vector<VkImageView>(SWAPCHAIN_BUFFER_COUNT);

        for (uint32_t i(0); i < SWAPCHAIN_BUFFER_COUNT; ++i)
        {
            // Create VkImageViews for the Swapchain
            VkImageViewCreateInfo imageViewCreateInfo
            {
                .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image    = _vkSwapchainImages.at(i),
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format   = VK_FORMAT_B8G8R8A8_UNORM,
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

            VkResult result = vkCreateImageView(_pSolVulkanDevice->Device(),
                                                &imageViewCreateInfo,
                                                NULL, 
                                                &_vkSwapchainImageViews.at(i));

            // Was it successful?
            DBG_ASSERT_VULKAN_MSG(result, "Failed to create ImageView");
        }
    }

    void Application::PrintDeviceMemoryCapabilities()
    {
        // Query device for memory count
        vkGetPhysicalDeviceQueueFamilyProperties(_pSolVulkanDevice->PhysicalDevice(),
                                                 &_physDeviceQueueFamilyCount,
                                                 NULL);

        // Create vector of the size of the family count
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(_physDeviceQueueFamilyCount);

        // Retrieve properties
        vkGetPhysicalDeviceQueueFamilyProperties(_pSolVulkanDevice->PhysicalDevice(),
                                                 &_physDeviceQueueFamilyCount,
                                                 &queueFamilyProperties.at(0));

        // Print out the families
        for (uint32_t i(0); i < _physDeviceCount; ++i)
        {
            for (uint32_t j(0); j < _physDeviceQueueFamilyCount; ++j)
            {
                const VkQueueFamilyProperties& currentQueueFamilyProperties = queueFamilyProperties.at(j);
                const VkQueueFlags& queueFlags = currentQueueFamilyProperties.queueFlags;

                DebugHelpers::DPrintf("Queue count: %d\n", currentQueueFamilyProperties.queueCount);
                DebugHelpers::DPrintf("Supporting operating on this queue:\n");

                if (queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    DebugHelpers::DPrintf("\t\tGraphics\n");
                }

                if (queueFlags & VK_QUEUE_COMPUTE_BIT)
                {
                    DebugHelpers::DPrintf("\t\tCompute\n");
                }

                if (queueFlags & VK_QUEUE_TRANSFER_BIT)
                {
                    DebugHelpers::DPrintf("\t\tTransfer\n");
                }

                if (queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
                {
                    DebugHelpers::DPrintf("\t\tSparse Binding\n");
                }
            }
        }
    }

    void Application::SetupVulkanDrawCommandBuffer()
    {
        // Define type of Command Buffer
        VkCommandBufferAllocateInfo commandBufferAllocateInfo
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = _pSolVulkanDevice->CommandPool(),
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = _commandBufferCount
        };

        VkResult result = vkAllocateCommandBuffers(_pSolVulkanDevice->Device(),
                                                   &commandBufferAllocateInfo, 
                                                   &_vkDrawCommandBuffer);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to allocate Draw Command Buffer.")
    }

    void Application::SetupFrameBufferRenderPass()
    {
#ifdef DEPTH_BUFFER
        // (Optional) - Setup a depth buffer
        VkImage depthImage{ NULL };
        VkImageView depthImageView{ NULL };

        // Create the Depth Image
        VkImageCreateInfo depthImageCreateInfo
        {
            .sType     = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = VK_IMAGE_TYPE_2D,
            .format    = VK_FORMAT_D16_UNORM,
            .extent 
            {
                .width  = _surfaceBufferWidth,
                .height = _surfaceBufferHeight,
                .depth  = 1
            },
            .mipLevels   = 1,
            .arrayLayers = 1,
            .samples               = VK_SAMPLE_COUNT_1_BIT,
            .tiling                = VK_IMAGE_TILING_OPTIMAL,
            .usage                 = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .sharingMode           = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices   = NULL,
            .initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED
        };

        VkResult result = vkCreateImage(_pSolVulkanDevice->Device(),
                                        &depthImageCreateInfo, 
                                        NULL, 
                                        &depthImage);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Depth Image.");
#endif // DEPTH_BUFFER
    }
}
