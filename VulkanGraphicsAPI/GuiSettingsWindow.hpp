#pragma once
#include "IGuiWindow.hpp"
#include "Constants.hpp"
#include "GameOfLifeSettings.hpp"
#include "GeneralSettings.hpp"

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
						  GameOfLifeSettings& rGameOfLifeSettings, GeneralSettings& rGeneralSettings);

		// Inherited via IGuiWindow
		virtual void RenderWindowContents() override;

	private:
		static constexpr float MIN_SIMULATION_SPEED{ 0.f };
		static constexpr float MAX_SIMULATION_SPEED{ 5.f };

		void RenderGameOfLifeSettings();
		void RenderGeneralSettings();

		void RenderGenerationText();
		void RenderSimulationSpeedInputFloat();
		void RenderPauseButton();

		void OnMinLiveNeighboursChanged(const int value);
		void OnMaxLiveNeighboursChanged(const int value);
		void OnSimulationSpeedChanged();

		GameOfLifeSettings& _rGameOfLifeSettings;
		GeneralSettings&	_rGeneralSettings;

		float _simulationSpeed		  { 0.f };
		float _simulationSpeedStep	  { 0.05f };
		float _simulationSpeedFastStep{ 0.1f };
	};
}
