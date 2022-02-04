#pragma once
#include "SolClock.hpp"
#include "SolModel.hpp"
#include "SolGameObject.hpp"
#include "SolDescriptorWriter.hpp"
#include "SimpleRenderSystem.hpp"
#include "GuiWindowManager.hpp"
#include "MarchingCubesManager.hpp"

#if _DEBUG_LAPTOP || NDEBUG_LAPTOP
#define DISABLE_IM_GUI  // Disables all Dear ImGui integration. (On by default on laptop due to insufficient Pool memory)
#endif  // _DEBUG_LAPTOP

using namespace SolEngine;
using namespace SolEngine::Data;
using namespace SolEngine::Descriptors;
using namespace SolEngine::GUI;
using namespace SolEngine::Interface;
using namespace SolEngine::Manager;
using namespace SolEngine::Rendering;

class Application : private IDisposable, public IMonoBehaviour
{
public:
    Application() = delete;
    Application(const ApplicationData &appData);
    ~Application();
        
    void Run();

    std::shared_ptr<SolModel> CreateCubeModel(SolDevice &rDevice, const glm::vec3 &offset);
    std::shared_ptr<SolModel> CreateModel(SolDevice &rDevice, const Vertex *pVertices, const Index_t *pIndices, const glm::vec3 &offset);

private:
    // Inherited via IDisposable
    virtual void Dispose() override;

    // Inherited via IMonoBehaviour
    virtual void Update(const float deltaTime) override;
    void Render();

    void CreateDescriptorPool();
    void SetupCamera();
    void SetupGrid();

#ifndef DISABLE_IM_GUI
    void CreateGuiWindowManager();
#endif  // !DISABLE_IM_GUI

    void LoadGameObjects();

    ApplicationData _appData;
    DiagnosticData _diagnosticData{};

    SolClock    _solClock;
    SolCamera   _solCamera;
    SolWindow   _solWindow;
    SolDevice   _solDevice;
    SolRenderer _solRenderer;

    std::unique_ptr<SolDescriptorPool> _pSolDescriptorPool;

#ifndef DISABLE_IM_GUI
    std::unique_ptr<GuiWindowManager>  _pGuiWindowManager;
#endif  // !DISABLE_IM_GUI

    std::unique_ptr<MarchingCubesManager> _pMarchingCubesManager;

    std::vector<SolGameObject> _gameObjects;

    static constexpr float CAM_NEAR{ 0.01f };
    static constexpr float CAM_FAR { 100.f };
};
