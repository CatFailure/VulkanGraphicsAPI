#pragma once
#include "imgui.h"
#include "GridSettings.hpp"
#include "SimulationSettings.hpp"
#include "GuiStrings.hpp"
#include "Helpers.hpp"

using namespace Utility;
using namespace SolEngine::GUI;
using namespace SolEngine::Settings;

namespace SolEngine::GUI::View
{
	class GuiGridView
	{
	public:
		GuiGridView() = delete;
		GuiGridView(GridSettings& rGridSettings, const SimulationSettings& simulationSettings);

		void RenderViewContents();

	private:
		static constexpr const char* RESET_GRID_DIMENSIONS_BUTTON_ID {"Label##ResetGridDimensions"};

		void InitGuiGridDimensions();
		void RenderGridSizeInputInt3();
		void RenderResetGridSizeButton();

		void OnGridSizeChanged();
		void OnGridSizeReset();

		GridSettings			  _defaultGridSettings{};
		GridSettings&			  _rGridSettings;
		const SimulationSettings& _simulationSettings;	// Only need a reference to know if the simulation is currently playing

		int _guiGridDimensions[3]{ 0 };
	};
}
