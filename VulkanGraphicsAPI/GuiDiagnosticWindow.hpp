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
		void PushBackDeltaTime();

		static constexpr size_t MAX_BACKLOGGED_DELTA_TIMES{ 10 };
		static constexpr float TO_MILLISECONDS{ 1000.f };
		static constexpr float MIN_DELTA_TIME_SCALE{ 0.5f }, MAX_DELTA_TIME_SCALE{ 2.f };

		DiagnosticData &_rRealTimeDiagnosticData;
		DiagnosticData _diagnosticData;

		float _deltaTimeBacklog[MAX_BACKLOGGED_DELTA_TIMES]{ 0.f };
	};
}
