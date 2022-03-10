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

	void GuiSettingsWindow::RenderGameOfLifeSettings()
	{
		if (!ImGui::CollapsingHeader("Game of Life"))
		{
			return;
		}

		int minLiveNeighbourCount = (int)_rGameOfLifeSettings.minLiveNeighbourCount;
		int maxLiveNeighbourCount = (int)_rGameOfLifeSettings.maxLiveNeighbourCount;

		if (ImGui::SliderInt("Min. Live Neighbours",
							 &minLiveNeighbourCount,
							 0,
							 maxLiveNeighbourCount - 1))
		{
			OnMinLiveNeighboursChanged(minLiveNeighbourCount);
		}

		if (ImGui::SliderInt("Max. Live Neighbours",
							 &maxLiveNeighbourCount,
							 minLiveNeighbourCount + 1,
							 CELL_NEIGHBOURS_COUNT))
		{
			OnMaxLiveNeighboursChanged(maxLiveNeighbourCount);
		}

		if (ImGui::Button("Reset"))
		{
			_rGameOfLifeSettings.Reset();
		}
	}

	void GuiSettingsWindow::RenderGeneralSettings()
	{
		if (!ImGui::CollapsingHeader("General"))
		{
			return;
		}

		RenderGenerationText();
		RenderSimulationSpeedInputFloat();
		RenderPauseButton();
	}

	void GuiSettingsWindow::RenderGenerationText()
	{
		ImGui::Text("Generation: %zu", 
					_rGeneralSettings.generation);

		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		ImGui::Text("Current Game of Life Generation.");
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderSimulationSpeedInputFloat()
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

		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		ImGui::Text("Adjusts Next Generation Delay.\n(Lower = Faster, Higher = Slower)");
		ImGui::Text("(Min: %.2f, Max: %.2f)", 
					MIN_SIMULATION_SPEED,
					MAX_SIMULATION_SPEED);
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderPauseButton()
	{
		ImGui::Checkbox("Pause", 
						&_rGeneralSettings.isPaused);

		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		ImGui::Text("Pauses and Resumes the Simulation.");
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
		// Clamp the speed so it remains within limits
		_simulationSpeed = fminf(MAX_SIMULATION_SPEED, 
								 fmaxf(MIN_SIMULATION_SPEED, 
									   _simulationSpeed));

		_rGeneralSettings.nextGenerationDelay = _simulationSpeed;
	}
}
