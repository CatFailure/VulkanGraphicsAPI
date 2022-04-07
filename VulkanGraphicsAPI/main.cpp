#include "Application.hpp"

int main()
{
	const ApplicationData appData
	{
		.windowTitle	  = "[28/03/22] Cellular Automata | Release Candidate 2",
		.windowDimensions = glm::uvec2(1280, 720)
	};

	DiagnosticData     diagnosticData    {};
	CameraSettings     cameraSettings    {};
	GridSettings       gridSettings		 {};
	GameOfLifeSettings gameOfLifeSettings{};
	SimulationSettings simulationSettings{};

#ifdef LAPTOP_BUILD
	simulationSettings.state = SimulationState::PLAY;
#endif // _DEBUG_LAPTOP || _NDEBUG_LAPTOP

	Application application(appData,
							diagnosticData,
							cameraSettings,
							gridSettings,
							gameOfLifeSettings,
							simulationSettings);

	try
	{
		application.Run();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
