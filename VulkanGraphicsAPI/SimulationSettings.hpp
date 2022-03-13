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

			// For UX we won't reset the seed or simulation speed
			// Those can be reset seperately...
			generation					= defaultSettings.generation;
			simulationState				= defaultSettings.simulationState;
			wasSimulationResetRequested = true;
		}

		void ResetSimulationSeed()
		{
			SimulationSettings defaultSettings{};

			seed = defaultSettings.seed;
			wasSimulationResetRequested = true;	// Change in seed requires full reset
		}

		void ResetSimulationSpeed()
		{
			SimulationSettings defaultSettings{};

			simulationSpeed = defaultSettings.simulationSpeed;
			onSimulationSpeedChangedEvent.Invoke(simulationSpeed);
		}

		SolEvent<float> onSimulationSpeedChangedEvent;

		bool wasSimulationResetRequested{ false };

		int				seed		   { 0 };
		size_t			generation	   { 0U };
		float			simulationSpeed{ 1.f };
		SimulationState simulationState{ SimulationState::PAUSED };
	};
}