#include "GuiSettingsWindow.hpp"

namespace SolEngine::GUI
{
	GuiSettingsWindow::GuiSettingsWindow(const char* windowTitle, 
										 const bool isActive, 
										 const ImGuiWindowFlags windowFlags,
										 GameOfLifeSettings& rGameOfLifeSettings, 
										 SimulationSettings& rSimulationSettings)
		: IGuiWindow(windowTitle,
					 isActive,
					 windowFlags),
		  _rGameOfLifeSettings(rGameOfLifeSettings),
		  _simulationView(rSimulationSettings)
	{}

	void GuiSettingsWindow::RenderWindowContents()
	{
		ImGui::Begin(_windowTitle, 
					 &_isActive, 
					 _windowFlags);

		_simulationView.Render();
		ImGui::Separator();
		RenderGameOfLifeSettings();

		ImGui::End();
	}

	void GuiSettingsWindow::RenderGameOfLifeSettings()
	{
		if (!ImGui::CollapsingHeader(HEADER_GAME_OF_LIFE))
		{
			return;
		}

		// Make a copy of these values for input sanitation 
		// (See OnMinLiveNeighboursChanged, OnMaxLiveNeighboursChanged and OnReproLiveNeighboursChanged)
		int minLiveNeighbourCount	= (int)_rGameOfLifeSettings.minLiveNeighbourCount;
		int maxLiveNeighbourCount	= (int)_rGameOfLifeSettings.maxLiveNeighbourCount;
		int reproLiveNeighbourCount = (int)_rGameOfLifeSettings.reproductionLiveNeighbourCount;

		RenderGameOfLifeMinLiveNeighboursSlider(minLiveNeighbourCount, maxLiveNeighbourCount);
		RenderGameOfLifeMaxLiveNeighboursSlider(maxLiveNeighbourCount, minLiveNeighbourCount);
		RenderGameOfLifeReproductionLiveNeighboursSlider(reproLiveNeighbourCount);
		RenderGameOfLifeResetButton();
	}

	void GuiSettingsWindow::RenderGameOfLifeMinLiveNeighboursSlider(int& rMinLiveNeighbourCount, 
																	const int maxLiveNeighbourCount)
	{
		if (ImGui::SliderInt(LABEL_GAME_OF_LIFE_MIN_LIVE_NEIGHBOURS,
							 &rMinLiveNeighbourCount,
							 0,
							 maxLiveNeighbourCount))
		{
			OnMinLiveNeighboursChanged(rMinLiveNeighbourCount);
		}

		// Tooltip - Minimum Live Neighbours Slider
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GAME_OF_LIFE_MIN_LIVE_NEIGHBOURS, 
						rMinLiveNeighbourCount,										// Current Value
						(size_t)_defaultGameOfLifeSettings.minLiveNeighbourCount);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderGameOfLifeMaxLiveNeighboursSlider(int& rMaxLiveNeighbourCount, 
																	const int minLiveNeighbourCount)
	{
		if (ImGui::SliderInt(LABEL_GAME_OF_LIFE_MAX_LIVE_NEIGHBOURS,
							 &rMaxLiveNeighbourCount,
							 minLiveNeighbourCount,
							 CELL_NEIGHBOURS_COUNT))
		{
			OnMaxLiveNeighboursChanged(rMaxLiveNeighbourCount);
		}


		// Tooltip - Maximum Live Neighbours Slider
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GAME_OF_LIFE_MAX_LIVE_NEIGHBOURS,
						rMaxLiveNeighbourCount,										// Current Value
						(size_t)_defaultGameOfLifeSettings.maxLiveNeighbourCount);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderGameOfLifeReproductionLiveNeighboursSlider(int& rReproductionLiveNeighbourCount)
	{
		if (ImGui::SliderInt(LABEL_GAME_OF_LIFE_REPRO_LIVE_NEIGHBOURS,
							 &rReproductionLiveNeighbourCount,
							 0,
							 CELL_NEIGHBOURS_COUNT))
		{
			OnReproductionLiveNeighboursChanged(rReproductionLiveNeighbourCount);
		}


		// Tooltip - Maximum Live Neighbours Slider
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GAME_OF_LIFE_REPRODUCE_LIVE_NEIGHBOURS, 
						rReproductionLiveNeighbourCount,										// Current Value
						0U,																		// Min Value
						CELL_NEIGHBOURS_COUNT,													// Max Value
						(size_t)_defaultGameOfLifeSettings.reproductionLiveNeighbourCount);		// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderGameOfLifeResetButton()
	{
		if (ImGui::Button(LABEL_GAME_OF_LIFE_RESET))
		{
			_rGameOfLifeSettings.Reset();
		}

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

	void GuiSettingsWindow::OnMinLiveNeighboursChanged(const int value)
	{
		_rGameOfLifeSettings.minLiveNeighbourCount = (NeighbourCount_t)value;
	}

	void GuiSettingsWindow::OnMaxLiveNeighboursChanged(const int value)
	{
		_rGameOfLifeSettings.maxLiveNeighbourCount = (NeighbourCount_t)value;
	}

	void GuiSettingsWindow::OnReproductionLiveNeighboursChanged(const int value)
	{
		_rGameOfLifeSettings.reproductionLiveNeighbourCount = (NeighbourCount_t)value;
	}
}
