#pragma once

#include "SolVulkanWindow.hpp"
#include "SolVulkanSwapchain.hpp"
#include "SolVulkanPipeline.hpp"
#include "SolVulkanModel.hpp"
#include "SolVulkanGameObject.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Interface;

namespace SolEngine
{
    class Application : public IDisposable, public IMonoBehaviour
    {
    public:
        Application() = delete;
        Application(ApplicationData &rAppData);
        
        void Run();

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void LoadGameObjects();
        void RenderGameObjects(const VkCommandBuffer &commandBuffer);

        void PrintDeviceMemoryCapabilities();

        virtual void Update(const float deltaTime) override;
        void Draw();

        void CreatePipelineLayout();
        void CreatePipeline();

        void CreateCommandBuffers();
        void RecordCommandBuffer(const size_t imageIndex);
        void FreeCommandBuffers();

        void RecreateSwapchain();

        uint32_t _physDeviceCount{ 0 }, _physDeviceQueueFamilyCount{ 0 };

        ApplicationData &_rAppData;

        std::unique_ptr<SolVulkanWindow>    _pSolVulkanWindow;
        std::unique_ptr<SolVulkanDevice>    _pSolVulkanDevice;
        std::unique_ptr<SolVulkanSwapchain> _pSolVulkanSwapchain;
        std::unique_ptr<SolVulkanPipeline>  _pSolVulkanPipeline;

        VkPipelineLayout             _vkPipelineLayout;
        std::vector<VkCommandBuffer> _vkCommandBuffers;

        std::vector<SolVulkanGameObject> _gameObjects;
    };
}
