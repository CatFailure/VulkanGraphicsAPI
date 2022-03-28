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
		static constexpr NeighbourCount_t DEFAULT_UNDERPOPULATION_COUNT_MOORE	   { 3U };
		static constexpr NeighbourCount_t DEFAULT_OVERPOPULATION_COUNT_MOORE	   { 6U };
		static constexpr NeighbourCount_t DEFAULT_REPRODUCTION_COUNT_MOORE		   { 6U };

		static constexpr NeighbourCount_t DEFAULT_UNDERPOPULATION_COUNT_VON_NEUMANN{ 2U };
		static constexpr NeighbourCount_t DEFAULT_OVERPOPULATION_COUNT_VON_NEUMANN { 3U };
		static constexpr NeighbourCount_t DEFAULT_REPRODUCTION_COUNT_VON_NEUMANN   { 3U };

		void Reset()
		{
			underpopulationCount = GetDefaultUnderpopulationCount();
			overpopulationCount  = GetDefaultOverpopulationCount();
			reproductionCount    = GetDefaultReproductionCount();
		}

		NeighbourCount_t GetDefaultUnderpopulationCount() const { return neighbourhoodType == NeighbourhoodType::MOORE_NEIGHBOURHOOD ? DEFAULT_UNDERPOPULATION_COUNT_MOORE : DEFAULT_UNDERPOPULATION_COUNT_VON_NEUMANN; }
		NeighbourCount_t GetDefaultOverpopulationCount()  const { return neighbourhoodType == NeighbourhoodType::MOORE_NEIGHBOURHOOD ? DEFAULT_OVERPOPULATION_COUNT_MOORE : DEFAULT_OVERPOPULATION_COUNT_VON_NEUMANN; }
		NeighbourCount_t GetDefaultReproductionCount()    const { return neighbourhoodType == NeighbourhoodType::MOORE_NEIGHBOURHOOD ? DEFAULT_REPRODUCTION_COUNT_MOORE : DEFAULT_REPRODUCTION_COUNT_VON_NEUMANN; }

		NeighbourhoodType neighbourhoodType	  { NeighbourhoodType::MOORE_NEIGHBOURHOOD };	// What cells are considered "Neighbours"?
		NeighbourCount_t  underpopulationCount{ 3U };										// Min number of live neighbours to keep a cell alive.
		NeighbourCount_t  overpopulationCount { 6U };										// Max number of live neighbours to keep a cell alive.
		NeighbourCount_t  reproductionCount   { 6U };										// Exact number of live neighbours to reproduce a cell.

		SolEvent<> onResetEvent;
	};
}