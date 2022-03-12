#pragma once
#include "SolEvent.hpp"
#include "Typedefs.hpp"

using namespace Utility;
using namespace SolEngine::Events;

namespace SolEngine::Settings
{
	struct GameOfLifeSettings
	{
		void Reset()
		{
			GameOfLifeSettings defaultSettings{};

			minLiveNeighbourCount		   = defaultSettings.minLiveNeighbourCount;
			maxLiveNeighbourCount		   = defaultSettings.maxLiveNeighbourCount;
			reproductionLiveNeighbourCount = defaultSettings.reproductionLiveNeighbourCount;
		}

		NeighbourCount_t minLiveNeighbourCount		   { 2U };	// Min number of live neighbours to keep a cell alive.
		NeighbourCount_t maxLiveNeighbourCount		   { 3U };	// Max number of live neighbours to keep a cell alive.
		NeighbourCount_t reproductionLiveNeighbourCount{ 3U };	// Exact number of live neighbours to reproduce a cell.

		SolEvent<> onResetEvent;
	};
}