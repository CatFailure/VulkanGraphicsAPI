#pragma once
#define ENABLE_VULKAN_DEBUG_CALLBACK

using namespace Utility;

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
        SolVulkanDevice(HWND &rWinHandle);
        ~SolVulkanDevice();

        // Public Accessors
        VkInstance       Instance()		  const { return _vkInstance; }
        VkDevice         Device()		  const { return _vkDevice; }
        VkPhysicalDevice PhysicalDevice() const { return _vkPhysicalDevice; }
        VkSurfaceKHR     Surface()		  const { return _vkSurface; }
        VkCommandPool    CommandPool()	  const { return _vkCommandPool; }

        uint32_t EnabledLayerCount()	 const { return static_cast<uint32_t>(_enabledLayerNames.size()); }
        uint32_t EnabledExtensionCount() const { return static_cast<uint32_t>(_enabledExtensionNames.size()); }
        uint32_t DeviceExtensionCount()  const { return static_cast<uint32_t>(_logicalDeviceExtensions.size()); }

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void CreateVulkanInstance();
        void CreateVulkanSurface();
        void CreateVulkanPhysicalDevice();
        void CreateVulkanDevice();
        void CreateVulkanCommandPool();

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        void CreateVulkanDebugCallback();

        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
                                                                 int32_t messageCode, const char *layerPrefix, const char *message, void *pUserData);
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        // TEMP: Will need to be wrapped in the future
        HWND &_rWinHandle;

        VkInstance       _vkInstance      { NULL };
        VkDevice         _vkDevice        { NULL };
        VkPhysicalDevice _vkPhysicalDevice{ NULL };
        VkSurfaceKHR     _vkSurface       { NULL };
        VkCommandPool    _vkCommandPool   { NULL };

        std::vector<const char *> _enabledLayerNames{ "VK_LAYER_NV_optimus" };
        std::vector<const char *> _enabledExtensionNames;   // Defined in CreateVulkanInstance()
        std::vector<const char *> _logicalDeviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    };
}
