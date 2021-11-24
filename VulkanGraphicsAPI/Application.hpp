#pragma once
#define ENABLE_VULKAN_DEBUG_CALLBACK

#include "ApplicationData.hpp"

using namespace Utility;

namespace Engine
{
    class Application;

    typedef VkBool32(__stdcall Application:: *VulkanDebugReportCallback_t)(VkDebugReportFlagsEXT,
                                                                           VkDebugReportObjectTypeEXT,
                                                                           uint64_t,
                                                                           size_t,
                                                                           int32_t,
                                                                           const char *,
                                                                           const char *,
                                                                           void *);

    class Application : public IDisposable
    {
    public:
        Application() = delete;
        Application(const ApplicationData &appData);

        void Initialise(const WNDPROC wndProcCallback);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void SetupWin32Window(const WNDPROC wndProcCallback);

        void SetupVulkanInstance();
        void SetupVulkanInstance_InitVkInstance();
        void SetupVulkanInstance_InitVkSurface();

        void SetupPhysicalDevices();
        void SetupVulkanDevice();

        void SetupSwapchain();
        void SetupSwapchain_CreateSwapchain();
        void SetupSwapchain_CreateImages();
        void SetupSwapchain_CreateImageViews();

        void PrintDeviceMemoryCapabilities();

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        void SetupVulkanInstance_InitVkDebugCallback();

        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
                                                                 int32_t messageCode, const char *layerPrefix, const char *message, void *pUserData);
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        ApplicationData _appData;

        // Desktop (NVIDIA Card)
        constexpr static uint32_t ENABLED_LAYER_COUNT{ 1 };
        std::array<const char *, ENABLED_LAYER_COUNT> _enabledLayerNames{ "VK_LAYER_NV_optimus" };

        // Laptop (No NVIDIA Card)
        //constexpr static uint32_t ENABLED_LAYER_COUNT{ 0 };
        //std::array<const char *, ENABLED_LAYER_COUNT> _enabledLayerNames{ NULL };

        VkInstance _vkInstance{ NULL };
        VkSurfaceKHR _vkSurface{ NULL };

        VkPhysicalDevice _vkPhysicalDevice{ NULL };
        VkDevice _vkLogicalDevice{ NULL };

        VkSwapchainKHR _vkSwapchain{ NULL };

        constexpr static uint32_t SWAPCHAIN_BUFFER_COUNT{ 2 };

        std::vector<VkImage> _vkSwapchainImages;
        std::vector<VkImageView> _vkSwapchainImageViews;

        uint32_t _surfaceBufferWidth{ 0 }, _surfaceBufferHeight{ 0 };
    };
}
