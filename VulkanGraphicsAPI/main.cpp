#include "pch.hpp"
#include "Application.hpp"

using namespace SolEngine;

int main()
{
	ApplicationData appData
	{
		.windowTitle	  = "Hello Vulkan!",
		.windowClassName  = L"VulkanWindowClass",
		.engineName		  = "SolEngine",
		.appName		  = "VulkanGraphicsAPI",
		.windowDimensions = Vector2u(800, 600)
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

	application.Dispose();

	return EXIT_SUCCESS;
}
