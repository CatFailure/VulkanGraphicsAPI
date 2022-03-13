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

			seed				= defaultSettings.seed;
			generation		    = defaultSettings.generation;
			nextGenerationDelay = defaultSettings.nextGenerationDelay;
			simulationState		= defaultSettings.simulationState;
			wasSimulationResetRequested	= true;
		}

		bool wasSimulationResetRequested{ false };

		int				seed			   { 0 };
		size_t			generation		   { 0U };
		float			nextGenerationDelay{ 1.f };
		SimulationState simulationState	   { SimulationState::PAUSED };
	};
}