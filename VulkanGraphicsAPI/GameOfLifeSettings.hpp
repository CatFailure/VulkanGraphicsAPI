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

			minLiveNeighbourCount = defaultSettings.minLiveNeighbourCount;
			maxLiveNeighbourCount = defaultSettings.maxLiveNeighbourCount;
		}

		NeighbourCount_t minLiveNeighbourCount{ 2U };
		NeighbourCount_t maxLiveNeighbourCount{ 3U };

		SolEvent<> onResetEvent;
	};
}