#include "GuiGameOfLifeWindow.hpp"

namespace SolEngine::GUI
{
	GuiGameOfLifeWindow::GuiGameOfLifeWindow(const char* windowTitle, 
											 const bool isActive, 
											 const ImGuiWindowFlags windowFlags, 
											 GameOfLifeSettings& rGameOfLifeSettings)
		: IGuiWindow(windowTitle,
					 isActive,
					 windowFlags),
		_rGameOfLifeSettings(rGameOfLifeSettings)
	{}
}