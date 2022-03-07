#pragma once
#include "SolGrid.hpp"
#include "MarchingCubesHelpers.hpp"
#include "SolEvent.hpp"

using namespace Utility;
using namespace SolEngine::Events;

namespace SolEngine::System
{
    class GameOfLifeSystem
    {
    public:
        GameOfLifeSystem(SolGrid& rSolGrid);

        void CheckAllLiveNeighbours();
        void UpdateAllCellStates();

        void Update(const float deltaTime);

        SolEvent<> onUpdateAllCellStatesEvent;

    private:
        static constexpr NeighbourCount_t MIN_LIVE_NEIGHBOUR_COUNT{ 2U };
        static constexpr NeighbourCount_t MAX_LIVE_NEIGHBOUR_COUNT{ 3U };
        static constexpr float            NEXT_GENERATION_DELAY   { .1f };

        void CheckNeighbourState(const size_t neighbourIndex, const bool* pCellStates, NeighbourCount_t& rLiveNeighbourCount);

        float _nextGenerationDelayRemaining{ NEXT_GENERATION_DELAY };

        SolGrid& _rSolGrid;
    };
}
