#pragma once
#define ENABLE_VULKAN_DEBUG_CALLBACK

#include "ApplicationData.hpp"

using namespace Utility;

namespace Engine
{
    class Application;

    typedef VkBool32(__stdcall Application:: *VulkanDebugReportCallback_t)(VkDebugReportFlagsEXT,
                                                                           VkDebugReportObjectTypeEXT,
                                                                           uint64_t,
                                                                           size_t,
                                                                           int32_t,
                                                                           const char *,
                                                                           const char *,
                                                                           void *);

    class Application : public IDisposable
    {
    public:
        Application();

        void Initialise(const ApplicationData &appData, const WNDPROC wndProcCallback);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void SetupWin32Window(const WNDPROC wndProcCallback);
        void SetupVulkanInstance();
        void SetupPhysicalDevices();

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
                                                                 int32_t messageCode, const char *layerPrefix, const char *message, void *pUserData);
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        ApplicationData _appData;
        VkInstance _vkInstance{ NULL };
        VkSurfaceKHR _vkSurface{ NULL };
        VkPhysicalDevice _vkDevice{ NULL };
    };
}
