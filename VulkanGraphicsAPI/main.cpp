#include "Application.hpp"

int main()
{
	const ApplicationData appData
	{
		.windowTitle	  = "[11/02/22] ImGui - Marching Cubes Window",
		.windowClassName  = L"VulkanWindowClass",
		.engineName		  = "SolEngine",
		.appName		  = "VulkanGraphicsAPI",
		.windowDimensions = glm::uvec2(1280, 720)
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
