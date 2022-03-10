#pragma once
#include "SolEvent.hpp"

using namespace SolEngine::Events;

namespace SolEngine::Settings
{
	struct GeneralSettings
	{
		void Reset()
		{
			GeneralSettings defaultSettings{};

			generation		    = 0U;
			nextGenerationDelay = defaultSettings.nextGenerationDelay;
			isPaused			= defaultSettings.isPaused;
			isResetRequested	= false;
		}

		size_t generation		  { 0U };
		float  nextGenerationDelay{ 1.f };
		bool   isPaused			  { false };
		bool   isResetRequested	  { false };
	};
}