#include "pch.hpp"
#include "GuiDiagnosticWindow.hpp"

namespace SolEngine::GUI
{
	GuiDiagnosticWindow::GuiDiagnosticWindow(const char *windowTitle, 
											 const bool isActive, 
											 const ImGuiWindowFlags windowFlags)
		: IGuiWindow(windowTitle, isActive, windowFlags)
	{}

	void GuiDiagnosticWindow::UpdateWindowContents()
	{
		ImGui::Begin(_windowTitle, &_isActive, _windowFlags);

		ImGui::Text("Hello World!");

		ImGui::End();
	}
}