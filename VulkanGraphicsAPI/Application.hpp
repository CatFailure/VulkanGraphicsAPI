#pragma once

#include "SolWindow.hpp"
#include "SolModel.hpp"
#include "SolGameObject.hpp"
#include "SolRenderer.hpp"
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

        std::shared_ptr<SolModel> CreateCubeModel(SolDevice &rDevice, const glm::vec3 &offset);

    private:
        // Inherited via IDisposable
        virtual void Dispose() override;

        // Inherited via IMonoBehaviour
        virtual void Update(const float deltaTime) override;
        void Draw();

        void LoadGameObjects();

        ApplicationData _appData;

        SolCamera   _solCamera{};
        SolWindow   _solWindow;
        SolDevice   _solDevice;
        SolRenderer _solRenderer;

        std::vector<SolGameObject> _gameObjects;
    };
}
