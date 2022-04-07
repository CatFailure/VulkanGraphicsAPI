#include "GuiSettingsWindow.hpp"

namespace SolEngine::GUI
{
	GuiSettingsWindow::GuiSettingsWindow(const char* windowTitle, 
										 const bool isActive, 
										 const ImGuiWindowFlags windowFlags,
										 GameOfLifeSettings& rGameOfLifeSettings, 
										 SimulationSettings& rSimulationSettings, 
										 GridSettings& rGridSettings)
		: GuiWindowBase(windowTitle,
					 isActive,
					 windowFlags),
		  _gameOfLifeView(rGameOfLifeSettings),
		  _simulationView(rSimulationSettings),
		  _gridView(rGridSettings, 
					rSimulationSettings)
	{}

	void GuiSettingsWindow::Render()
	{
		ImGui::Begin(_windowTitle, 
					 &_isActive, 
					 _windowFlags);

		_simulationView.RenderViewContents();
		ImGui::Separator();
		_gameOfLifeView.RenderViewContents();
		ImGui::Separator();
		_gridView.RenderViewContents();

		ImGui::End();
	}
}
