#pragma once

namespace SolEngine::GUI
{
	static constexpr const char* TOOLTIP_GAME_OF_LIFE_GENERATION  { "Current Game of Life Generation." };
	static constexpr const char* TOOLTIP_GAME_OF_LIFE_RESET		  { "Resets the Game of Life Ruleset to Default Settings." };
	static constexpr const char* TOOLTIP_SIMULATION_SPEED		  { "Adjusts Delay between Generations (Seconds).\n(Min: %.2f, Max: %.2f)" };
	static constexpr const char* TOOLTIP_PAUSE_SIMULATION		  { "Pauses and Resumes the Simulation." };
	static constexpr const char* TOOLTIP_MIN_LIVE_NEIGHBOURS	  { "Currently ALIVE Cells with LESS than %zu Live Neighbouring Cell(s) will DIE next Generation.\n(Cannot exceed or equal Maximum Live Neighbours)." };
	static constexpr const char* TOOLTIP_MAX_LIVE_NEIGHBOURS	  { "Currently ALIVE Cells with MORE than %zu Live Neighbouring Cell(s) will DIE next Generation.\n(Cannot receed or equal Minimum Live Neighbours)." };
	static constexpr const char* TOOLTIP_REPRODUCE_LIVE_NEIGHBOURS{ "Currently DEAD Cells with EXACTLY %zu Live Neighbouring Cell(s) will become ALIVE next Generation." };
}