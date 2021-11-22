#pragma once
#define ENABLE_VULKAN_DEBUG_CALLBACK

#include "IDisposable.hpp"
#include "ApplicationData.hpp"
#include "DebugHelpers.hpp"

using namespace Utility;

class Application : public IDisposable
{
public:
	Application(const ApplicationData &appData);

	void SetupWin32Window(const WNDPROC wndProcCallback);
	void SetupVulkanInstance();

	// Inherited via IDisposable
	virtual void Dispose() override;
private:
#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
															 int32_t messageCode, const char *layerPrefix, const char *message, void *pUserData);
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

	ApplicationData _appData;
	VkInstance _vkInstance{ NULL };
	VkSurfaceKHR _vkSurface{ NULL };
};
