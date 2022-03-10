#include "Application.hpp"

int main()
{
	const ApplicationData appData
	{
		.windowTitle	  = "[09/03/22] Integrating Game of Life",
		.windowDimensions = glm::uvec2(1280, 720)
	};

	DiagnosticData     diagnosticData    {};
	GridSettings       gridSettings		 { .dimensions = glm::uvec3{ 30 } };
	GameOfLifeSettings gameOfLifeSettings{ .nextGenerationDelay = .5f };

	Application application(appData, 
							diagnosticData,
							gridSettings,
							gameOfLifeSettings);

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
