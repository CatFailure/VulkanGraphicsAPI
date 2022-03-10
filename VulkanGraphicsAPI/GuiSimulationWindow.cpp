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
}