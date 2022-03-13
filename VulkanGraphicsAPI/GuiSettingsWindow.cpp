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
		_simulationSpeed = _rSimulationSettings.simulationSpeed;
		SetSimulationState(_rSimulationSettings.simulationState);
	}

	void GuiSettingsWindow::RenderWindowContents()
	{
		ImGui::Begin(_windowTitle, 
					 &_isActive, 
					 _windowFlags);

		RenderSimulationSettings();
		ImGui::Separator();
		RenderGameOfLifeSettings();

		ImGui::End();
	}

	void GuiSettingsWindow::RenderSimulationSettings()
	{
		if (!ImGui::CollapsingHeader(HEADER_SIMULATION,
									 ImGuiTreeNodeFlags_DefaultOpen))	// This header should be expanded by default
		{
			return;
		}

		RenderSimulationGenerationText();

		// Simulation Seed Controls
		RenderSimulationSeedInput();
		ImGui::SameLine(SIMULATION_RESET_SEED_OFFSET);
		RenderSimulationResetSeedButton();

		// Simulation Speed Controls
		RenderSimulationSimulationSpeedInput();
		ImGui::SameLine();
		RenderSimulationResetSpeedButton();

		RenderSimulationPauseButton(); 
		ImGui::SameLine();
		RenderSimulationResetButton();
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

	void GuiSettingsWindow::RenderSimulationGenerationText()
	{
		ImGui::Text(LABEL_SIMULATION_GENERATION, 
					_rSimulationSettings.generation);

		// Tooltip - Game of Life Generation
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_SIMULATION_GENERATION);
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderSimulationSeedInput()
	{
		// Prevent user interaction whilst a simulation is running
		// Changing the Seed at runtime will cause Bad Things to Happen�
		ImGui::BeginDisabled(IsSimulationPlaying());
		{
			if (ImGui::InputInt(LABEL_SIMULATION_SEED, 
								&_simulationSeed, 
								SIMULATION_SEED_INPUT_STEP,
								SIMULATION_SEED_INPUT_FAST_STEP,
								ImGuiInputTextFlags_EnterReturnsTrue))
			{
				OnSimulationSeedChanged();
			}
		}
		ImGui::EndDisabled();

		// Tooltip - Game of Life Seed
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_SIMULATION_SEED,
						MIN_SIMULATION_SEED,
						MAX_SIMULATION_SEED,
						_simulationSettings_default.seed);
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderSimulationResetSeedButton()
	{
		// Prevent user interaction whilst a simulation is running
		// Changing the Seed at runtime will cause Bad Things to Happen�
		ImGui::BeginDisabled(IsSimulationPlaying());
		{
			ImGui::PushID(RESET_SEED_BUTTON_ID);	// Since there are multiple buttons with a "Reset" label, we have to define a unique ID here
			if (ImGui::Button(LABEL_SIMULATION_SEED_RESET))
			{
				OnSimulationSeedReset();
			}
			ImGui::PopID();
		}
		ImGui::EndDisabled();

		// Tooltip - Game of Life Seed Reset
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_SIMULATION_SEED_RESET);
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderSimulationSimulationSpeedInput()
	{
		if (ImGui::InputFloat(LABEL_SIMULATION_SPEED,
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
						MIN_SIMULATION_SPEED,							// Min Value
						MAX_SIMULATION_SPEED,							// Max Value
						_simulationSettings_default.simulationSpeed);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderSimulationResetSpeedButton()
	{
		ImGui::PushID(RESET_SPEED_BUTTON_ID);	// Since there are multiple buttons with a "Reset" label, we have to define a unique ID here
		if (ImGui::Button(LABEL_SIMULATION_SPEED_RESET))
		{
			OnSimulationSpeedReset();
		}
		ImGui::PopID();

		// Tooltip - Game of Life Seed Reset
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_SIMULATION_SPEED_RESET);
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderSimulationPauseButton()
	{
		if (ImGui::Button(_toggleStateButtonText.c_str()))
		{
			OnSimulationStateToggled();
		}

		// Tooltip - Pause Simulation
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_SIMULATION_PAUSE);
		}
		ImGui::EndTooltip();
	}

	void GuiSettingsWindow::RenderSimulationResetButton()
	{
		ImGui::BeginDisabled(IsSimulationPlaying());
		{
			if (ImGui::Button(LABEL_SIMULATION_RESET))
			{
				OnSimulationReset();
			}
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

	void GuiSettingsWindow::OnSimulationSeedChanged()
	{
		_simulationSeed	= Clamp(_simulationSeed, 
								MIN_SIMULATION_SEED, 
								MAX_SIMULATION_SEED);

		_rSimulationSettings.seed = _simulationSeed;
		_rSimulationSettings.wasSimulationResetRequested = true;
	}

	void GuiSettingsWindow::OnSimulationSpeedChanged()
	{
		// Keep simulation speed within limits
		_simulationSpeed = Clamp(_simulationSpeed, 
								 MIN_SIMULATION_SPEED, 
								 MAX_SIMULATION_SPEED);

		_rSimulationSettings.simulationSpeed = _simulationSpeed;
		_rSimulationSettings.onSimulationSpeedChangedEvent.Invoke(_simulationSpeed);
	}

	void GuiSettingsWindow::OnSimulationSeedReset()
	{
		_rSimulationSettings.ResetSimulationSeed();
		_simulationSeed = _rSimulationSettings.seed;
	}

	void GuiSettingsWindow::OnSimulationSpeedReset()
	{
		_rSimulationSettings.ResetSimulationSpeed();
		_simulationSpeed = _rSimulationSettings.simulationSpeed;
	}

	void GuiSettingsWindow::OnSimulationReset()
	{
		_rSimulationSettings.Reset();
		_simulationSpeed = _rSimulationSettings.simulationSpeed;
	}

	void GuiSettingsWindow::OnSimulationStateToggled()
	{
		SimulationState& rSimulationState = _rSimulationSettings.simulationState;

		switch (rSimulationState)
		{
		case SimulationState::PAUSED:
		{
			SetSimulationState(SimulationState::PLAY);

			return;
		}
		case SimulationState::PLAY:
		default:	// Move simulation to a paused state if something goes wrong just in case...
		{
			SetSimulationState(SimulationState::PAUSED);

			return;
		}
		}
	}

	void GuiSettingsWindow::SetSimulationState(const SimulationState state)
	{
		SimulationState& rSimulationState = _rSimulationSettings.simulationState;
		rSimulationState				  = state;

		switch (state)
		{
		case SimulationState::PAUSED:
		{
			_toggleStateButtonText = LABEL_SIMULATION_STATE_PLAY;

			return;
		}
		case SimulationState::PLAY:
		default:	// Move simulation to a paused state if something goes wrong just in case...
		{
			_toggleStateButtonText = LABEL_SIMULATION_STATE_PAUSE;

			return;
		}
		}
	}
}
