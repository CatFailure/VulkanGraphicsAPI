#pragma once
#include "SolGrid.hpp"
#include "SolEvent.hpp"
#include "GameOfLifeSettings.hpp"
#include "SimulationSettings.hpp"

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
        void ResetNextGenerationDelayRemaining();

        SolEvent<> onUpdateAllCellStatesEvent;

    private:
        inline void CheckAdjacentNeighbourStates(const int xIndex, const int yIndex, const int zIndex, 
                                                 const glm::uvec3& gridDimensions, const bool* pCellStates, 
                                                 NeighbourCount_t& rLiveNeighbourCount);

        inline void CheckDiagonalNeighbourStates(const int xIndex, const int yIndex, const int zIndex,
                                                 const glm::uvec3& gridDimensions, const bool* pCellStates,
                                                 NeighbourCount_t& rLiveNeighbourCount);

        void CheckNeighbourState(const int xIndex, const int yIndex, const int zIndex,
                                 const glm::uvec3& gridDimensions, const bool* pCellStates, 
                                 NeighbourCount_t& rLiveNeighbourCount);

        void NextGeneration();

        float _nextGenerationDelayRemaining;

        SolGrid&            _rSolGrid;
        GameOfLifeSettings& _rGameOfLifeSettings;
        SimulationSettings& _rSimulationSettings;
    };
}
