#include "Application.hpp"

int main()
{
	const ApplicationData appData
	{
		.windowTitle	  = "[13/04/22] Marching Cubes Cellular Automata",
		.windowDimensions = glm::uvec2(1280, 720)
	};

	DiagnosticData     diagnosticData    {};
	RenderSettings     renderSettings    { .polygonMode = VK_POLYGON_MODE_FILL };
	CameraSettings     cameraSettings    {};
	GridSettings       gridSettings		 {};
	GameOfLifeSettings gameOfLifeSettings{};
	SimulationSettings simulationSettings{};

#ifdef LAPTOP_BUILD
	simulationSettings.state = SimulationState::PLAY;
	gridSettings.dimensions  = { 64 };
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
