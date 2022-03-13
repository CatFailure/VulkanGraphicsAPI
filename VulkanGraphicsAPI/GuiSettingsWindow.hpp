#pragma once
#include "IGuiWindow.hpp"
#include "Constants.hpp"
#include "GameOfLifeSettings.hpp"
#include "SimulationSettings.hpp"
#include "GuiTooltips.hpp"
#include "Helpers.hpp"
#include "SimulationState.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Enumeration;
using namespace SolEngine::Interface;
using namespace SolEngine::Settings;

namespace SolEngine::GUI
{
	class GuiSettingsWindow : public IGuiWindow
	{
	public:
		GuiSettingsWindow() = delete;
		GuiSettingsWindow(const char* windowTitle, const bool isActive, const ImGuiWindowFlags windowFlags,
						  GameOfLifeSettings& rGameOfLifeSettings, SimulationSettings& rSimulationSettings);

		// Inherited via IGuiWindow
		virtual void RenderWindowContents() override;

	private:
		static constexpr float MIN_SIMULATION_SPEED				{ 0.f };
		static constexpr float MAX_SIMULATION_SPEED				{ 5.f };
		static constexpr int   MIN_SIMULATION_SEED				{ 0 };
		static constexpr int   MAX_SIMULATION_SEED				{ 2147483647 };
		static constexpr float SIMULATION_SPEED_SLIDER_STEP		{ 0.05f };
		static constexpr float SIMULATION_SPEED_SLIDER_FAST_STEP{ 0.1f };
		static constexpr int   SIMULATION_SEED_INPUT_STEP		{ 1 };
		static constexpr int   SIMULATION_SEED_INPUT_FAST_STEP	{ 100 };

		bool IsSimulationPlaying() const { return _rSimulationSettings.simulationState == SimulationState::PLAY; }

		void RenderSimulationSettings();
		void RenderGameOfLifeSettings();

		void RenderSimulationGenerationText();
		void RenderSimulationSeedInput();
		void RenderSimulationSimulationSpeedInput();
		void RenderSimulationPauseButton();
		void RenderSimulationResetButton();

		void RenderGameOfLifeMinLiveNeighboursSlider(int& rMinLiveNeighbourCount, const int maxLiveNeighbourCount);
		void RenderGameOfLifeMaxLiveNeighboursSlider(int& rMaxLiveNeighbourCount, const int minLiveNeighbourCount);
		void RenderGameOfLifeReproductionLiveNeighboursSlider(int& rReproductionLiveNeighbourCount);
		void RenderGameOfLifeResetButton();

		void OnMinLiveNeighboursChanged(const int value);
		void OnMaxLiveNeighboursChanged(const int value);
		void OnReproductionLiveNeighboursChanged(const int value);
		void OnSimulationSeedChanged();
		void OnSimulationSpeedChanged();
		void OnSimulationStateToggled();
		void OnSimulationReset();
		void SetSimulationState(const SimulationState state);

		GameOfLifeSettings  _defaultGameOfLifeSettings{};
		SimulationSettings  _defaultSimulationSettings{};

		GameOfLifeSettings& _rGameOfLifeSettings;
		SimulationSettings&	_rSimulationSettings;

		float			_simulationSpeed	  { 0.f };
		int				_simulationSeed		  { 0 };
		std::string     _toggleStateButtonText{};
	};
}
