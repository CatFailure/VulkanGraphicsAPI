#pragma once
#include "SolEvent.hpp"
#include "Typedefs.hpp"
#include "NeighbourhoodType.hpp"

using namespace Utility;
using namespace SolEngine::Events;
using namespace SolEngine::Enumeration;

namespace SolEngine::Settings
{
	struct GameOfLifeSettings
	{
		void Reset()
		{
			GameOfLifeSettings defaultSettings{};

			underpopulationCount = defaultSettings.underpopulationCount;
			overpopulationCount  = defaultSettings.overpopulationCount;
			reproductionCount	 = defaultSettings.reproductionCount;
		}

		NeighbourhoodType neighbourhoodType	 { NeighbourhoodType::MOORE_NEIGHBOURHOOD };	// What cells are considered "Neighbours"?
		NeighbourCount_t underpopulationCount{ 2U };										// Min number of live neighbours to keep a cell alive.
		NeighbourCount_t overpopulationCount { 3U };										// Max number of live neighbours to keep a cell alive.
		NeighbourCount_t reproductionCount   { 3U };										// Exact number of live neighbours to reproduce a cell.

		SolEvent<> onResetEvent;
	};
}