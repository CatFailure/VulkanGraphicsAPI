#pragma once
#include "GuiWindowBase.hpp"
#include "GuiSimulationView.hpp"
#include "GuiGameOfLifeView.hpp"
#include "GuiGridView.hpp"
#include "GuiRenderSystemView.hpp"

using namespace SolEngine::GUI::View;
using namespace SolEngine::Interface;

namespace SolEngine::GUI
{
	class GuiSettingsWindow : public GuiWindowBase
	{
	public:
		GuiSettingsWindow() = delete;
		GuiSettingsWindow(const char* windowTitle, const bool isActive, const ImGuiWindowFlags windowFlags,
						  GameOfLifeSettings& rGameOfLifeSettings, SimulationSettings& rSimulationSettings, 
						  GridSettings& rGridSettings, RenderSettings& rRenderSettings);

		// Inherited via IGuiWindow
		virtual void Render() override;

	private:
		GuiSimulationView	_simulationView;
		GuiGameOfLifeView	_gameOfLifeView;
		GuiGridView			_gridView;
		GuiRenderSystemView _renderSystemView;
	};
}
