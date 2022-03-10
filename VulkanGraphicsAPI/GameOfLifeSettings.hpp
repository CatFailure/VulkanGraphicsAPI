#pragma once
#include "Typedefs.hpp"

using namespace Utility;

namespace SolEngine::Settings
{
	struct GameOfLifeSettings
	{
		size_t currentGeneration  { 0U };
		float  nextGenerationDelay{ .1f };

		NeighbourCount_t minLiveNeighbourCount{ 2U };
		NeighbourCount_t maxLiveNeighbourCount{ 3U };
	};
}