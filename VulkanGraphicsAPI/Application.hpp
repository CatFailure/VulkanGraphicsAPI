#pragma once
#if _DEBUG_LAPTOP || NDEBUG_LAPTOP
//#define LAPTOP_BUILD
//#define DISABLE_IM_GUI	// Disables all Dear ImGui integration. (On by default on laptop due to insufficient Pool memory)
#endif // _DEBUG_LAPTOP || NDEBUG_LAPTOP

#include "SolClock.hpp"
#include "SolDescriptorWriter.hpp"
#include "SimpleRenderSystem.hpp"
#include "GuiWindowManager.hpp"
#include "SolGrid.hpp"
#include "MarchingCubesSystem.hpp"
#include "GameOfLifeSystem.hpp"
#include "CameraController.hpp"

using namespace SolEngine;
using namespace SolEngine::Data;
using namespace SolEngine::Descriptors;
using namespace SolEngine::GUI;
using namespace SolEngine::Rendering;
using namespace SolEngine::System;

class Application : public IMonoBehaviour
{
public:
    Application() = delete;
    Application(const ApplicationData& appData, DiagnosticData& rDiagnosticData,
                RenderSettings& rRenderSettings, CameraSettings& rCameraSettings, GridSettings& rGridSettings, 
                GameOfLifeSettings& rGameOfLifeSettings, SimulationSettings& rSimulationSettings);
    ~Application();
        
    void Run();

private:
    // Inherited via IMonoBehaviour
    virtual void Update(const float deltaTime) override;
    void Render();

    void CreateDescriptorPool();

    void SetupRandomNumberGenerator();
    void SetupCamera();
    void SetupGrid();
    void SetupMarchingCubesSystem();
    void SetupGameOfLifeSystem();
    void SetupEventCallbacks();

    void HandleUserInput(Transform& rGameObjectTransform);

    void CheckForSimulationResetFlag();
    void CheckForGridDimenionsChangedFlag();

#ifndef DISABLE_IM_GUI
    void CreateGuiWindowManager();
#endif  // !DISABLE_IM_GUI

    ApplicationData     _appData;
    CameraSettings&     _rCameraSettings;
    DiagnosticData&     _rDiagnosticData;
    RenderSettings&     _rRenderSettings;
    GridSettings&       _rGridSettings;
    GameOfLifeSettings& _rGameOfLifeSettings;
    SimulationSettings& _rSimulationSettings;

    SolClock    _solClock;
    SolWindow   _solWindow;
    SolDevice   _solDevice;
    SolRenderer _solRenderer;

    std::unique_ptr<SolDescriptorPool> _pSolDescriptorPool;

#ifndef DISABLE_IM_GUI
    std::unique_ptr<GuiWindowManager>  _pGuiWindowManager;
#endif  // !DISABLE_IM_GUI

    std::unique_ptr<SolCamera>           _pSolCamera          { nullptr };
    std::unique_ptr<SolGrid>             _pSolGrid            { nullptr };
    std::unique_ptr<SimpleRenderSystem>  _pRenderSystem       { nullptr };
    std::unique_ptr<MarchingCubesSystem> _pMarchingCubesSystem{ nullptr };
    std::unique_ptr<GameOfLifeSystem>    _pGameOfLifeSystem   { nullptr };
};
