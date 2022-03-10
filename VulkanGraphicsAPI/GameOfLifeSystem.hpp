#pragma once
#include "SolGrid.hpp"
#include "MarchingCubesHelpers.hpp"
#include "SolEvent.hpp"
#include "GameOfLifeSettings.hpp"
#include "SimulationSettings.hpp"

using namespace Utility;
using namespace SolEngine::Events;
using namespace SolEngine::Settings;

namespace SolEngine::System
{
    class GameOfLifeSystem
    {
    public:
        GameOfLifeSystem(SolGrid& rSolGrid, GameOfLifeSettings& rGameOfLifeSettings, SimulationSettings& rSimulationSettings);

        void CheckAllCellNeighbours();
        void UpdateAllCellStates();

        void Update(const float deltaTime);
        void ForceUpdateCellStates();

        SolEvent<> onUpdateAllCellStatesEvent;

    private:
        void CheckNeighbourState(const uint32_t xIndex, const uint32_t yIndex, const uint32_t zIndex,
                                 const glm::vec3& scaledDimensions, const bool* pCellStates, 
                                 NeighbourCount_t& rLiveNeighbourCount);

        void NextGeneration();

        float _nextGenerationDelayRemaining;

        SolGrid&            _rSolGrid;
        GameOfLifeSettings& _rGameOfLifeSettings;
        SimulationSettings& _rSimulationSettings;
    };
}
