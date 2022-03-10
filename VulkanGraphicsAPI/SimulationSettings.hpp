#pragma once
#include "SolEvent.hpp"

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
			isPaused			= defaultSettings.isPaused;
		}

		size_t generation		  { 0U };
		float  nextGenerationDelay{ 1.f };
		bool   isPaused			  { false };

		SolEvent<> onResetEvent;
	};
}