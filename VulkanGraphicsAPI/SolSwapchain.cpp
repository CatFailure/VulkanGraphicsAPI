#include "SolSwapchain.hpp"

namespace SolEngine
{
    SolSwapchain::SolSwapchain(SolDevice &rSolDevice,
                               const VkExtent2D &windowExtent)
        : SolSwapchain(rSolDevice, windowExtent, nullptr)
    {}

    SolSwapchain::SolSwapchain(SolDevice &rSolDevice, 
                               const VkExtent2D &windowExtent, 
                               std::shared_ptr<SolSwapchain> pOldSwapchain)
        : _rSolDevice(rSolDevice),
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
        CreateRenderPass();
        CreateFramebuffers();
        CreateSyncObjects();

        // Old Swapchain no longer needed
        _pOldSwapchain = nullptr;
    }

    SolSwapchain::~SolSwapchain()
    {
        Dispose();
    }

    void SolSwapchain::Dispose()
    {
        const VkDevice vkDevice = _rSolDevice.GetDevice();

        // Image Views
        {
            for (const VkImageView &imageView : _swapchainImageViews)
            {
                vkDestroyImageView(vkDevice, 
                                   imageView, 
                                   NULL);
            }

            _swapchainImageViews.clear();
        }

        // Swapchain, if possible
        {
            if (_swapchain != nullptr)
            {
                vkDestroySwapchainKHR(vkDevice, 
                                      _swapchain,
                                      NULL);

                _swapchain = nullptr;
            }
        }
        
        // Depth Resources
        {
            for (size_t i(0); i < _depthImages.size(); ++i)
            {
                vkDestroyImageView(vkDevice,
                                   _depthImageViews.at(i),
                                   NULL);

                vkDestroyImage(vkDevice,
                               _depthImages.at(i),
                               NULL);

                vkFreeMemory(vkDevice, 
                             _depthImageMemories.at(i),
                             NULL);
            }

            _depthImageViews.clear();
            _depthImages.clear();
            _depthImageMemories.clear();
        }

        // Framebuffers
        {
            for (const VkFramebuffer &framebuffer : _swapchainFrameBuffers)
            {
                vkDestroyFramebuffer(vkDevice, 
                                     framebuffer, 
                                     NULL);
            }
        }

        // Render Pass
        {
            vkDestroyRenderPass(vkDevice,
                                _renderPass, 
                                NULL);
        }

        // Synchronisation Objects
        {
            for (size_t i(0); i < MAX_FRAMES_IN_FLIGHT; ++i)
            {
                vkDestroySemaphore(vkDevice,
                                   _renderFinishedSemaphores.at(i), 
                                   NULL);

                vkDestroySemaphore(vkDevice,
                                   _imageAvailableSemaphores.at(i),
                                   NULL);

                vkDestroyFence(vkDevice, 
                               _inFlightFences.at(i), 
                               NULL);
            }
        }
    }

    void SolSwapchain::CreateSwapchain(uint32_t *pOutImageCount,
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
        *pOutSwapchainExtent    = ChooseSwapchainExtent(swapchainSupportDetails.surfaceCapabilities);    // Retrieve and use the actual back/front buffer widths
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
            .surface          = _rSolDevice.GetSurface(),
            .minImageCount    = *pOutImageCount,
            .imageFormat      = pOutSurfaceImageFormat->format,
            .imageColorSpace  = pOutSurfaceImageFormat->colorSpace,
            .imageExtent      = *pOutSwapchainExtent,
            .imageArrayLayers = 1,
            .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform     = swapchainSupportDetails.surfaceCapabilities.currentTransform,
            .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode      = presentMode,
            .clipped          = VK_TRUE,                                            // Clipping outside of extents?
            .oldSwapchain     = _pOldSwapchain == nullptr ?                         // Provide old swapchain if possible
                                    VK_NULL_HANDLE : _pOldSwapchain->_swapchain
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

        const VkResult result = vkCreateSwapchainKHR(_rSolDevice.GetDevice(),
                                                     &swapchainCreateInfo,
                                                     NULL,
                                                     &_swapchain);

        // Was this successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Swapchain.");
    }

    void SolSwapchain::CreateSwapchainImages(uint32_t &rImageCount, 
                                             const VkSurfaceFormatKHR &surfaceImageFormat, 
                                             const VkExtent2D &swapchainExtent)
    {
        // We only specified a minimum number of images in the swapchain, so the implementation is
        // allowed to create a swapchain with more. That's why we'll first query the final number of
        // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
        // retrieve the handles.
        VkResult result = vkGetSwapchainImagesKHR(_rSolDevice.GetDevice(),
                                                  _swapchain,
                                                  &rImageCount,
                                                  NULL);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to retrieve Swapchain Images.");

        _swapchainImages.resize(rImageCount);

        result = vkGetSwapchainImagesKHR(_rSolDevice.GetDevice(),
                                         _swapchain,
                                         &rImageCount,
                                         _swapchainImages.data());

        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Swapchain Images.");

        _swapchainImageFormat = surfaceImageFormat.format;
        _swapchainExtent      = swapchainExtent;
    }

    void SolSwapchain::CreateSwapchainImageViews()
    {
        const size_t swapchainImageCount = _swapchainImages.size();

        _swapchainImageViews.resize(swapchainImageCount);

        for (size_t i(0); i < swapchainImageCount; ++i)
        {
            const VkImageViewCreateInfo imageViewCreateInfo
            {
                .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image    = _swapchainImages.at(i),
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format   = _swapchainImageFormat,
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

            const VkResult result = vkCreateImageView(_rSolDevice.GetDevice(),
                                                      &imageViewCreateInfo, 
                                                      NULL,
                                                      &_swapchainImageViews.at(i));

            DBG_ASSERT_VULKAN_MSG(result, "Failed to create Texture Image View.");
        }
    }

    void SolSwapchain::CreateDepthResources()
    {
        const VkFormat depthFormat       = FindDepthFormat();
        const size_t swapchainImageCount = GetImageCount();

        _swapchainDepthFormat = depthFormat;
        _depthImages.resize(swapchainImageCount);
        _depthImageMemories.resize(swapchainImageCount);
        _depthImageViews.resize(swapchainImageCount);

        for (size_t i(0); i < swapchainImageCount; ++i)
        {
            const VkImageCreateInfo imageCreateInfo
            {
                .sType     = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .flags     = NULL,
                .imageType = VK_IMAGE_TYPE_2D,
                .format    = depthFormat,
                .extent
                {
                    .width  = _swapchainExtent.width,
                    .height = _swapchainExtent.height,
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

            VkImage& rCurrentDepthImage = _depthImages.at(i);

            _rSolDevice.CreateImageWithInfo(imageCreateInfo, 
                                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                            rCurrentDepthImage,
                                            _depthImageMemories.at(i));

            const VkImageViewCreateInfo imageViewCreateInfo
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

            const VkResult result = vkCreateImageView(_rSolDevice.GetDevice(),
                                                      &imageViewCreateInfo, 
                                                      NULL, 
                                                      &_depthImageViews.at(i));

            DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Image View.");
        }
    }

    void SolSwapchain::CreateRenderPass()
    {
        // Depth Stencil
        const VkAttachmentDescription depthStencilAttachment
        {
            .format         = FindDepthFormat(),
            .samples        = VK_SAMPLE_COUNT_1_BIT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        const VkAttachmentReference depthStencilAttachmentReference
        {
            .attachment = 1,
            .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        // Colour
        const VkAttachmentDescription colourAttachment
        {
            .format         = _swapchainImageFormat,
            .samples        = VK_SAMPLE_COUNT_1_BIT,
            .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };

        const VkAttachmentReference colourAttachmentReference
        {
            .attachment = 0,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        // Subpass
        const VkSubpassDescription subpassDescription
        {
            .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount    = 1,
            .pColorAttachments       = &colourAttachmentReference,
            .pDepthStencilAttachment = &depthStencilAttachmentReference
        };

        const VkSubpassDependency subpassDependency
        {
            .srcSubpass    = VK_SUBPASS_EXTERNAL,
            .dstSubpass    = 0,
            .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        };

        const std::array<VkAttachmentDescription, 2> attachments
        {
            colourAttachment,
            depthStencilAttachment
        };

        const VkRenderPassCreateInfo renderPassCreateInfo
        {
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments    = attachments.data(),
            .subpassCount    = 1,
            .pSubpasses      = &subpassDescription,
            .dependencyCount = 1,
            .pDependencies   = &subpassDependency
        };

        const VkResult result = vkCreateRenderPass(_rSolDevice.GetDevice(), 
                                                   &renderPassCreateInfo, 
                                                   NULL, 
                                                   &_renderPass);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Render Pass.");
    }

    void SolSwapchain::CreateFramebuffers()
    {
        const size_t imageCount = GetImageCount();

        _swapchainFrameBuffers.resize(imageCount);

        for (size_t i(0); i < imageCount; ++i)
        {
            const std::array<VkImageView, 2> imageAttachments
            {
                _swapchainImageViews.at(i),
                _depthImageViews.at(i)
            };

            const VkFramebufferCreateInfo framebufferCreateInfo
            {
                .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .renderPass      = _renderPass,
                .attachmentCount = static_cast<uint32_t>(imageAttachments.size()),
                .pAttachments    = imageAttachments.data(),
                .width           = _swapchainExtent.width,
                .height          = _swapchainExtent.height,
                .layers          = 1
            };

            const VkResult result = vkCreateFramebuffer(_rSolDevice.GetDevice(), 
                                                        &framebufferCreateInfo, 
                                                        NULL, 
                                                        &_swapchainFrameBuffers.at(i));

            DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Framebuffer.");
        }
    }

    void SolSwapchain::CreateSyncObjects()
    {
        const VkDevice &vkDevice = _rSolDevice.GetDevice();

        _imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        _renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        _inFlightImages.resize(GetImageCount(), VK_NULL_HANDLE);

        const VkSemaphoreCreateInfo semaphoreCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
        };

        const VkFenceCreateInfo fenceCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT
        };

        for (size_t i(0); i < MAX_FRAMES_IN_FLIGHT; ++i)
        {
            VkResult result = vkCreateSemaphore(vkDevice, 
                                                &semaphoreCreateInfo,
                                                NULL, 
                                                &_imageAvailableSemaphores.at(i));

            DBG_ASSERT_VULKAN_MSG(result, "Failed to create Semaphore - Image Available for a frame.");

            result = vkCreateSemaphore(vkDevice, 
                                       &semaphoreCreateInfo, 
                                       NULL, 
                                       &_renderFinishedSemaphores.at(i));

            DBG_ASSERT_VULKAN_MSG(result, "Failed to create Semaphore - Render Finished for a frame.");

            result = vkCreateFence(vkDevice, 
                                   &fenceCreateInfo,    
                                   NULL, 
                                   &_inFlightFences.at(i));

            DBG_ASSERT_VULKAN_MSG(result, "Failed to create Fence - In Flight Fences for a frame.");
        }
    }

    VkSurfaceFormatKHR SolSwapchain::ChooseImageFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
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

    VkPresentModeKHR SolSwapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
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

    VkExtent2D SolSwapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities)
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

    VkFormat SolSwapchain::FindDepthFormat()
    {
        return _rSolDevice.FindSupportedFormat(_depthFormatCandidates,
                                               _depthImageTiling, 
                                               _depthFormatFeatureFlags);
    }

    VkResult SolSwapchain::AcquireNextImage(uint32_t *pImageIndex)
    {
        const VkDevice &device = _rSolDevice.GetDevice();
        const uint32_t fenceCount(1);

        VkResult result = vkWaitForFences(device, 
                                          fenceCount, 
                                          &_inFlightFences.at(_currentFrame), 
                                          VK_TRUE, 
                                          _timeoutDuration);

        DBG_ASSERT_VULKAN_MSG(result, "Wait For Fences Failed.");

        result = vkAcquireNextImageKHR(device, 
                                       _swapchain,
                                       _timeoutDuration, 
                                       _imageAvailableSemaphores.at(_currentFrame),   // Must be a non-signaled semaphore
                                       VK_NULL_HANDLE, 
                                       pImageIndex);

        return result;
    }

    VkResult SolSwapchain::SubmitCommandBuffers(const VkCommandBuffer *pCommandBuffers, 
                                                      const uint32_t *pImageIndex)
    {
        const VkDevice &device  = _rSolDevice.GetDevice();
        VkFence &rInFlightImage = _inFlightImages.at(*pImageIndex);
        VkFence &rInFlightFence = _inFlightFences.at(_currentFrame);

        if (rInFlightImage != VK_NULL_HANDLE)
        {
            vkWaitForFences(device, 
                            1,
                            &rInFlightImage, 
                            VK_TRUE, 
                            _timeoutDuration);
        }

        rInFlightImage = rInFlightFence;

        const VkSemaphore          waitSemaphores[]  { _imageAvailableSemaphores.at(_currentFrame) };
        const VkPipelineStageFlags waitStageFlags[]  { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        const VkSemaphore          signalSemaphores[]{ _renderFinishedSemaphores.at(_currentFrame) };
        const VkSwapchainKHR       swapChains[]      { _swapchain };

        const VkSubmitInfo submitInfo
        {
            .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount   = 1,
            .pWaitSemaphores      = waitSemaphores,
            .pWaitDstStageMask    = waitStageFlags,
            .commandBufferCount   = 1,
            .pCommandBuffers      = pCommandBuffers,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores    = signalSemaphores
        };

        vkResetFences(device, 1, &rInFlightFence);

        VkResult result = vkQueueSubmit(_rSolDevice.GetGraphicsQueue(), 
                                        1,
                                        &submitInfo,
                                        rInFlightFence);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Submit Draw Command Buffer.");

        const VkPresentInfoKHR presentInfo
        {
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = signalSemaphores,
            .swapchainCount     = 1,
            .pSwapchains        = swapChains,
            .pImageIndices      = pImageIndex
        };

        result = vkQueuePresentKHR(_rSolDevice.GetPresentQueue(), 
                                   &presentInfo);

        // Flip current frame
        _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
    }

    bool SolSwapchain::CompareSwapchanFormats(const SolSwapchain& swapchain) const
    {
        // When a swapchain is re-created, these values may change.
        return swapchain._swapchainDepthFormat == _swapchainDepthFormat &&
               swapchain._swapchainImageFormat == _swapchainImageFormat;
    }
};