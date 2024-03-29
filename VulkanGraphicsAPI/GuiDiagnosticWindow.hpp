#pragma once
#include "GuiWindowBase.hpp"
#include "DiagnosticData.hpp"
#include "Constants.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Interface;

namespace SolEngine::GUI
{
	class GuiDiagnosticWindow : public GuiWindowBase
	{
	public:
		GuiDiagnosticWindow() = delete;
		GuiDiagnosticWindow(const char* windowTitle, const bool isActive, const ImGuiWindowFlags windowFlags, DiagnosticData& rDiagnosticData);

		// Inherited via IGuiWindow
		virtual void Render() override;

	private:
		void OnUpdateData_Method();
		void PushBackDeltaTime();

		static constexpr size_t MAX_BACKLOGGED_DELTA_TIMES{ 20 };
		static constexpr float MIN_DELTA_TIME_SCALE{ 0.5f }, MAX_DELTA_TIME_SCALE{ 2.f };

		DiagnosticData& _rRealtimeDiagnosticData;
		DiagnosticData  _rDiagnosticData;

		float _deltaTimeBacklog[MAX_BACKLOGGED_DELTA_TIMES]{ 0.f };
	};
}
