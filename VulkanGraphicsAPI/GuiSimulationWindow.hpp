#pragma once
#include "IGuiWindow.hpp"
#include "SimulationSettings.hpp"
#include "Constants.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Settings;
using namespace SolEngine::Interface;

namespace SolEngine::GUI
{
	class GuiSimulationWindow : public IGuiWindow
	{
	public:
		GuiSimulationWindow() = delete;
		GuiSimulationWindow(const char* windowTitle, const bool isActive, const ImGuiWindowFlags windowFlags, SimulationSettings& rSimulationSettings);

		// Inherited via IGuiWindow
		virtual void RenderWindowContents() override;

	private:
		SimulationSettings& _rSimulationSettings;
	};
}
