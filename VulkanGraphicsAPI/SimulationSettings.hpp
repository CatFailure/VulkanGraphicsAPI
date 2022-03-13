#pragma once
#include "SolEvent.hpp"
#include "SimulationState.hpp"

using namespace SolEngine::Enumeration;
using namespace SolEngine::Events;

namespace SolEngine::Settings
{
	struct SimulationSettings
	{
		void ResetGeneration()
		{
			generation = 0U;
		}

		void ResetSeed()
		{
			SimulationSettings defaultSettings{};
			seed = defaultSettings.seed;
		}

		void ResetSpeed()
		{
			SimulationSettings defaultSettings{};

			speed = defaultSettings.speed;
			onSimulationSpeedChangedEvent.Invoke(speed);
		}

		SolEvent<float> onSimulationSpeedChangedEvent;

		bool isSimulationResetRequested{ false };

		int				seed	  { 0 };
		size_t			generation{ 0U };
		float			speed	  { 1.f };
		SimulationState state	  { SimulationState::PAUSED };
	};
}