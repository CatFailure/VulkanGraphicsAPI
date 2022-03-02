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
        void CheckAllLiveNeighbours(SolGrid& rSolGrid);
        void UpdateAllCellStates(SolGrid& rSolGrid);

        void Update(const float deltaTime, SolGrid& rSolGrid);

        SolEvent<> onUpdateAllCellStatesEvent;

    private:
        static constexpr NeighbourCount_t MIN_LIVE_NEIGHBOUR_COUNT{ 2U };
        static constexpr NeighbourCount_t MAX_LIVE_NEIGHBOUR_COUNT{ 3U };
        static constexpr float            NEXT_GENERATION_DELAY   { .1f };

        float _nextGenerationDelayRemaining{ NEXT_GENERATION_DELAY };
    };
}
