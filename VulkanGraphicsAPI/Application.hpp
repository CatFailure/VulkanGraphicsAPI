#pragma once
#define ENABLE_VULKAN_DEBUG_CALLBACK
#define DEPTH_BUFFER

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

        void SetupVulkanPhysicalDevices();
        void SetupVulkanDevice();

        void SetupVulkanSwapchain();
        void SetupVulkanSwapchain_CreateSwapchain();
        void SetupVulkanSwapchain_CreateImages();
        void SetupVulkanSwapchain_CreateImageViews();

        void PrintDeviceMemoryCapabilities();

        void SetupVulkanDrawCommandBuffer();

        void SetupFrameBufferRenderPass();

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        void SetupVulkanInstance_InitVkDebugCallback();

        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
                                                                 int32_t messageCode, const char *layerPrefix, const char *message, void *pUserData);
#endif // ENABLE_VULKAN_DEBUG_CALLBACK
        constexpr static uint32_t ENABLED_LAYER_COUNT{ 1 };     // Change to 0 for no NVIDIA card
        constexpr static uint32_t SWAPCHAIN_BUFFER_COUNT{ 2 };

        uint32_t _surfaceBufferWidth{ 0 }, _surfaceBufferHeight{ 0 };
        uint32_t _physDeviceCount{ 0 }, _physDeviceQueueFamilyCount{ 0 };
        uint32_t _swapChainImageCount{ 0 };
        uint32_t _commandBufferCount{ 1 };

        ApplicationData _appData;

        // Desktop (NVIDIA Card)
        std::array<const char *, ENABLED_LAYER_COUNT> _enabledLayerNames{ "VK_LAYER_NV_optimus" };

        // Laptop (No NVIDIA Card)
        //std::array<const char *, ENABLED_LAYER_COUNT> _enabledLayerNames{ NULL };

        VkInstance _vkInstance{ NULL };
        VkSurfaceKHR _vkSurface{ NULL };

        VkPhysicalDevice _vkPhysicalDevice{ NULL };
        VkDevice _vkLogicalDevice{ NULL };

        VkSwapchainKHR _vkSwapchain{ NULL };

        std::vector<VkImage> _vkSwapchainImages;
        std::vector<VkImageView> _vkSwapchainImageViews;

        VkCommandPool _vkCommandPool{ NULL };
        VkCommandBuffer _vkDrawCommandBuffer{ NULL };
    };
}
