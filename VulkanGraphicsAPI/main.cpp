#include "Application.hpp"

int main()
{
	const ApplicationData appData
	{
		.windowTitle	  = "[13/03/22] Simulation Settings GUI",
		.windowDimensions = glm::uvec2(1280, 720)
	};

	DiagnosticData     diagnosticData    {};
	GridSettings       gridSettings		 { .dimensions = glm::uvec3{ 30 } };
	GameOfLifeSettings gameOfLifeSettings{};
	SimulationSettings simulationSettings{};

	Application application(appData, 
							diagnosticData,
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
