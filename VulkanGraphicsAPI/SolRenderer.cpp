#include "pch.hpp"
#include "SolRenderer.hpp"

namespace SolEngine
{
	SolRenderer::SolRenderer(const ApplicationData &appData, 
										 SolWindow &rSolWindow, 
										 SolDevice &rSolDevice)
		: _appData(appData),
		  _rSolWindow(rSolWindow),
		  _rSolDevice(rSolDevice)
	{
        RecreateSwapchain();
        CreateCommandBuffers();
    }

    SolRenderer::~SolRenderer()
    {
        Dispose();
    }

    VkCommandBuffer SolRenderer::GetCurrentCommandBuffer() const
    {
        DBG_ASSERT_MSG(_isFrameStarted,
                       "Cannot get Command Buffer when frame is not in progress!");

        return _commandBuffers.at(_currentFrameIndex);
    }

    size_t SolRenderer::GetFrameIndex() const
    {
        DBG_ASSERT_MSG(_isFrameStarted,
                       "Cannot get Frame Index when frame is not in progress!");

        return _currentFrameIndex;
    }

    VkCommandBuffer SolRenderer::BeginFrame()
    {
        // Ensure multiple frames can't be started.
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

    void SolRenderer::EndFrame()
    {
        // You can't end a frame that was never started...
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

            _isFrameStarted = false;

            return;
        }

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Present Swapchain Image.");

        _isFrameStarted = false;
        _currentFrameIndex = (_currentFrameIndex + 1) % SolSwapchain::MAX_FRAMES_IN_FLIGHT;
    }

    void SolRenderer::BeginSwapchainRenderPass(const VkCommandBuffer commandBuffer)
    {
        // Swapchain Render Pass' require a frame in progress.
        // If a frame is started, the passed command buffer MUST be the current frame.
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

    void SolRenderer::EndSwapchainRenderPass(const VkCommandBuffer commandBuffer)
    {
        // Swapchain Render Pass' require a frame in progress.
        // If a frame is started, the passed command buffer MUST be the current frame.
        DBG_ASSERT_MSG(_isFrameStarted,
                       "Cannot call EndSwapchainRenderPass while frame is not in progress!");
        DBG_ASSERT_MSG((commandBuffer == GetCurrentCommandBuffer()),
                       "Cannot begin RenderPass on Command Buffer from a different frame!");

        vkCmdEndRenderPass(commandBuffer);
    }

    void SolRenderer::Dispose()
	{
        FreeCommandBuffers();
	}

	void SolRenderer::PrintDeviceMemoryCapabilities()
	{
        const VkPhysicalDevice &physicalDevice = _rSolDevice.GetPhysicalDevice();

        // Query device for memory count
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
                                                 &_physDeviceQueueFamilyCount,
                                                 NULL);

        // Create vector of the size of the family count
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(_physDeviceQueueFamilyCount);

        // Retrieve properties
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
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

    void SolRenderer::CreateCommandBuffers()
    {
        _commandBuffers.resize(SolSwapchain::MAX_FRAMES_IN_FLIGHT);

        const VkCommandBufferAllocateInfo commandBufferAllocateInfo
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = _rSolDevice.GetCommandPool(),
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(_commandBuffers.size())
        };

        const VkResult result = vkAllocateCommandBuffers(_rSolDevice.GetDevice(),
                                                         &commandBufferAllocateInfo,
                                                         _commandBuffers.data());

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Allocate Command Buffers.");
    }

    void SolRenderer::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(_rSolDevice.GetDevice(), 
                             _rSolDevice.GetCommandPool(),
                             static_cast<uint32_t>(_commandBuffers.size()), 
                             _commandBuffers.data());

        _commandBuffers.clear();
    }

    void SolRenderer::RecreateSwapchain()
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
        vkDeviceWaitIdle(_rSolDevice.GetDevice());

        // Check if there's an old Swapchain to be passed
        if (_pSolSwapchain == nullptr)
        {
            _pSolSwapchain = std::make_unique<SolSwapchain>(_rSolDevice, winExtent);
        }
        else
        {
            std::shared_ptr<SolSwapchain> pOldSwapchain = std::move(_pSolSwapchain);

            _pSolSwapchain = std::make_unique<SolSwapchain>(_rSolDevice,
                                                                  winExtent,
                                                                  pOldSwapchain);

            // Ensure re-created swapchain is compatible...
            DBG_ASSERT_MSG((pOldSwapchain->CompareSwapchanFormats(*_pSolSwapchain)), 
                           "Swapchain Image (Or Depth) Format has changed!");
        }

        _pSolSwapchain = nullptr; // TEMP: Ensure old swap chain is destroyed to prevent 2 swapchains co-existing.
        _pSolSwapchain = std::make_unique<SolSwapchain>(_rSolDevice, winExtent);
    }
}