#pragma once
#define DEPTH_BUFFER

#include "SolVulkanDevice.hpp"

using namespace Utility;

namespace SolEngine
{
    class Application : public IDisposable
    {
    public:
        Application() = delete;
        Application(HWND &rWinHandle, ApplicationData &rAppData);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void SetupVulkanSwapchain();
        void SetupVulkanSwapchain_CreateSwapchain();
        void SetupVulkanSwapchain_CreateImages();
        void SetupVulkanSwapchain_CreateImageViews();

        void PrintDeviceMemoryCapabilities();

        void SetupVulkanDrawCommandBuffer();

        void SetupFrameBufferRenderPass();

        constexpr static uint32_t ENABLED_LAYER_COUNT{ 1 };     // Change to 0 for no NVIDIA card
        constexpr static uint32_t SWAPCHAIN_BUFFER_COUNT{ 2 };

        uint32_t _surfaceBufferWidth{ 0 }, _surfaceBufferHeight{ 0 };
        uint32_t _physDeviceCount{ 0 }, _physDeviceQueueFamilyCount{ 0 };
        uint32_t _swapChainImageCount{ 0 };
        uint32_t _commandBufferCount{ 1 };

        HWND &_rWinHandle;
        ApplicationData &_rAppData;

        std::unique_ptr<SolVulkanDevice> _pSolVulkanDevice;

        VkSwapchainKHR _vkSwapchain{ NULL };

        std::vector<VkImage> _vkSwapchainImages;
        std::vector<VkImageView> _vkSwapchainImageViews;

        VkCommandBuffer _vkDrawCommandBuffer{ NULL };
    };
}
