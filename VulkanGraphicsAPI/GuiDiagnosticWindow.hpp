#pragma once
#include "IGuiWindow.hpp"
#include "DiagnosticData.hpp"

using namespace SolEngine::Interface;

namespace SolEngine::GUI
{
	class GuiDiagnosticWindow : public IGuiWindow
	{
	public:
		GuiDiagnosticWindow() = default;
		GuiDiagnosticWindow(const char *windowTitle, const bool isActive, const ImGuiWindowFlags windowFlags, DiagnosticData &rDiagnosticData);

		// Inherited via IGuiWindow
		virtual void RenderWindowContents() override;

	private:
		void OnUpdate_Method();

		DiagnosticData &_rRealTimeDiagnosticData;
		DiagnosticData _diagnosticData;
	};
}
