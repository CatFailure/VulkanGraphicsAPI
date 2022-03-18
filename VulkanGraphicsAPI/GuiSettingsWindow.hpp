#pragma once
#include "IGuiWindow.hpp"
#include "GuiSimulationView.hpp"
#include "GuiGameOfLifeView.hpp"

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
		virtual void Render() override;

	private:
		GuiSimulationView _simulationView;
		GuiGameOfLifeView _gameOfLifeView;
	};
}
