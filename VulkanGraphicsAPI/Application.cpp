#include "pch.hpp"
#include "Application.hpp"

namespace SolEngine
{
    Application::Application(HWND &rWindowHandle, 
                             ApplicationData &rAppData)
        : _rWinHandle(rWindowHandle),
          _rAppData(rAppData),
          _pSolVulkanSwapchain(std::make_unique<SolVulkanSwapchain>(*_pSolVulkanDevice, 
                                                                    rAppData.GetExtent())),
          _pSolVulkanDevice(std::make_unique<SolVulkanDevice>(rWindowHandle, 
                                                              rAppData))
        
    {
        PrintDeviceMemoryCapabilities();
        SetupVulkanDrawCommandBuffer();
    }

    void Application::Dispose()
    {
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
        const VkCommandBufferAllocateInfo commandBufferAllocateInfo
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = _pSolVulkanDevice->CommandPool(),
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = _commandBufferCount
        };

        const VkResult result = vkAllocateCommandBuffers(_pSolVulkanDevice->Device(),
                                                         &commandBufferAllocateInfo, 
                                                         &_vkDrawCommandBuffer);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to allocate Draw Command Buffer.")
    }
}
