#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "SolClock.hpp"
#include "SolWindow.hpp"
#include "SolModel.hpp"
#include "SolGameObject.hpp"
#include "SolRenderer.hpp"
#include "SolDescriptorWriter.hpp"
#include "SimpleRenderSystem.hpp"

using namespace SolEngine;
using namespace SolEngine::Data;
using namespace SolEngine::Descriptors;
using namespace SolEngine::Interface;
using namespace SolEngine::Rendering;

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

    // TEMP
    void InitImGUI();
    void InitImGUIFont();

    ApplicationData _appData;

    SolClock    _solClock;
    SolCamera   _solCamera;
    SolWindow   _solWindow;
    SolDevice   _solDevice;
    SolRenderer _solRenderer;

    std::unique_ptr<SolDescriptorPool> _pSolDescriptorPool;

    std::vector<SolGameObject> _gameObjects;

    // TEMP: Testing out ImGUI
    bool _drawGameObjects{ true };
    float _gameObjectScale{ 1.f };

    static constexpr float CAM_NEAR{ 0.01f };
    static constexpr float CAM_FAR { 100.f };
};
