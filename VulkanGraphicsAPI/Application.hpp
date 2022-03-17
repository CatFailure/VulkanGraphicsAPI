#pragma once
#include "SolClock.hpp"
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
using namespace SolEngine::Rendering;
using namespace SolEngine::System;

struct GlobalUniformBufferObject
{
    glm::mat4 projectionViewMatrix{ 1.f };
};

class Application : public IMonoBehaviour
{
public:
    Application() = delete;
    Application(const ApplicationData& appData, DiagnosticData& rDiagnosticData, 
                GridSettings& rGridSettings, GameOfLifeSettings& rGameOfLifeSettings,
                SimulationSettings& rSimulationSettings);
    ~Application();
        
    void Run();

private:
    static constexpr float CAM_NEAR{ 0.01f };
    static constexpr float CAM_FAR { 100.f };

    // Inherited via IMonoBehaviour
    virtual void Update(const float deltaTime) override;
    void Render();

    void CreateDescriptorPool();

    void SetupRandomNumberGenerator();
    void SetupRenderSystem();
    void SetupCamera();
    void SetupGrid();
    void SetupMarchingCubesSystem();
    void SetupGameOfLifeSystem();
    void SetupEventCallbacks();

    void CheckForSimulationResetFlag();

#ifndef DISABLE_IM_GUI
    void CreateGuiWindowManager();
#endif  // !DISABLE_IM_GUI

    ApplicationData     _appData;
    DiagnosticData&     _rDiagnosticData;
    GridSettings&       _rGridSettings;
    GameOfLifeSettings& _rGameOfLifeSettings;
    SimulationSettings& _rSimulationSettings;

    SolClock    _solClock;
    SolCamera   _solCamera;
    SolWindow   _solWindow;
    SolDevice   _solDevice;
    SolRenderer _solRenderer;

    std::unique_ptr<SolDescriptorPool>      _pSolDescriptorPool;
    std::vector<std::unique_ptr<SolBuffer>> _uniformBufferObjects;
    std::vector<VkDescriptorSet>            _globalDescriptorSets;

#ifndef DISABLE_IM_GUI
    std::unique_ptr<GuiWindowManager>  _pGuiWindowManager;
#endif  // !DISABLE_IM_GUI

    std::unique_ptr<SolGrid>             _pSolGrid            { nullptr };
    std::unique_ptr<MarchingCubesSystem> _pMarchingCubesSystem{ nullptr };
    std::unique_ptr<GameOfLifeSystem>    _pGameOfLifeSystem   { nullptr };
    std::unique_ptr<SimpleRenderSystem>  _pRenderSystem       { nullptr };
};
