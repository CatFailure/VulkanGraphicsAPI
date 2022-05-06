#pragma once
#include "CameraSettings.hpp"
#include "GameOfLifeSettings.hpp"
#include "GridSettings.hpp"
#include "RenderSettings.hpp"
#include "SimulationSettings.hpp"
#include "ProfilerSettings.hpp"

namespace SolEngine::Settings
{
    struct SettingsBundle : public ISerializable
    {
		virtual bool Deserialize(const rapidjson::Value& obj) override
		{
			if (!JsonHasMembers(obj, 
								PROP_SETTINGS_CAMERA,
								PROP_SETTINGS_GAME_OF_LIFE,
								PROP_SETTINGS_GRID, 
								PROP_SETTINGS_RENDER, 
								PROP_SETTINGS_SIMULATION,
								PROP_SETTINGS_PROFILER))
			{
				DBG_ASSERT_MSG(false, "[SimulationSettings] Object Missing Members!");

				return false;
			}

			// Deserialize each setting...
			return cameraSettings.Deserialize(obj[PROP_SETTINGS_CAMERA].GetObj()) &&
				   gameOfLifeSettings.Deserialize(obj[PROP_SETTINGS_GAME_OF_LIFE].GetObj()) &&
				   gridSettings.Deserialize(obj[PROP_SETTINGS_GRID].GetObj()) &&
				   renderSettings.Deserialize(obj[PROP_SETTINGS_RENDER].GetObj()) &&
				   simulationSettings.Deserialize(obj[PROP_SETTINGS_SIMULATION].GetObj()) &&
				   profilerSettings.Deserialize(obj[PROP_SETTINGS_PROFILER].GetObj());
		}

        CameraSettings     cameraSettings    {};
        GameOfLifeSettings gameOfLifeSettings{};
        GridSettings       gridSettings      {};
        RenderSettings     renderSettings    {};
        SimulationSettings simulationSettings{};
		ProfilerSettings   profilerSettings  {};

	private:
		static constexpr const char* PROP_SETTINGS_CAMERA	   { "camera_settings" };
		static constexpr const char* PROP_SETTINGS_GAME_OF_LIFE{ "game_of_life_settings" };
		static constexpr const char* PROP_SETTINGS_GRID		   { "grid_settings" };
		static constexpr const char* PROP_SETTINGS_RENDER	   { "render_settings" };
		static constexpr const char* PROP_SETTINGS_SIMULATION  { "simulation_settings" };
		static constexpr const char* PROP_SETTINGS_PROFILER    { "profiler_settings" };
    };
}
