#include "pch.hpp"
#include "SolVulkanRenderer.hpp"

namespace SolEngine
{
	SolVulkanRenderer::SolVulkanRenderer(const ApplicationData &appData, 
										 SolVulkanWindow &rSolWindow, 
										 SolVulkanDevice &rSolDevice)
		: _appData(appData),
		  _rSolWindow(rSolWindow),
		  _rSolDevice(rSolDevice)
	{
        RecreateSwapchain();
        CreateCommandBuffers();
    }

    SolVulkanRenderer::~SolVulkanRenderer()
    {
        Dispose();
    }

    VkCommandBuffer SolVulkanRenderer::BeginFrame()
    {
        DBG_ASSERT_MSG(!_isFrameStarted, 
                       "Cannot call BeginFrame while already in progress!");

        VkResult result = _pSolSwapchain->AcquireNextImage(&_currentImageIndex);

        // After window resize
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapchain();

            return nullptr;
        }

        DBG_ASSERT_MSG((result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR),
                       "Failed to acquire Swapchain Image.");

        // Frame has begun...
        _isFrameStarted = true;

        const VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
        const VkExtent2D& swapchainExtent = _pSolSwapchain->GetExtent();

        const VkCommandBufferBeginInfo commandBufferBeginInfo
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        };

        result = vkBeginCommandBuffer(commandBuffer,
                                      &commandBufferBeginInfo);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Begin Recording Command Buffer.");

        return commandBuffer;
    }

    void SolVulkanRenderer::EndFrame()
    {
        DBG_ASSERT_MSG(_isFrameStarted,
                       "Cannot call EndFrame while frame is not in progress!");

        const VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();

        VkResult result = vkEndCommandBuffer(commandBuffer);

        DBG_ASSERT_VULKAN_MSG(result,
                              "Failed to Record Command Buffer.");

        result = _pSolSwapchain->SubmitCommandBuffers(&commandBuffer,
                                                      &_currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR ||
            result == VK_SUBOPTIMAL_KHR ||
            _rSolWindow.WasWindowResized())
        {
            _rSolWindow.ResetWindowResizedFlag();
            RecreateSwapchain();

            return;
        }

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Present Swapchain Image.");

        _isFrameStarted = false;
    }

    void SolVulkanRenderer::BeginSwapchainRenderPass(const VkCommandBuffer commandBuffer)
    {
        DBG_ASSERT_MSG(_isFrameStarted,
                       "Cannot call BeginSwapchainRenderPass while frame is not in progress!");
        DBG_ASSERT_MSG((commandBuffer == GetCurrentCommandBuffer()),
                       "Cannot begin RenderPass on Command Buffer from a different frame!");

        const VkExtent2D &swapchainExtent = _pSolSwapchain->GetExtent();

        // Index 0 = Colour Attachment
        // Index 1 = Depth Attachment
        const std::array<VkClearValue, 2> clearValues
        {
            VkClearValue
            {
                .color
                {
                    0.01f,  // R
                    0.01f,  // G
                    0.01f,  // B
                    1.0f    // A
                }
            },
            VkClearValue
            {
                .depthStencil
                {
                    .depth   = 1.0f,
                    .stencil = 0
                }
            },
        };

        const VkRenderPassBeginInfo renderPassBeginInfo
        {
            .sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass  = _pSolSwapchain->GetRenderPass(),
            .framebuffer = _pSolSwapchain->GetFramebuffer(_currentImageIndex),
            .renderArea
            {
                .offset = { 0, 0 },
                .extent = swapchainExtent
            },
            .clearValueCount = static_cast<uint32_t>(clearValues.size()),
            .pClearValues    = clearValues.data()
        };

        vkCmdBeginRenderPass(commandBuffer,
                             &renderPassBeginInfo,
                             VK_SUBPASS_CONTENTS_INLINE);

        // Setup Viewport and Scissor
        const VkViewport viewport
        {
            .x        = 0.0f,
            .y        = 0.0f,
            .width    = static_cast<float>(swapchainExtent.width),
            .height   = static_cast<float>(swapchainExtent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        const VkRect2D scissor
        {
            .offset { 0, 0 },
            .extent = swapchainExtent
        };

        // Set them on the Command Buffer
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void SolVulkanRenderer::EndSwapchainRenderPass(const VkCommandBuffer commandBuffer)
    {
        DBG_ASSERT_MSG(_isFrameStarted,
                       "Cannot call EndSwapchainRenderPass while frame is not in progress!");
        DBG_ASSERT_MSG((commandBuffer == GetCurrentCommandBuffer()),
                       "Cannot begin RenderPass on Command Buffer from a different frame!");

        vkCmdEndRenderPass(commandBuffer);
    }

    void SolVulkanRenderer::Dispose()
	{
        FreeCommandBuffers();
	}

	void SolVulkanRenderer::PrintDeviceMemoryCapabilities()
	{
        // Query device for memory count
        vkGetPhysicalDeviceQueueFamilyProperties(_rSolDevice.PhysicalDevice(),
                                                 &_physDeviceQueueFamilyCount,
                                                 NULL);

        // Create vector of the size of the family count
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(_physDeviceQueueFamilyCount);

        // Retrieve properties
        vkGetPhysicalDeviceQueueFamilyProperties(_rSolDevice.PhysicalDevice(),
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

    void SolVulkanRenderer::CreateCommandBuffers()
    {
        _vkCommandBuffers.resize(_pSolSwapchain->GetImageCount());

        const VkCommandBufferAllocateInfo commandBufferAllocateInfo
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = _rSolDevice.CommandPool(),
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(_vkCommandBuffers.size())
        };

        const VkResult result = vkAllocateCommandBuffers(_rSolDevice.Device(),
                                                         &commandBufferAllocateInfo,
                                                         _vkCommandBuffers.data());

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Allocate Command Buffers.");
    }

    void SolVulkanRenderer::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(_rSolDevice.Device(), 
                             _rSolDevice.CommandPool(),
                             static_cast<uint32_t>(_vkCommandBuffers.size()), 
                             _vkCommandBuffers.data());

        _vkCommandBuffers.clear();
    }

    void SolVulkanRenderer::RecreateSwapchain()
    {
        VkExtent2D winExtent = _rSolWindow.GetWindowExtent();

        // Whilst a dimension of the window is dimensionless,
        // wait until it is set correctly.
        while (winExtent.width == 0 || winExtent.height == 0)
        {
            winExtent = _rSolWindow.GetWindowExtent();

            glfwWaitEvents();
        }

        // Wait until the current Swapchain is no longer being used
        // before creating a new one.
        vkDeviceWaitIdle(_rSolDevice.Device());

        // Check if there's an old Swapchain to be passed
        if (_pSolSwapchain == nullptr)
        {
            _pSolSwapchain = std::make_unique<SolVulkanSwapchain>(_rSolDevice, winExtent);
        }
        else
        {
            _pSolSwapchain = std::make_unique<SolVulkanSwapchain>(_rSolDevice,
                                                                  winExtent,
                                                                  std::move(_pSolSwapchain));

            if (_pSolSwapchain->GetImageCount() != _vkCommandBuffers.size())
            {
                FreeCommandBuffers();
                CreateCommandBuffers();
            }
        }

        _pSolSwapchain = nullptr; // TEMP: Ensure old swap chain is destroyed to prevent 2 swapchains co-existing.
        _pSolSwapchain = std::make_unique<SolVulkanSwapchain>(_rSolDevice, winExtent);
    }
}