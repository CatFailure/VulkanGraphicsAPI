#pragma once
#include "CameraSettings.hpp"
#include "GameOfLifeSettings.hpp"
#include "GridSettings.hpp"
#include "RenderSettings.hpp"
#include "SimulationSettings.hpp"

namespace SolEngine::Settings
{
    struct SettingsBundle
    {
        CameraSettings     cameraSettings    {};
        GameOfLifeSettings gameOfLifeSettings{};
        GridSettings       gridSettings      {};
        RenderSettings     renderSettings    {};
        SimulationSettings simulationSettings{};
    };
}
