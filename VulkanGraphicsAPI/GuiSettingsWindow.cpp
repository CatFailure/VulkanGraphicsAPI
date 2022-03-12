#include "GuiSettingsWindow.hpp"

namespace SolEngine::GUI
{
	GuiSettingsWindow::GuiSettingsWindow(const char* windowTitle, 
										 const bool isActive, 
										 const ImGuiWindowFlags windowFlags,
										 GameOfLifeSettings& rGameOfLifeSettings, 
										 GeneralSettings& rGeneralSettings)
		: IGuiWindow(windowTitle,
					 isActive,
					 windowFlags),
		  _rGameOfLifeSettings(rGameOfLifeSettings),
		  _rGeneralSettings(rGeneralSettings)
	{
		_simulationSpeed = _rGeneralSettings.nextGenerationDelay;
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
		if (!ImGui::CollapsingHeader("General"))
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

		int minLiveNeighbourCount = (int)_rGameOfLifeSettings.minLiveNeighbourCount;
		int maxLiveNeighbourCount = (int)_rGameOfLifeSettings.maxLiveNeighbourCount;

		RenderGameOfLifeMinLiveNeighbours(minLiveNeighbourCount, maxLiveNeighbourCount);
		RenderGameOfLifeMaxLiveNeighbours(minLiveNeighbourCount, maxLiveNeighbourCount);
		RenderGameOfLifeResetButton();
	}

	void GuiSettingsWindow::RenderGeneralGenerationText()
	{
		ImGui::Text("Generation: %zu", 
					_rGeneralSettings.generation);

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
							  _simulationSpeedStep,
							  _simulationSpeedFastStep,
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
						&_rGeneralSettings.isPaused);

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
		if (!ImGui::Button("Reset"))
		{
			return;
		}

		_rGameOfLifeSettings.Reset();
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
		// Clamp the speed so it remains within limits
		_simulationSpeed = fminf(MAX_SIMULATION_SPEED, 
								 fmaxf(MIN_SIMULATION_SPEED, 
									   _simulationSpeed));

		_rGeneralSettings.nextGenerationDelay = _simulationSpeed;
	}
}
