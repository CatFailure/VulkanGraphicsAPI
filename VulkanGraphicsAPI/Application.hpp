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
        Application(const ApplicationData &appData);

        // Inherited via IDisposable
        virtual void Dispose() override;
        
        void Run();

    private:
        // Inherited via IMonoBehaviour
        virtual void Update(const float deltaTime) override;
        void Draw();

        void LoadGameObjects();
        void RenderGameObjects(const VkCommandBuffer commandBuffer);

        void CreatePipelineLayout();
        void CreatePipeline();

        ApplicationData _appData;

        SolVulkanWindow   _solWindow;
        SolVulkanDevice   _solDevice;
        SolVulkanRenderer _solRenderer;
        std::unique_ptr<SolVulkanPipeline> _pSolPipeline{ nullptr };

        VkPipelineLayout _vkPipelineLayout;

        std::vector<SolVulkanGameObject> _gameObjects;
    };
}
