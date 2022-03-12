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
		  _rSimulationSettings(rSimulationSettings)
	{
		_simulationSpeed = _rSimulationSettings.nextGenerationDelay;
	}

	void GuiSettingsWindow::RenderWindowContents()
	{
		ImGui::Begin(_windowTitle, 
					 &_isActive, 
					 _windowFlags);

		RenderSimulationSettings();
		RenderGameOfLifeSettings();

		ImGui::End();
	}

	void GuiSettingsWindow::RenderSimulationSettings()
	{
		if (!ImGui::CollapsingHeader("Simulation",
									 ImGuiTreeNodeFlags_DefaultOpen))	// This header should be expanded by default
		{
			return;
		}

		RenderSimulationGenerationText();
		RenderSimulationSimulationSpeedInput();
		RenderSimulationPauseButton();
	}

	void GuiSettingsWindow::RenderGameOfLifeSettings()
	{
		if (!ImGui::CollapsingHeader("Game of Life"))
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

	void GuiSettingsWindow::RenderSimulationGenerationText()
	{
		ImGui::Text("Generation: %zu", 
					_rSimulationSettings.generation);

		// Tooltip - Game of Life Generation
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_GAME_OF_LIFE_GENERATION);
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderSimulationSimulationSpeedInput()
	{
		if (ImGui::InputFloat("Simulation Speed",
							  &_simulationSpeed,
							  SIMULATION_SPEED_SLIDER_STEP,
							  SIMULATION_SPEED_SLIDER_FAST_STEP,
							  "%.2f", 
							  ImGuiInputTextFlags_EnterReturnsTrue))
		{
			OnSimulationSpeedChanged();
		}

		// Tooltip - Simulation Speed
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_SIMULATION_SPEED, 
						MIN_SIMULATION_SPEED,								// Min Value
						MAX_SIMULATION_SPEED,								// Max Value
						_defaultSimulationSettings.nextGenerationDelay);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderSimulationPauseButton()
	{
		ImGui::Checkbox("Pause Simulation", 
						&_rSimulationSettings.isPaused);

		// Tooltip - Pause Simulation
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_PAUSE_SIMULATION);
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderGameOfLifeMinLiveNeighboursSlider(int& rMinLiveNeighbourCount, 
																	const int maxLiveNeighbourCount)
	{
		if (ImGui::SliderInt("Min. Live Neighbours",
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
			ImGui::Text(TOOLTIP_MIN_LIVE_NEIGHBOURS, 
						rMinLiveNeighbourCount,										// Current Value
						(size_t)_defaultGameOfLifeSettings.minLiveNeighbourCount);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderGameOfLifeMaxLiveNeighboursSlider(int& rMaxLiveNeighbourCount, 
																	const int minLiveNeighbourCount)
	{
		if (ImGui::SliderInt("Max. Live Neighbours",
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
			ImGui::Text(TOOLTIP_MAX_LIVE_NEIGHBOURS,
						rMaxLiveNeighbourCount,										// Current Value
						(size_t)_defaultGameOfLifeSettings.maxLiveNeighbourCount);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderGameOfLifeReproductionLiveNeighboursSlider(int& rReproductionLiveNeighbourCount)
	{
		if (ImGui::SliderInt("Repro. Live Neighbours",
							 &rReproductionLiveNeighbourCount,
							 0,
							 CELL_NEIGHBOURS_COUNT))
		{
			OnReproLiveNeighboursChanged(rReproductionLiveNeighbourCount);
		}


		// Tooltip - Maximum Live Neighbours Slider
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_REPRODUCE_LIVE_NEIGHBOURS, 
						rReproductionLiveNeighbourCount,										// Current Value
						0U,																		// Min Value
						CELL_NEIGHBOURS_COUNT,													// Max Value
						(size_t)_defaultGameOfLifeSettings.reproductionLiveNeighbourCount);		// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderGameOfLifeResetButton()
	{
		if (ImGui::Button("Reset"))
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

	void GuiSettingsWindow::OnReproLiveNeighboursChanged(const int value)
	{
		_rGameOfLifeSettings.reproductionLiveNeighbourCount = (NeighbourCount_t)value;
	}

	void GuiSettingsWindow::OnSimulationSpeedChanged()
	{
		// Keep simulation speed within limits
		_simulationSpeed = Clamp(_simulationSpeed, 
								 MIN_SIMULATION_SPEED, 
								 MAX_SIMULATION_SPEED);

		_rSimulationSettings.nextGenerationDelay = _simulationSpeed;
	}
}
