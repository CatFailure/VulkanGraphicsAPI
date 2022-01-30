#include "pch.hpp"
#include "GuiDiagnosticWindow.hpp"

namespace SolEngine::GUI
{
	GuiDiagnosticWindow::GuiDiagnosticWindow(const char *windowTitle, 
											 const bool isActive, 
											 const ImGuiWindowFlags windowFlags, 
											 DiagnosticData &rDiagnosticData)
		: _diagnosticData(_rRealTimeDiagnosticData),
		  _rRealTimeDiagnosticData(rDiagnosticData),
		  IGuiWindow(windowTitle, isActive, windowFlags)
	{
		_onUpdateEvent.AddListener([this]() { OnUpdate_Method(); });
	}

	void GuiDiagnosticWindow::RenderWindowContents()
	{
		ImGui::Begin(_windowTitle, &_isActive, _windowFlags);
		ImGui::Text("Frame Rate: %G2", 1.f / _diagnosticData.deltaTime);

		ImGui::End();
	}

	void GuiDiagnosticWindow::OnUpdate_Method()
	{
		_diagnosticData = _rRealTimeDiagnosticData;
	}
}