#pragma once

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

        void RecreateSwapchain();

        uint32_t _physDeviceCount{ 0 }, _physDeviceQueueFamilyCount{ 0 };
        uint32_t _commandBufferCount{ 1 };
        VkCommandBuffer _vkDrawCommandBuffer{ NULL };

        ApplicationData &_rAppData;

        std::unique_ptr<SolVulkanWindow> _pSolVulkanWindow;
        std::unique_ptr<SolVulkanDevice> _pSolVulkanDevice;
        std::unique_ptr<SolVulkanSwapchain> _pSolVulkanSwapchain;
    };
}
