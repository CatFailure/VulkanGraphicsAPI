#pragma once
#include "IGuiWindow.hpp"

using namespace SolEngine::Interface;

namespace SolEngine::GUI
{
	class GuiDiagnosticWindow : public IGuiWindow
	{
	public:
		GuiDiagnosticWindow() = default;
		GuiDiagnosticWindow(const char *windowTitle, const bool isActive = true, const ImGuiWindowFlags windowFlags = 0);

		// Inherited via IGuiWindow
		virtual void UpdateWindowContents() override;
	};
}
