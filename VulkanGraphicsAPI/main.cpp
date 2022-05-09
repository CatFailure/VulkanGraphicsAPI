#include "Application.hpp"

#define ARG_EXE_PATH 0
#define ARG_DRAG_DROP_FILE_PATH 1

int main(size_t argc, char* argv[])
{
	const ApplicationData appData
	{
		.windowTitle	  = "Marching Cubes Cellular Automata [v1.2.0 CSV Profiling Support]",
		.windowDimensions = glm::uvec2(1280, 720),
		.exeDirectory	  = path(argv[ARG_EXE_PATH]).parent_path()
	};

	DiagnosticData diagnosticData{};
	UserSettings   settings	     {};

	// Are any settings being passed by cmd?
	if (argc == MAX_CMD_ARG_COUNT)
	{
		const std::string settingsFilepath = argv[ARG_DRAG_DROP_FILE_PATH];

		SettingsFileLoader::GetInstance().LoadSettingsFromFile(settingsFilepath.c_str(), 
															   &settings);
	}
	else if (argc > MAX_CMD_ARG_COUNT)
	{
		printf_s("Too many Arguments - Possible multiple files uploaded (This is not allowed).\nNo Settings Loaded.\n");
	}

#ifdef LAPTOP_BUILD
	settings.simulationSettings.state = SimulationState::PLAY;
#endif // LAPTOP_BUILD

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
