#pragma once
#include "SolEvent.hpp"
#include "SimulationState.hpp"

using namespace SolEngine::Enumeration;
using namespace SolEngine::Events;

namespace SolEngine::Settings
{
	struct SimulationSettings
	{
		void Reset()
		{
			SimulationSettings defaultSettings{};

			generation		    = 0U;
			nextGenerationDelay = defaultSettings.nextGenerationDelay;
			simulationState		= defaultSettings.simulationState;
			wasResetRequested	= true;
		}

		size_t			generation		   { 0U };
		float			nextGenerationDelay{ 1.f };
		SimulationState simulationState	   { SimulationState::PAUSED };
		bool			wasResetRequested  { false };
	};
}