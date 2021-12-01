#pragma once
#define DEPTH_BUFFER

#include "SolVulkanWindow.hpp"
#include "SolVulkanDevice.hpp"
#include "SolVulkanSwapchain.hpp"

using namespace Utility;

namespace SolEngine
{
    class Application : public IDisposable
    {
    public:
        Application() = delete;
        Application(ApplicationData &rAppData);

        void Run();

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void PrintDeviceMemoryCapabilities();

        void SetupVulkanDrawCommandBuffer();

        uint32_t _physDeviceCount{ 0 }, _physDeviceQueueFamilyCount{ 0 };
        uint32_t _commandBufferCount{ 1 };
        VkCommandBuffer _vkDrawCommandBuffer{ NULL };

        //HWND &_rWinHandle;
        SolVulkanWindow _solVulkanWindow;
        ApplicationData &_rAppData;

        std::unique_ptr<SolVulkanDevice> _pSolVulkanDevice;
        std::unique_ptr<SolVulkanSwapchain> _pSolVulkanSwapchain;
    };
}
