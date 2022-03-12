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

		RenderGeneralSettings();
		RenderGameOfLifeSettings();

		ImGui::End();
	}

	void GuiSettingsWindow::RenderGeneralSettings()
	{
		if (!ImGui::CollapsingHeader("Simulation"))
		{
			return;
		}

		RenderGeneralGenerationText();
		RenderGeneralSimulationSpeedInputFloat();
		RenderGeneralPauseButton();
	}

	void GuiSettingsWindow::RenderGameOfLifeSettings()
	{
		if (!ImGui::CollapsingHeader("Game of Life"))
		{
			return;
		}

		// Make a copy of these values for input sanitation 
		// (See OnMinLiveNeighboursChanged and OnMaxLiveNeighboursChanged)
		int minLiveNeighbourCount = (int)_rGameOfLifeSettings.minLiveNeighbourCount;
		int maxLiveNeighbourCount = (int)_rGameOfLifeSettings.maxLiveNeighbourCount;

		RenderGameOfLifeMinLiveNeighbours(minLiveNeighbourCount, maxLiveNeighbourCount);
		RenderGameOfLifeMaxLiveNeighbours(minLiveNeighbourCount, maxLiveNeighbourCount);
		RenderGameOfLifeResetButton();
	}

	void GuiSettingsWindow::RenderGeneralGenerationText()
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

	void GuiSettingsWindow::RenderGeneralSimulationSpeedInputFloat()
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
						MIN_SIMULATION_SPEED,
						MAX_SIMULATION_SPEED);
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderGeneralPauseButton()
	{
		ImGui::Checkbox("Pause", 
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

	void GuiSettingsWindow::RenderGameOfLifeMinLiveNeighbours(int& rMinLiveNeighbourCount, 
															  const int maxLiveNeighbourCount)
	{
		if (ImGui::SliderInt("Min. Live Neighbours",
							 &rMinLiveNeighbourCount,
							 0,
							 maxLiveNeighbourCount - 1))
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
						rMinLiveNeighbourCount);
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderGameOfLifeMaxLiveNeighbours(const int minLiveNeighbourCount, 
															  int& rMaxLiveNeighbourCount)
	{
		if (ImGui::SliderInt("Max. Live Neighbours",
							 &rMaxLiveNeighbourCount,
							 minLiveNeighbourCount + 1,
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
						rMaxLiveNeighbourCount);
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

	void GuiSettingsWindow::OnSimulationSpeedChanged()
	{
		// Keep simulation speed within limits
		_simulationSpeed = Clamp(_simulationSpeed, 
								 MIN_SIMULATION_SPEED, 
								 MAX_SIMULATION_SPEED);

		_rSimulationSettings.nextGenerationDelay = _simulationSpeed;
	}
}
