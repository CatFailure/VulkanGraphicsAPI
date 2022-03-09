#pragma once
#include "SolClock.hpp"
#include "SolGameObject.hpp"
#include "SolDescriptorWriter.hpp"
#include "SimpleRenderSystem.hpp"
#include "GuiWindowManager.hpp"
#include "SolGrid.hpp"
#include "MarchingCubesSystem.hpp"
#include "GameOfLifeSystem.hpp"

#if _DEBUG_LAPTOP || NDEBUG_LAPTOP
#define DISABLE_IM_GUI  // Disables all Dear ImGui integration. (On by default on laptop due to insufficient Pool memory)
#endif  // _DEBUG_LAPTOP

using namespace SolEngine;
using namespace SolEngine::Data;
using namespace SolEngine::Descriptors;
using namespace SolEngine::GUI;
using namespace SolEngine::Interface;
using namespace SolEngine::Rendering;
using namespace SolEngine::System;

class Application : public IMonoBehaviour
{
public:
    Application() = delete;
    Application(const ApplicationData &appData);
    ~Application();
        
    void Run();

private:
    // Inherited via IMonoBehaviour
    virtual void Update(const float deltaTime) override;
    void Render();

    void CreateDescriptorPool();
    void SetupCamera();
    void SetupGrid();
    void SetupMarchingCubesSystem();
    void SetupGameOfLifeSystem();
    void SetupMarchingCubesDataEventCallbacks();

#ifndef DISABLE_IM_GUI
    void CreateGuiWindowManager();
#endif  // !DISABLE_IM_GUI

    ApplicationData _appData;

    DiagnosticData _diagnosticData{};
    GridData       _gridData      {};

    SolClock    _solClock;
    SolCamera   _solCamera;
    SolWindow   _solWindow;
    SolDevice   _solDevice;
    SolRenderer _solRenderer;

    std::unique_ptr<SolDescriptorPool> _pSolDescriptorPool;

#ifndef DISABLE_IM_GUI
    std::unique_ptr<GuiWindowManager>  _pGuiWindowManager;
#endif  // !DISABLE_IM_GUI

    std::unique_ptr<SolGrid>             _pSolGrid            { nullptr };
    std::unique_ptr<MarchingCubesSystem> _pMarchingCubesSystem{ nullptr };
    std::unique_ptr<GameOfLifeSystem>    _pGameOfLifeSystem   { nullptr };

    static constexpr float CAM_NEAR{ 0.01f };
    static constexpr float CAM_FAR { 100.f };
};
