#include "GuiSimulationWindow.hpp"

namespace SolEngine::GUI
{
	GuiSimulationWindow::GuiSimulationWindow(const char* windowTitle, 
											 const bool isActive, 
											 const ImGuiWindowFlags windowFlags, 
											 SimulationSettings& rSimulationSettings)
		: IGuiWindow(windowTitle, 
					 isActive, 
					 windowFlags),
		_rSimulationSettings(rSimulationSettings)
	{}

	void GuiSimulationWindow::RenderWindowContents()
	{
		ImGui::Begin(_windowTitle, 
					 &_isActive, 
					 _windowFlags);

		ImGui::Text("Generation: %zu", 
					_rSimulationSettings.generation);

		ImGui::End();
	}
}