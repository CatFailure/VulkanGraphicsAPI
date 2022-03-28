#include "GuiGridView.hpp"

namespace SolEngine::GUI::View
{
	GuiGridView::GuiGridView(GridSettings& rGridSettings, 
							 const SimulationSettings& simulationSettings)
		: _rGridSettings(rGridSettings),
		  _simulationSettings(simulationSettings)
	{
		InitGuiGridDimensions();
	}

	void GuiGridView::RenderViewContents()
	{
		if (!ImGui::CollapsingHeader(HEADER_GRID))	// This header should be collapsed by default
		{
			return;
		}

		RenderGridSizeInputInt3();
		RenderResetGridSizeButton();
	}

	void GuiGridView::InitGuiGridDimensions()
	{
		const glm::uvec3& gridDimensions = _rGridSettings.dimensions;

		_guiGridDimensions[0] = gridDimensions.x;
		_guiGridDimensions[1] = gridDimensions.y;
		_guiGridDimensions[2] = gridDimensions.z;
	}

	void GuiGridView::RenderGridSizeInputInt3()
	{
		if (ImGui::InputInt3(LABEL_GRID_DIMENSIONS, 
							 _guiGridDimensions,
							 ImGuiInputTextFlags_EnterReturnsTrue))
		{
			OnGridSizeChanged();
		}
	}

	void GuiGridView::RenderResetGridSizeButton()
	{
		ImGui::BeginDisabled(_simulationSettings.IsSimulationPlaying());
		{
			ImGui::PushID(RESET_GRID_DIMENSIONS_BUTTON_ID);	// Since there are multiple buttons with a "Reset" label, we have to define a unique ID here
			if (ImGui::Button(LABEL_GRID_DIMENSIONS_RESET))
			{
				OnGridSizeReset();
			}
			ImGui::PopID();
		}
		ImGui::EndDisabled();

		// Tooltip - Reset Simulation
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_SIMULATION_RESET);
		}
		ImGui::EndTooltip();
	}

	void GuiGridView::OnGridSizeChanged()
	{
		const int minGridAxisDimension = 1U;
		int& rGuiGridDimensionsX = _guiGridDimensions[0];
		int& rGuiGridDimensionsY = _guiGridDimensions[1];
		int& rGuiGridDimensionsZ = _guiGridDimensions[2];

		// Clamp the GUI Grid Dimensions so they can't go out of range.
		rGuiGridDimensionsX = Clamp(rGuiGridDimensionsX, minGridAxisDimension, MAX_CELLS_PER_AXIS_COUNT);
		rGuiGridDimensionsY = Clamp(rGuiGridDimensionsY, minGridAxisDimension, MAX_CELLS_PER_AXIS_COUNT);
		rGuiGridDimensionsZ = Clamp(rGuiGridDimensionsZ, minGridAxisDimension, MAX_CELLS_PER_AXIS_COUNT);

		// Force the grid dimensions to be even for whole number grid bounds
		rGuiGridDimensionsX = ForceEven(rGuiGridDimensionsX);
		rGuiGridDimensionsY = ForceEven(rGuiGridDimensionsY);
		rGuiGridDimensionsZ = ForceEven(rGuiGridDimensionsZ);

		_rGridSettings.dimensions = glm::uvec3(rGuiGridDimensionsX, 
											   rGuiGridDimensionsY, 
											   rGuiGridDimensionsZ);

		_rGridSettings.isGridDimensionsChangeRequested = true;
	}

	void GuiGridView::OnGridSizeReset()
	{
		_rGridSettings.Reset();
		_rGridSettings.isGridDimensionsChangeRequested = true;

		// Update GUI side...
		InitGuiGridDimensions();
	}
}