#pragma once

#include "SolVulkanWindow.hpp"
#include "SolVulkanPipeline.hpp"
#include "SolVulkanModel.hpp"
#include "SolVulkanGameObject.hpp"
#include "SolVulkanRenderer.hpp"

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

        void CreatePipelineLayout();
        void CreatePipeline();

        virtual void Update(const float deltaTime) override;

        ApplicationData _appData;

        std::unique_ptr<SolVulkanWindow>   _pSolVulkanWindow;
        std::unique_ptr<SolVulkanDevice>   _pSolVulkanDevice;
        std::unique_ptr<SolVulkanPipeline> _pSolVulkanPipeline;
        std::unique_ptr<SolVulkanRenderer> _pSolVulkanRenderer;

        VkPipelineLayout _vkPipelineLayout;

        std::vector<SolVulkanGameObject> _gameObjects;
    };
}
