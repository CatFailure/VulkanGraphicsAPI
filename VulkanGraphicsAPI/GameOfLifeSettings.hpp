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

			//neighbourhoodType	 = NeighbourhoodType::MOORE_NEIGHBOURHOOD;
			underpopulationCount = defaultSettings.underpopulationCount;
			overpopulationCount  = defaultSettings.overpopulationCount;
			reproductionCount	 = defaultSettings.reproductionCount;
		}

		static constexpr NeighbourCount_t DEFAULT_UNDERPOPULATION_COUNT_MOORE	   { 3U };
		static constexpr NeighbourCount_t DEFAULT_OVERPOPULATION_COUNT_MOORE	   { 6U };
		static constexpr NeighbourCount_t DEFAULT_REPRODUCTION_COUNT_MOORE		   { 6U };
		static constexpr NeighbourCount_t DEFAULT_UNDERPOPULATION_COUNT_VON_NEUMANN{ 3U };
		static constexpr NeighbourCount_t DEFAULT_OVERPOPULATION_COUNT_VON_NEUMANN { 6U };
		static constexpr NeighbourCount_t DEFAULT_REPRODUCTION_COUNT_VON_NEUMANN   { 6U };

		NeighbourhoodType neighbourhoodType	  { NeighbourhoodType::MOORE_NEIGHBOURHOOD };	// What cells are considered "Neighbours"?
		NeighbourCount_t  underpopulationCount{ 3U };										// Min number of live neighbours to keep a cell alive.
		NeighbourCount_t  overpopulationCount { 6U };										// Max number of live neighbours to keep a cell alive.
		NeighbourCount_t  reproductionCount   { 6U };										// Exact number of live neighbours to reproduce a cell.

		SolEvent<> onResetEvent;
	};
}