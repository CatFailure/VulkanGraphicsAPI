#include "GuiSettingsWindow.hpp"

namespace SolEngine::GUI
{
	GuiSettingsWindow::GuiSettingsWindow(const char* windowTitle, 
										 const bool isActive, 
										 const ImGuiWindowFlags windowFlags,
										 GameOfLifeSettings& rGameOfLifeSettings, 
										 SimulationSettings& rSimulationSettings)
		: IGuiWindow(windowTitle,
					 isActive,
					 windowFlags),
		  _gameOfLifeView(rGameOfLifeSettings),
		  _simulationView(rSimulationSettings)
	{}

	void GuiSettingsWindow::Render()
	{
		ImGui::Begin(_windowTitle, 
					 &_isActive, 
					 _windowFlags);

		_simulationView.RenderViewContents();
		ImGui::Separator();
		_gameOfLifeView.RenderViewContents();

		ImGui::End();
	}
}
