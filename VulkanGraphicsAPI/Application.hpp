#pragma once

#include "SolVulkanWindow.hpp"
#include "SolVulkanModel.hpp"
#include "SolVulkanGameObject.hpp"
#include "SolVulkanRenderer.hpp"
#include "SimpleRenderSystem.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Interface;

namespace SolEngine
{
    class Application : private IDisposable, public IMonoBehaviour
    {
    public:
        Application() = delete;
        Application(const ApplicationData &appData);
        ~Application();
        
        void Run();

    private:
        // Inherited via IDisposable
        virtual void Dispose() override;

        // Inherited via IMonoBehaviour
        virtual void Update(const float deltaTime) override;
        void Draw();

        void LoadGameObjects();

        ApplicationData _appData;

        SolVulkanWindow   _solWindow;
        SolVulkanDevice   _solDevice;
        SolVulkanRenderer _solRenderer;

        std::vector<SolVulkanGameObject> _gameObjects;
    };
}
