#include "Application.hpp"
#define ARG_EXE_PATH 0
#define ARG_DRAG_DROP_FILE_PATH 1

int main(size_t argc, char* argv[])
{
	const ApplicationData appData
	{
		.windowTitle	  = "Marching Cubes Cellular Automata [v1.1 External Settings Support]",
		.windowDimensions = glm::uvec2(1280, 720),
		.exeDirectory	  = path(argv[ARG_EXE_PATH]).parent_path()
	};

	DiagnosticData diagnosticData{};
	SettingsBundle settings		 {};

	// Are any settings being passed by cmd?
	if (argc == MAX_CMD_ARG_COUNT)
	{
		const std::string settingsFilepath = argv[ARG_DRAG_DROP_FILE_PATH];

		if (settings.DeserializeFromFile(settingsFilepath))
		{
			printf_s("%s - loaded successfully!\n", 
					 settingsFilepath.c_str());
		}
		else
		{
			printf_s("One (or More) Settings failed to load from: %s!\n", 
					 settingsFilepath.c_str());
		}
	}
	else if (argc > MAX_CMD_ARG_COUNT)
	{
		printf_s("Too many Arguments - Possible multiple files uploaded (This is not allowed).\nNo Settings Loaded.");
	}

#ifdef LAPTOP_BUILD
	settings.simulationSettings.state = SimulationState::PLAY;
#endif // _DEBUG_LAPTOP || _NDEBUG_LAPTOP

	Application application(appData,
							diagnosticData,
							settings);

	try
	{
		application.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
