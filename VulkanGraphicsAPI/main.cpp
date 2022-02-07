#include "pch.hpp"
#include "Application.hpp"

int main()
{
	const ApplicationData appData
	{
		.windowTitle	  = "[Clean Up] Clean up Cubes",
		.windowClassName  = L"VulkanWindowClass",
		.engineName		  = "SolEngine",
		.appName		  = "VulkanGraphicsAPI",
		.windowDimensions = glm::uvec2(800, 600)
	};

	Application application(appData);

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
