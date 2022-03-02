#pragma once
#include "SolGrid.hpp"
#include "MarchingCubesHelpers.hpp"

using namespace Utility;

namespace SolEngine::System
{
    class GameOfLifeSystem
    {
    public:
        void CheckAllLiveNeighbours(SolGrid& rSolGrid);
        void UpdateAllCellStates(SolGrid& rSolGrid);

        void Update(const float deltaTime, SolGrid& rSolGrid);

    private:
        static constexpr unsigned char MIN_LIVE_NEIGHBOUR_COUNT{ 2U };
        static constexpr unsigned char MAX_LIVE_NEIGHBOUR_COUNT{ 3U };
        static constexpr float         NEXT_GENERATION_DELAY   { 1.5f };

        float _nextGenerationDelayRemaining{ NEXT_GENERATION_DELAY };
    };
}
