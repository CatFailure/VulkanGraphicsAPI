#pragma once

#include "SolClock.hpp"
#include "SolDescriptorWriter.hpp"
#include "SimpleRenderSystem.hpp"
#include "GuiWindowManager.hpp"
#include "SolGrid.hpp"
#include "MarchingCubesSystem.hpp"
#include "GameOfLifeSystem.hpp"
#include "CameraController.hpp"
#include "SettingsBundle.hpp"
#include "CSVPerformanceProfiler.hpp"

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
    Application(const ApplicationData& appData, DiagnosticData& rDiagnosticData, SettingsBundle& rSettings);
    ~Application();
        
    void Run();

private:
    static constexpr float CAMERA_MOVE_SPEED{ 1.5f };
    static constexpr float GAME_OBJECT_ROT_SPEED{ glm::radians(.5f) };

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

    ApplicationData _appData;
    SettingsBundle& _rSettings;
    DiagnosticData& _rDiagnosticData;

    SolClock    _solClock;
    SolWindow   _solWindow;
    SolDevice   _solDevice;
    SolRenderer _solRenderer;

    CSVPerformanceProfiler _performanceProfiler;

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
