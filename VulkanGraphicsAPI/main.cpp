#include "Application.hpp"

void ThreadTest()
{
	const size_t threadCount(10);
	const size_t arraySize(100);

	thread_pool pool{ threadCount };
	uint32_t squares[arraySize];

	pool.parallelize_loop(0, 
						  arraySize,
						  [&squares](const uint32_t a, const uint32_t b) 
						  {
							  for (uint32_t i = a; i < b; ++i)
							  {
								  squares[i] = i * i;
							  }
						  });
}

int main()
{
	ThreadTest();

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
