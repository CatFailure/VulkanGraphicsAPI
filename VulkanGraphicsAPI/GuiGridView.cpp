#include "GuiGridView.hpp"

namespace SolEngine::GUI::View
{
	GuiGridView::GuiGridView(GridSettings& rGridSettings, 
							 SimulationSettings& rSimulationSettings)
		: _rGridSettings(rGridSettings),
		  _rSimulationSettings(rSimulationSettings)
	{
		InitGuiGridDimensions();
	}

	void GuiGridView::RenderViewContents()
	{
		if (!ImGui::CollapsingHeader(HEADER_GRID))	// This header should be collapsed by default
		{
			return;
		}

		RenderGridDimensionsInputInt3();
		RenderResetGridSizeButton();
	}

	void GuiGridView::InitGuiGridDimensions()
	{
		const glm::uvec3& gridDimensions = _rGridSettings.dimensions;

		_guiGridDimensions[0] = gridDimensions.x;
		_guiGridDimensions[1] = gridDimensions.y;
		_guiGridDimensions[2] = gridDimensions.z;
	}

	void GuiGridView::RenderGridDimensionsInputInt3()
	{
		ImGui::BeginDisabled(_rSimulationSettings.IsSimulationPlaying());
		ImGui::InputInt3(LABEL_GRID_DIMENSIONS, _guiGridDimensions);
		ImGui::EndDisabled();

		// Update values after user is finished
		if (ImGui::IsItemDeactivatedAfterEdit())
		{
			OnGridSizeChanged();
		}

		// Tooltip - Change Grid Dimensions
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			const glm::uvec3 defaultGridDimensions = _defaultGridSettings.dimensions;

			ImGui::Text(TOOLTIP_GRID_DIMENSIONS, defaultGridDimensions.x, defaultGridDimensions.y, defaultGridDimensions.z);
		}
		ImGui::EndTooltip();
	}

	void GuiGridView::RenderResetGridSizeButton()
	{
		ImGui::BeginDisabled(_rSimulationSettings.IsSimulationPlaying());
		{
			ImGui::PushID(RESET_GRID_DIMENSIONS_BUTTON_ID);	// Since there are multiple buttons with a "Reset" label, we have to define a unique ID here
			if (ImGui::Button(LABEL_GRID_DIMENSIONS_RESET))
			{
				OnGridSizeReset();
			}
			ImGui::PopID();
		}
		ImGui::EndDisabled();

		// Tooltip - Reset Grid Dimensions
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GRID_DIMENSIONS_RESET);
		}
		ImGui::EndTooltip();
	}

	void GuiGridView::OnGridSizeChanged()
	{
		int& rGuiGridDimensionsX = _guiGridDimensions[0];
		int& rGuiGridDimensionsY = _guiGridDimensions[1];
		int& rGuiGridDimensionsZ = _guiGridDimensions[2];

		// Clamp the GUI Grid Dimensions so they can't go out of range.
		rGuiGridDimensionsX = Clamp(rGuiGridDimensionsX, MIN_CELLS_PER_AXIS_COUNT, MAX_CELLS_PER_AXIS_COUNT);
		rGuiGridDimensionsY = Clamp(rGuiGridDimensionsY, MIN_CELLS_PER_AXIS_COUNT, MAX_CELLS_PER_AXIS_COUNT);
		rGuiGridDimensionsZ = Clamp(rGuiGridDimensionsZ, MIN_CELLS_PER_AXIS_COUNT, MAX_CELLS_PER_AXIS_COUNT);

		// Force the grid dimensions to be even for whole number grid bounds
		rGuiGridDimensionsX = ForceEven(rGuiGridDimensionsX);
		rGuiGridDimensionsY = ForceEven(rGuiGridDimensionsY);
		rGuiGridDimensionsZ = ForceEven(rGuiGridDimensionsZ);

		// Changing grid dimensions re-generates cells so start over
		_rSimulationSettings.ResetGeneration();

		// Update grid settings and raise flags
		_rGridSettings.dimensions = glm::uvec3(rGuiGridDimensionsX, 
											   rGuiGridDimensionsY, 
											   rGuiGridDimensionsZ);

		_rGridSettings.isGridDimensionsChangeRequested = true;
	}

	void GuiGridView::OnGridSizeReset()
	{
		// Changing grid dimensions re-generates cells so start over
		_rSimulationSettings.ResetGeneration();

		// Reset Grid to defaults
		_rGridSettings.Reset();
		_rGridSettings.isGridDimensionsChangeRequested = true;

		// Update GUI side...
		InitGuiGridDimensions();
	}
}