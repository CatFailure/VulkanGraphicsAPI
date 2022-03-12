#pragma once
#include "IGuiWindow.hpp"
#include "Constants.hpp"
#include "GameOfLifeSettings.hpp"
#include "SimulationSettings.hpp"
#include "GuiTooltips.hpp"
#include "Helpers.hpp"

using namespace SolEngine::Data;
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
		static constexpr float SIMULATION_SPEED_SLIDER_STEP		{ 0.05f };
		static constexpr float SIMULATION_SPEED_SLIDER_FAST_STEP{ 0.1f };

		void RenderGeneralSettings();
		void RenderGameOfLifeSettings();

		void RenderGeneralGenerationText();
		void RenderGeneralSimulationSpeedInputFloat();
		void RenderGeneralPauseButton();

		void RenderGameOfLifeMinLiveNeighbours(int& rMinLiveNeighbourCount, const int maxLiveNeighbourCount);
		void RenderGameOfLifeMaxLiveNeighbours(int& rMaxLiveNeighbourCount, const int minLiveNeighbourCount);
		void RenderGameOfLifeReproductionLiveNeighbours(int& rReproductionLiveNeighbourCount);
		void RenderGameOfLifeResetButton();

		void OnMinLiveNeighboursChanged(const int value);
		void OnMaxLiveNeighboursChanged(const int value);
		void OnReproLiveNeighboursChanged(const int value);
		void OnSimulationSpeedChanged();

		GameOfLifeSettings& _rGameOfLifeSettings;
		SimulationSettings&	_rSimulationSettings;

		float _simulationSpeed{ 0.f };
	};
}
