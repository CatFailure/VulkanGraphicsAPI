#pragma once

namespace SolEngine::GUI
{
	static constexpr const char* TITLE_DIAGNOSTICS{ "Diagnostics" };
	static constexpr const char* TITLE_SETTINGS   { "Settings" };

	static constexpr const char* HEADER_SIMULATION	 { "Simulation " };		// Added Space for quick padding
	static constexpr const char* HEADER_GAME_OF_LIFE { "Game of Life " };	// Added Space for quick padding
	static constexpr const char* HEADER_GRID		 { "Grid " };			// Added Space for quick padding
	static constexpr const char* HEADER_RENDER_SYSTEM{ "Render System " };	// Added Space for quick padding

	static constexpr const char* LABEL_SIMULATION_GENERATION { "Generation: %zu" };
	static constexpr const char* LABEL_SIMULATION_SEED		 { "Seed" };
	static constexpr const char* LABEL_SIMULATION_SEED_RESET { "Reset" };
	static constexpr const char* LABEL_SIMULATION_SPEED		 { "Speed" };
	static constexpr const char* LABEL_SIMULATION_SPEED_RESET{ "Reset" };
	static constexpr const char* LABEL_SIMULATION_RESET		 { "Reset" };
	static constexpr const char* LABEL_SIMULATION_STATE_PLAY { "Play" };
	static constexpr const char* LABEL_SIMULATION_STATE_PAUSE{ "Pause" };

	static constexpr const char* LABEL_GAME_OF_LIFE_MIN_LIVE_NEIGHBOURS  { "Underpopulation Value" };
	static constexpr const char* LABEL_GAME_OF_LIFE_MAX_LIVE_NEIGHBOURS  { "Overpopulation Value" };
	static constexpr const char* LABEL_GAME_OF_LIFE_REPRO_LIVE_NEIGHBOURS{ "Reproduction Value" };
	static constexpr const char* LABEL_GAME_OF_LIFE_RESET				 { "Reset" };

	static constexpr const char* LABEL_GRID_DIMENSIONS		 { "Grid Dimensions" };
	static constexpr const char* LABEL_GRID_DIMENSIONS_RESET { "Reset" };

	static constexpr const char* TOOLTIP_SIMULATION_GENERATION { "Current Simulation Generation." };
	static constexpr const char* TOOLTIP_SIMULATION_SEED	   { "Seed used for Generating Random Cell States.\n(Min: %i, Max: %i, Default: %i)" };
	static constexpr const char* TOOLTIP_SIMULATION_SEED_RESET { "Resets the Simulation Seed to Default.\nSimulation MUST be paused to reset." };
	static constexpr const char* TOOLTIP_SIMULATION_SPEED	   { "Adjusts Delay between Generations (Seconds).\n(Min: %.2f, Max: %.2f, Default: %.2f)" };
	static constexpr const char* TOOLTIP_SIMULATION_SPEED_RESET{ "Resets the Simulation Speed to Default." };
	static constexpr const char* TOOLTIP_SIMULATION_PAUSE	   { "Pauses and Resumes the Simulation." };
	static constexpr const char* TOOLTIP_SIMULATION_RESET	   { "Reset Simulation.\nSimulation MUST be paused to reset." };

	static constexpr const char* TOOLTIP_GAME_OF_LIFE_RESET					   { "Resets the Game of Life Ruleset to Default Settings." };
	static constexpr const char* TOOLTIP_GAME_OF_LIFE_NEIGHBOURHOOD_TYPE	   { "Defines Cell Neighbours.\n(Moore: Include Diagonals, Von Neumann: Exclude Diagonals)." };
	static constexpr const char* TOOLTIP_GAME_OF_LIFE_MIN_LIVE_NEIGHBOURS	   { "Currently ALIVE Cells with LESS than %zu Live Neighbouring Cell(s) will DIE next Generation.\n(Cannot exceed above Maximum Live Neighbours).\n(Default: %zu)." };
	static constexpr const char* TOOLTIP_GAME_OF_LIFE_MAX_LIVE_NEIGHBOURS	   { "Currently ALIVE Cells with MORE than %zu Live Neighbouring Cell(s) will DIE next Generation.\n(Cannot receed below Minimum Live Neighbours).\n(Default: %zu)." };
	static constexpr const char* TOOLTIP_GAME_OF_LIFE_REPRODUCE_LIVE_NEIGHBOURS{ "Currently DEAD Cells with EXACTLY %zu Live Neighbouring Cell(s) will become ALIVE next Generation.\n(Min: %zu, Max: %zu, Default: %zu)." };

	static constexpr const char* TOOLTIP_GRID_DIMENSIONS	  { "Sets the Grid Dimensions.\nSimulation MUST be paused to edit.\n(Min: %u, Max: %u, Default: x: %u, y: %u, z: %u)." };
	static constexpr const char* TOOLTIP_GRID_DIMENSIONS_RESET{ "Reset Grid Dimensions.\nSimulation MUST be paused to reset." };

	static constexpr const char* TOOLTIP_RENDER_POLYGON_MODE{ "Specifies the method of rasterization for polygons.\n\
Fill: Polygons are rendered using the polygon rasterization rules.\n\
Wireframe: Polygon edges are drawn as line segments.\n\
Point: Polygon vertices are drawn as points.\n\
Default: %s"};
	static constexpr const char* TOOLTIP_RENDER_CULL_MODE{"Specifies the orientation of triangles that are culled out of the render.\n\
None: No triangles are discarded.\n\
Front: Front-facing triangles are discarded.\n\
Back: Back-facing triangles are discarded.\n\
Front and Back: All triangles are discarded.\n\
Default: %s"};
}