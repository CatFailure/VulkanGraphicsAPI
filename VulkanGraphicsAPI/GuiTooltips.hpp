#pragma once

namespace SolEngine::GUI
{
	static constexpr const char* TOOLTIP_GAME_OF_LIFE_GENERATION{ "Current Game of Life Generation." };
	static constexpr const char* TOOLTIP_GAME_OF_LIFE_RESET		{ "Resets the Game of Life Ruleset to Default Settings." };
	static constexpr const char* TOOLTIP_SIMULATION_SPEED		{ "Adjusts Delay between Generations.\n(Min: %.2f, Max: %.2f)" };
	static constexpr const char* TOOLTIP_PAUSE_SIMULATION		{ "Pauses and Resumes the Simulation." };
	static constexpr const char* TOOLTIP_MIN_LIVE_NEIGHBOURS	{ "Cells with LESS than %zu Live Cells will die NEXT Generation.\n(Cannot exceed or equal Maximum Live Neighbours)." };
	static constexpr const char* TOOLTIP_MAX_LIVE_NEIGHBOURS	{ "Cells with MORE than %zu Live Cells will die NEXT Generation.\n(Cannot receed or equal Minimum Live Neighbours)." };
}