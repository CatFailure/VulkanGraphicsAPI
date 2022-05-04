#include "Application.hpp"

int main(size_t argc, char* argv[])
{
	for (size_t i = 0; i < argc; i++)
	{
		printf_s("%s\n", argv[i]);
	}

	const ApplicationData appData
	{
		.windowTitle	  = "[13/04/22] Marching Cubes Cellular Automata",
		.windowDimensions = glm::uvec2(1280, 720)
	};

	DiagnosticData     diagnosticData    {};
	RenderSettings     renderSettings    {};
	CameraSettings     cameraSettings    {};
	GridSettings       gridSettings		 {};
	GameOfLifeSettings gameOfLifeSettings{};
	SimulationSettings simulationSettings{};

#ifdef LAPTOP_BUILD
	simulationSettings.state = SimulationState::PLAY;
#endif // _DEBUG_LAPTOP || _NDEBUG_LAPTOP

	Application application(appData,
							diagnosticData,
							renderSettings,
							cameraSettings,
							gridSettings,
							gameOfLifeSettings,
							simulationSettings);

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
