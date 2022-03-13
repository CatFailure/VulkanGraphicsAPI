#pragma once
#include "IGuiWindow.hpp"
#include "Constants.hpp"
#include "GameOfLifeSettings.hpp"
#include "GuiSimulationView.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Enumeration;
using namespace SolEngine::GUI::View;
using namespace SolEngine::Interface;

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
		void RenderGameOfLifeSettings();

		void RenderGameOfLifeMinLiveNeighboursSlider(int& rMinLiveNeighbourCount, const int maxLiveNeighbourCount);
		void RenderGameOfLifeMaxLiveNeighboursSlider(int& rMaxLiveNeighbourCount, const int minLiveNeighbourCount);
		void RenderGameOfLifeReproductionLiveNeighboursSlider(int& rReproductionLiveNeighbourCount);
		void RenderGameOfLifeResetButton();

		void OnMinLiveNeighboursChanged(const int value);
		void OnMaxLiveNeighboursChanged(const int value);
		void OnReproductionLiveNeighboursChanged(const int value);

		GuiSimulationView _simulationView;

		GameOfLifeSettings  _defaultGameOfLifeSettings{};
		GameOfLifeSettings& _rGameOfLifeSettings;
	};
}
