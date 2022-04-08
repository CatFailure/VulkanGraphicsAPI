#pragma once
#include "imgui.h"
#include "SimulationSettings.hpp"
#include "GuiStrings.hpp"
#include "Helpers.hpp"

using namespace SolEngine::Settings;
using namespace	Utility;

namespace SolEngine::GUI::View
{
	class GuiSimulationView
	{
	public:
		GuiSimulationView() = delete;
		GuiSimulationView(SimulationSettings& rSimulationSettings);

		void RenderViewContents();

		void SetSimulationState(const SimulationState state);

	private:
		static constexpr float MIN_SIMULATION_SPEED			    { 0.f };
		static constexpr float MAX_SIMULATION_SPEED			    { 5.f };
		static constexpr int   MIN_SIMULATION_SEED			    { 0 };
		static constexpr int   MAX_SIMULATION_SEED			    { 2147483647 };
		static constexpr float SIMULATION_SPEED_SLIDER_STEP	    { 0.05f };
		static constexpr float SIMULATION_SPEED_SLIDER_FAST_STEP{ 0.1f };
		static constexpr int   SIMULATION_SEED_INPUT_STEP		{ 1 };
		static constexpr int   SIMULATION_SEED_INPUT_FAST_STEP  { 100 };
		static constexpr float SIMULATION_RESET_SEED_OFFSET	    { 263.f };	// Makes Reset Buttons line up - Since the GUI is auto-scaled this will never change

		static constexpr const char* RESET_SEED_BUTTON_ID {"Label##ResetSimulationSeed"};
		static constexpr const char* RESET_SPEED_BUTTON_ID{"Label##ResetSimulationSpeed"};

		void RenderSimulationGenerationText();
		void RenderSimulationSeedInput();
		void RenderSimulationResetSeedButton();
		void RenderSimulationSimulationSpeedInput();
		void RenderSimulationResetSpeedButton();
		void RenderSimulationPauseButton();
		void RenderSimulationResetButton();

		void OnSimulationSeedChanged();
		void OnSimulationSpeedChanged();

		void OnSimulationSeedReset();
		void OnSimulationSpeedReset();
		void OnSimulationReset();

		void OnSimulationStateToggled();

		const SimulationSettings _defaultSimulationSettings{};
		SimulationSettings&		 _rSimulationSettings;

		float		_simulationSpeed	  { 0.f };
		int			_simulationSeed		  { 0 };
		std::string _toggleStateButtonText{};
	};
}