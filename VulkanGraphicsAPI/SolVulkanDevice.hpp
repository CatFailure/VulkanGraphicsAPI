#pragma once
#define ENABLE_VULKAN_DEBUG_CALLBACK

#include "ApplicationData.hpp"
#include "SolVulkanWindow.hpp"
#include "SwapchainSupportDetails.hpp"
#include "QueueFamilyIndices.hpp"

using namespace Utility;
using namespace SolEngine::Data;

namespace SolEngine
{
    class SolVulkanDevice;

    typedef VkBool32(__stdcall SolVulkanDevice::*VulkanDebugReportCallback_t)(VkDebugReportFlagsEXT,
                                                                              VkDebugReportObjectTypeEXT,
                                                                              uint64_t,
                                                                              size_t,
                                                                              int32_t,
                                                                              const char *,
                                                                              const char *,
                                                                              void *);

    class SolVulkanDevice : public IDisposable
    {
    public:
        SolVulkanDevice(SolVulkanWindow &rSolVulkanWindow, ApplicationData &rAppData);
        ~SolVulkanDevice();

        // Public Accessors
        VkInstance       Instance()		  const { return _vkInstance; }
        VkDevice         Device()		  const { return _vkDevice; }
        VkPhysicalDevice PhysicalDevice() const { return _vkPhysicalDevice; }
        VkSurfaceKHR     Surface()		  const { return _vkSurface; }
        VkCommandPool    CommandPool()	  const { return _vkCommandPool; }

        void CreateImageWithInfo(const VkImageCreateInfo &imageCreateInfo, VkMemoryPropertyFlags properties, VkImage &rImage, VkDeviceMemory &rImageMemory);

        SwapchainSupportDetails QueryPhysicalDeviceSwapchainSupport() { return QuerySwapchainSupport(_vkPhysicalDevice); }
        QueueFamilyIndices QueryPhysicalDeviceQueueFamilies() { return QueryQueueFamilies(_vkPhysicalDevice); }
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        uint32_t EnabledLayerCount()	 const { return static_cast<uint32_t>(_enabledLayerNames.size()); }
        uint32_t EnabledExtensionCount() const { return static_cast<uint32_t>(_enabledExtensionNames.size()); }
        uint32_t DeviceExtensionCount()  const { return static_cast<uint32_t>(_logicalDeviceExtensions.size()); }

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void CreateVulkanInstance();
        void CreateVulkanSurface() { _rSolVulkanWindow.CreateWindowSurface(_vkInstance, &_vkSurface); };
        void CreateVulkanPhysicalDevice();
        void CreateVulkanDevice();
        void CreateVulkanCommandPool();

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        void CreateVulkanDebugCallback();

        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
                                                                 int32_t messageCode, const char *layerPrefix, const char *message, void *pUserData);
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        // Helper Functions
        SwapchainSupportDetails QuerySwapchainSupport(const VkPhysicalDevice &physicalDevice);
        QueueFamilyIndices QueryQueueFamilies(const VkPhysicalDevice &physicalDevice);

        SolVulkanWindow &_rSolVulkanWindow;
        ApplicationData &_rAppData;

        VkInstance       _vkInstance      { NULL };
        VkDevice         _vkDevice        { NULL };
        VkPhysicalDevice _vkPhysicalDevice{ NULL };
        VkSurfaceKHR     _vkSurface       { NULL };
        VkCommandPool    _vkCommandPool   { NULL };

        std::vector<const char *> _enabledLayerNames{};                           // Laptop
        //std::vector<const char *> _enabledLayerNames{ "VK_LAYER_NV_optimus" };  // Desktop
        std::vector<const char *> _enabledExtensionNames;   // Defined in CreateVulkanInstance()
        std::vector<const char *> _logicalDeviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    };
}
