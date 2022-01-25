#pragma once
#define ENABLE_VULKAN_DEBUG_CALLBACK

#include "ApplicationData.hpp"
#include "SolWindow.hpp"
#include "SwapchainSupportDetails.hpp"
#include "QueueFamilyIndices.hpp"

using namespace Utility;
using namespace SolEngine::Data;

namespace SolEngine
{
    class SolDevice;

    typedef VkBool32(__stdcall SolDevice::*VulkanDebugReportCallback_t)(VkDebugReportFlagsEXT,
                                                                              VkDebugReportObjectTypeEXT,
                                                                              uint64_t,
                                                                              size_t,
                                                                              int32_t,
                                                                              const char *,
                                                                              const char *,
                                                                              void *);

    class SolDevice : private IDisposable
    {
    public:
        SolDevice(SolWindow &rSolWindow, const ApplicationData &appData);
        ~SolDevice();

        // Public Accessors
        VkInstance       GetInstance()		        const { return _instance; }
        VkDevice         GetDevice()		        const { return _device; }
        VkPhysicalDevice GetPhysicalDevice()        const { return _physicalDevice; }
        VkSurfaceKHR     GetSurface()		        const { return _surface; }
        VkCommandPool    GetCommandPool()	        const { return _commandPool; }
        VkQueue          GetGraphicsQueue()         const { return _graphicsQueue; }
        VkQueue          GetPresentQueue()          const { return _presentQueue; }
        uint32_t         GetEnabledLayerCount()	    const { return static_cast<uint32_t>(_enabledLayerNames.size()); }
        uint32_t         GetEnabledExtensionCount() const { return static_cast<uint32_t>(_enabledExtensionNames.size()); }
        uint32_t         GetDeviceExtensionCount()  const { return static_cast<uint32_t>(_logicalDeviceExtensions.size()); }

        void CreateImageWithInfo(const VkImageCreateInfo &imageCreateInfo, const VkMemoryPropertyFlags properties, VkImage &rImage, VkDeviceMemory &rImageMemory);

        SwapchainSupportDetails QueryPhysicalDeviceSwapchainSupport() const { return QuerySwapchainSupport(_physicalDevice); }
        QueueFamilyIndices      QueryPhysicalDeviceQueueFamilies()    const { return QueryQueueFamilies(_physicalDevice); }

        uint32_t FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties) const;
        VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features) const;

        // Buffer Helper Functions
        void CreateBuffer(const VkDeviceSize bufferSize, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer &rBuffer, VkDeviceMemory &rBufferMemory);

        /// <summary>
        /// Creates a Staging Buffer in Device Memory for copying data to Device Local Memory.
        /// Best for data that is set once and doesn't change.
        /// </summary>
        void CreateStagingBuffer(VkBuffer *pOutBuffer, VkDeviceMemory *pOutBufferMemory, const VkDeviceSize bufferSize, const void *pSrcData);

        void CopyBuffer(const VkBuffer srcBuffer, const VkBuffer dstBuffer, const VkDeviceSize size);

        /// <summary>
        /// Destroys the buffer and frees it's memory.
        /// </summary>
        void DisposeBuffer(const VkBuffer buffer, const VkDeviceMemory memory, const VkAllocationCallbacks *pAllocator = NULL);

    private:
        // Inherited via IDisposable
        virtual void Dispose() override;

        void CreateVulkanInstance();
        void CreateVulkanSurface() { _rSolWindow.CreateWindowSurface(_instance, &_surface); };
        void CreateVulkanPhysicalDevice();
        void CreateVulkanDevice();
        void CreateVulkanCommandPool();

        VkCommandBuffer BeginOneTimeCommandBuffer();
        void            EndOneTimeCommandBuffer(const VkCommandBuffer commandBuffer);

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        void CreateVulkanDebugCallback();

        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
                                                                 int32_t messageCode, const char *layerPrefix, const char *message, void *pUserData);
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        // Helper Functions
        SwapchainSupportDetails QuerySwapchainSupport(const VkPhysicalDevice &physicalDevice) const;
        QueueFamilyIndices      QueryQueueFamilies(const VkPhysicalDevice &physicalDevice) const;

        SolWindow             &_rSolWindow;
        const ApplicationData &_appData;

        VkInstance       _instance      { NULL };
        VkDevice         _device        { NULL };
        VkPhysicalDevice _physicalDevice{ NULL };
        VkSurfaceKHR     _surface       { NULL };
        VkCommandPool    _commandPool   { NULL };
        VkQueue          _graphicsQueue { NULL };
        VkQueue          _presentQueue  { NULL };

#if  _DEBUG_LAPTOP
        std::vector<const char*> _enabledLayerNames{};                          // Laptop
#else
        std::vector<const char *> _enabledLayerNames{ "VK_LAYER_NV_optimus" };  // Desktop
#endif //  _DEBUG_LAPTOP

        std::vector<const char *> _enabledExtensionNames;   // Defined in CreateVulkanInstance()
        std::vector<const char *> _logicalDeviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    };
}
