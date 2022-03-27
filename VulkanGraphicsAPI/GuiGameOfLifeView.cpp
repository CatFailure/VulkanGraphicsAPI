#include "GuiGameOfLifeView.hpp"

namespace SolEngine::GUI::View
{
	GuiGameOfLifeView::GuiGameOfLifeView(GameOfLifeSettings& rGameOfLifeSettings)
		: _rGameOfLifeSettings(rGameOfLifeSettings)
	{}

	void GuiGameOfLifeView::RenderViewContents()
	{
		if (!ImGui::CollapsingHeader(HEADER_GAME_OF_LIFE))
		{
			return;
		}

		// Make a copy of these values for input sanitation (Must be ints as ImGUI does not support uints)
		// (See OnUnderpopulationValueChanged, OnOverpopulationValueChanged and OnReproductionValueChanged)
		int underpopulationCount    = (int)_rGameOfLifeSettings.underpopulationCount;
		int overpopulationCount	    = (int)_rGameOfLifeSettings.overpopulationCount;
		int reproLiveNeighbourCount = (int)_rGameOfLifeSettings.reproductionCount;

		RenderGameOfLifeNeighbourhoodCombo(_rGameOfLifeSettings.neighbourhoodType);
		RenderGameOfLifeUnderpopulationSlider(underpopulationCount, overpopulationCount);
		RenderGameOfLifeOverpopulationSlider(overpopulationCount, underpopulationCount);
		RenderGameOfLifeReproductionSlider(reproLiveNeighbourCount);
		RenderGameOfLifeResetButton();
	}

	void GuiGameOfLifeView::RenderGameOfLifeNeighbourhoodCombo(NeighbourhoodType& rNeighbourhoodType)
	{
		int selectedType = (int)rNeighbourhoodType;

		if (ImGui::Combo("Neighbourhood Type",
						 &selectedType, 
						 _neighbourhoodTypes, 
						 IM_ARRAYSIZE(_neighbourhoodTypes)))
		{
			rNeighbourhoodType = (NeighbourhoodType)selectedType;
		}

		// Tooltip - Neighbourhood Type
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GAME_OF_LIFE_NEIGHBOURHOOD_TYPE, 
						_neighbourhoodTypes[(size_t)_defaultGameOfLifeSettings.neighbourhoodType]);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiGameOfLifeView::RenderGameOfLifeUnderpopulationSlider(int& rUnderpopulationCount,
																  const int overpopulationCount)
	{
		if (ImGui::SliderInt(LABEL_GAME_OF_LIFE_MIN_LIVE_NEIGHBOURS,
							 &rUnderpopulationCount,
							 0,
							 overpopulationCount))
		{
			OnUnderpopulationValueChanged(rUnderpopulationCount);
		}

		// Tooltip - Minimum Live Neighbours Slider
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GAME_OF_LIFE_MIN_LIVE_NEIGHBOURS, 
						rUnderpopulationCount,										// Current Value
						(size_t)_defaultGameOfLifeSettings.underpopulationCount);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiGameOfLifeView::RenderGameOfLifeOverpopulationSlider(int& rOverpopulationCount, 
																 const int underpopulationCount)
	{
		if (ImGui::SliderInt(LABEL_GAME_OF_LIFE_MAX_LIVE_NEIGHBOURS,
							 &rOverpopulationCount,
							 underpopulationCount,
							 CELL_NEIGHBOURS_COUNT))
		{
			OnOverpopulationValueChanged(rOverpopulationCount);
		}


		// Tooltip - Maximum Live Neighbours Slider
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GAME_OF_LIFE_MAX_LIVE_NEIGHBOURS,
						rOverpopulationCount,										// Current Value
						(size_t)_defaultGameOfLifeSettings.overpopulationCount);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiGameOfLifeView::RenderGameOfLifeReproductionSlider(int& rReproductionCount)
	{
		if (ImGui::SliderInt(LABEL_GAME_OF_LIFE_REPRO_LIVE_NEIGHBOURS,
							 &rReproductionCount,
							 0,
							 CELL_NEIGHBOURS_COUNT))
		{
			OnReproductionValueChanged(rReproductionCount);
		}


		// Tooltip - Maximum Live Neighbours Slider
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GAME_OF_LIFE_REPRODUCE_LIVE_NEIGHBOURS, 
						rReproductionCount,										// Current Value
						0U,														// Min Value
						CELL_NEIGHBOURS_COUNT,									// Max Value
						(size_t)_defaultGameOfLifeSettings.reproductionCount);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiGameOfLifeView::RenderGameOfLifeResetButton()
	{
		ImGui::PushID(RESET_SETTINGS_BUTTON_ID);
		if (ImGui::Button(LABEL_GAME_OF_LIFE_RESET))
		{
			_rGameOfLifeSettings.Reset();
		}
		ImGui::PopID();

		// Tooltip - Reset Game of Life Ruleset
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GAME_OF_LIFE_RESET);
		}
		ImGui::EndTooltip();
	}

	void GuiGameOfLifeView::OnUnderpopulationValueChanged(const int value)
	{
		_rGameOfLifeSettings.underpopulationCount = (NeighbourCount_t)value;
	}

	void GuiGameOfLifeView::OnOverpopulationValueChanged(const int value)
	{
		_rGameOfLifeSettings.overpopulationCount = (NeighbourCount_t)value;
	}

	void GuiGameOfLifeView::OnReproductionValueChanged(const int value)
	{
		_rGameOfLifeSettings.reproductionCount = (NeighbourCount_t)value;
	}
}