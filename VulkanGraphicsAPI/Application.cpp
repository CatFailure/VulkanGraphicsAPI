#include "pch.hpp"
#include "Application.hpp"

namespace SolEngine
{
    Application::Application(ApplicationData &rAppData)
        : _rAppData(rAppData),
          _pSolVulkanSwapchain(std::make_unique<SolVulkanSwapchain>(*_pSolVulkanDevice, 
                                                                    rAppData.GetExtent())),
          _pSolVulkanDevice(std::make_unique<SolVulkanDevice>(*_pSolVulkanWindow, 
                                                              rAppData)),
        _pSolVulkanWindow(std::make_unique<SolVulkanWindow>(rAppData.windowTitle,
                                                            rAppData.windowDimensions))
    {
        PrintDeviceMemoryCapabilities();

        CreatePipelineLayout();
        RecreateSwapchain();
        CreateCommandBuffers();
    }

    void Application::Run()
    {
        while (!_pSolVulkanWindow->ShouldClose())
        {
            glfwPollEvents();   // Poll Window Events
            DrawFrame();
        }

        vkDeviceWaitIdle(_pSolVulkanDevice->Device());
    }

    void Application::Dispose()
    {
        vkDestroyPipelineLayout(_pSolVulkanDevice->Device(), 
                                _vkPipelineLayout, 
                                NULL);
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

    void Application::DrawFrame()
    {
        uint32_t imageIndex;
        VkResult result = _pSolVulkanSwapchain->AcquireNextImage(&imageIndex);

        // After window resize
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            RecreateSwapchain();

            return;
        }

        DBG_ASSERT_MSG((result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR), 
                       "Failed to acquire Swapchain Image.");

        RecordCommandBuffer(imageIndex);

        result = _pSolVulkanSwapchain->SubmitCommandBuffers(&_vkCommandBuffers.at(imageIndex), 
                                                            &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR ||
            result == VK_SUBOPTIMAL_KHR ||
            _pSolVulkanWindow->WasWindowResized())
        {
            _pSolVulkanWindow->ResetWindowResizedFlag();
            RecreateSwapchain();

            return;
        }

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Present Swapchain Image.");
    }

    void Application::CreatePipelineLayout()
    {
        const VkPushConstantRange pushConstantRange
        {
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,    // Allow access to push constant data and both vertex/frag shaders.
            .offset = 0,
            .size = sizeof(SimplePushConstantData)
        };

        const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo
        {
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount         = 0,
            .pSetLayouts            = NULL,
            .pushConstantRangeCount = 1,
            .pPushConstantRanges    = &pushConstantRange
        };

        const VkResult result = vkCreatePipelineLayout(_pSolVulkanDevice->Device(),
                                                       &pipelineLayoutCreateInfo, 
                                                       NULL, 
                                                       &_vkPipelineLayout);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Pipeline Layout.");
    }

    void Application::CreatePipeline()
    {
        DBG_ASSERT_MSG((_pSolVulkanSwapchain != nullptr), "Cannot create Pipeline before Swapchain.");
        DBG_ASSERT_MSG((_vkPipelineLayout != nullptr), "Cannot create Pipeline before Pipeline Layout.");

        PipelineConfigInfo pipelineConfigInfo{};
        SolVulkanPipeline::DefaultPipelineConfigInfo(pipelineConfigInfo);

        _pSolVulkanPipeline = std::make_unique<SolVulkanPipeline>(*_pSolVulkanDevice,
                                                                  "Shaders/SimpleShader.vert.spv",
                                                                  "Shaders/SimpleShader.frag.spv",
                                                                  pipelineConfigInfo);
    }

    void Application::CreateCommandBuffers()
    {
        _vkCommandBuffers.resize(_pSolVulkanSwapchain->ImageCount());

        const VkCommandBufferAllocateInfo commandBufferAllocateInfo
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = _pSolVulkanDevice->CommandPool(),
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = static_cast<uint32_t>(_vkCommandBuffers.size())
        };

        const VkResult result = vkAllocateCommandBuffers(_pSolVulkanDevice->Device(), 
                                                         &commandBufferAllocateInfo,
                                                         _vkCommandBuffers.data());

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Allocate Command Buffers.");
    }

    void Application::RecordCommandBuffer(const size_t imageIndex)
    {
        const VkCommandBuffer &currentCommandBuffer = _vkCommandBuffers.at(imageIndex);
        const VkExtent2D      &swapchainExtent      = _pSolVulkanSwapchain->Extent();

        const VkCommandBufferBeginInfo commandBufferBeginInfo
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        };

        VkResult result = vkBeginCommandBuffer(currentCommandBuffer,
                                               &commandBufferBeginInfo);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Begin Recording Command Buffer.");

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
            .renderPass  = _pSolVulkanSwapchain->RenderPass(),
            .framebuffer = _pSolVulkanSwapchain->Framebuffer(imageIndex),
            .renderArea
            {
                .offset = { 0, 0 },
                .extent = swapchainExtent
            },
            .clearValueCount = static_cast<uint32_t>(clearValues.size()),
            .pClearValues    = clearValues.data()
        };

        vkCmdBeginRenderPass(currentCommandBuffer,
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
        vkCmdSetViewport(currentCommandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(currentCommandBuffer, 0, 1, &scissor);

        // Render stuff here

        // End of Rendering
        vkCmdEndRenderPass(currentCommandBuffer);

        result = vkEndCommandBuffer(currentCommandBuffer);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Record Command Buffer.");
    }

    void Application::FreeCommandBuffers()
    {
        vkFreeCommandBuffers(_pSolVulkanDevice->Device(), 
                             _pSolVulkanDevice->CommandPool(), 
                             static_cast<uint32_t>(_vkCommandBuffers.size()), 
                             _vkCommandBuffers.data());

        _vkCommandBuffers.clear();
    }

    void Application::RecreateSwapchain()
    {
        VkExtent2D winExtent = _pSolVulkanWindow->GetWindowExtent();

        // Whilst a dimension of the window is dimensionless,
        // wait until it is set correctly.
        while (winExtent.width == 0 || winExtent.height == 0)
        {
            winExtent = _pSolVulkanWindow->GetWindowExtent();

            glfwWaitEvents();
        }

        // Wait until the current Swapchain is no longer being used
        // before creating a new one.
        vkDeviceWaitIdle(_pSolVulkanDevice->Device());

        // Check if there's an old Swapchain to be passed
        if (_pSolVulkanSwapchain == nullptr)
        {
            _pSolVulkanSwapchain = std::make_unique<SolVulkanSwapchain>(*_pSolVulkanDevice, winExtent);
        }
        else
        {
            _pSolVulkanSwapchain = std::make_unique<SolVulkanSwapchain>(*_pSolVulkanDevice, 
                                                                        winExtent,
                                                                        std::move(_pSolVulkanSwapchain));

            if (_pSolVulkanSwapchain->ImageCount() != _vkCommandBuffers.size())
            {
                FreeCommandBuffers();
                CreateCommandBuffers();
            }
        }

        _pSolVulkanSwapchain = nullptr; // TEMP: Ensure old swap chain is destroyed to prevent 2 swapchains co-existing.
        _pSolVulkanSwapchain = std::make_unique<SolVulkanSwapchain>(*_pSolVulkanDevice, winExtent);

        // If render pass compatible do nothing else
        CreatePipeline();
    }
}
