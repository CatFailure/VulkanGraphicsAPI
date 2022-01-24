#include "pch.hpp"
#include "Application.hpp"

using namespace SolEngine;

int main()
{
	ApplicationData appData
	{
		.windowTitle	  = "[Dev] SolClock deltaTime",
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

	return EXIT_SUCCESS;
}
