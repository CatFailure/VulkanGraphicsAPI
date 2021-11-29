#pragma once
#define DEPTH_BUFFER

#include "SolVulkanDevice.hpp"
#include "SolVulkanSwapchain.hpp"

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
        void PrintDeviceMemoryCapabilities();

        void SetupVulkanDrawCommandBuffer();

        uint32_t _physDeviceCount{ 0 }, _physDeviceQueueFamilyCount{ 0 };
        uint32_t _commandBufferCount{ 1 };

        HWND &_rWinHandle;
        ApplicationData &_rAppData;

        std::unique_ptr<SolVulkanDevice> _pSolVulkanDevice;
        std::unique_ptr<SolVulkanSwapchain> _pSolVulkanSwapchain;

        VkSwapchainKHR _vkSwapchain{ NULL };

        std::vector<VkImage> _vkSwapchainImages;
        std::vector<VkImageView> _vkSwapchainImageViews;

        VkCommandBuffer _vkDrawCommandBuffer{ NULL };
    };
}
