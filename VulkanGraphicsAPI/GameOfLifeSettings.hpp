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

			currentGeneration     = 0U;
			nextGenerationDelay   = defaultSettings.nextGenerationDelay;
			minLiveNeighbourCount = defaultSettings.minLiveNeighbourCount;
			maxLiveNeighbourCount = defaultSettings.maxLiveNeighbourCount;

			isResetRequested = false;
		}

		size_t currentGeneration  { 0U };
		bool   isSimulationPaused { false };
		bool   isResetRequested	  { false };
		float  nextGenerationDelay{ .2f };

		NeighbourCount_t minLiveNeighbourCount{ 2U };
		NeighbourCount_t maxLiveNeighbourCount{ 3U };

		SolEvent<> onResetEvent;
	};
}