#include "GuiSimulationView.hpp"

namespace SolEngine::GUI::View
{
	GuiSimulationView::GuiSimulationView(SimulationSettings& rSimulationSettings)
		: _rSimulationSettings(rSimulationSettings)
	{
		_simulationSpeed = _rSimulationSettings.speed;
		SetSimulationState(_rSimulationSettings.state);
	}

	void GuiSimulationView::Render()
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

	void GuiSimulationView::SetSimulationState(const SimulationState state)
	{
		SimulationState& rSimulationState = _rSimulationSettings.state;
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

	void GuiSimulationView::RenderSimulationGenerationText()
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

	void GuiSimulationView::RenderSimulationSeedInput()
	{
		// Prevent user interaction whilst a simulation is running
		// Changing the Seed at runtime will cause Bad Things to Happen™
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
						_defaultSimulationSettings.seed);
		}
		ImGui::EndTooltip();
	}

	void GuiSimulationView::RenderSimulationResetSeedButton()
	{
		// Prevent user interaction whilst a simulation is running
		// Changing the Seed at runtime will cause Bad Things to Happen™
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

	void GuiSimulationView::RenderSimulationSimulationSpeedInput()
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
						_defaultSimulationSettings.speed);	// Default Value
		}
		ImGui::EndTooltip();
	}

	void GuiSimulationView::RenderSimulationResetSpeedButton()
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

	void GuiSimulationView::RenderSimulationPauseButton()
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

	void GuiSimulationView::RenderSimulationResetButton()
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

	void GuiSimulationView::OnSimulationSeedChanged()
	{
		_simulationSeed	= Clamp(_simulationSeed, 
								MIN_SIMULATION_SEED, 
								MAX_SIMULATION_SEED);

		_rSimulationSettings.seed = _simulationSeed;

		// Reset the Generation Count on a new seed
		// and raise the refresh flag.
		_rSimulationSettings.ResetGeneration();
		_rSimulationSettings.isSimulationResetRequested = true;
	}

	void GuiSimulationView::OnSimulationSpeedChanged()
	{
		// Keep simulation speed within limits
		_simulationSpeed = Clamp(_simulationSpeed, 
								 MIN_SIMULATION_SPEED, 
								 MAX_SIMULATION_SPEED);

		// Set speed and raise event
		_rSimulationSettings.speed = _simulationSpeed;
		_rSimulationSettings.onSimulationSpeedChangedEvent.Invoke(_simulationSpeed);
	}

	void GuiSimulationView::OnSimulationSeedReset()
	{
		// Reset seed and generation
		// and raise the refresh flag
		_rSimulationSettings.ResetGeneration();
		_rSimulationSettings.ResetSeed();
		_rSimulationSettings.isSimulationResetRequested = true;

		// Update value on GUI side
		_simulationSeed = _rSimulationSettings.seed;
	}

	void GuiSimulationView::OnSimulationSpeedReset()
	{
		// Reset speed and update on GUI side
		_rSimulationSettings.ResetSpeed();
		_simulationSpeed = _rSimulationSettings.speed;
	}

	void GuiSimulationView::OnSimulationReset()
	{
		// Reset generation and raise flag
		_rSimulationSettings.ResetGeneration();
		_rSimulationSettings.isSimulationResetRequested = true;
	}

	void GuiSimulationView::OnSimulationStateToggled()
	{
		SimulationState& rSimulationState = _rSimulationSettings.state;

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
}