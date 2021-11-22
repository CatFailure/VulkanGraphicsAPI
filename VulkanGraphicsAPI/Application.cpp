#include "Application.hpp"

void Application::Dispose()
{
	vkDestroyInstance(_vkInstance, NULL);
}

Application::Application(const ApplicationData &appData)
    : _appData(appData)
{}

void Application::SetupWin32Window(const WNDPROC wndProcCallback)
{
    // Defines window properties
    HINSTANCE hInst{ GetModuleHandle(NULL) };
    HWND &rWinHandle = _appData.windowHandle;
    LPCWSTR className{ L"VulkanWindowClass" }, windowTitle{ L"Hello Vulkan!" };

    WNDCLASSEX windowClass{};
    ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

    windowClass.cbSize        = sizeof(WNDCLASSEX);                 // Size of struct in bytes
    windowClass.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW; // Window Styles
    windowClass.lpfnWndProc   = wndProcCallback;                    // Callback func for message handling
    windowClass.hInstance     = hInst;                              // File Handle
    windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);        // Cursor type
    windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;               // Preferred Window colour
    windowClass.lpszClassName = className;                          // Class name

    RegisterClassEx(&windowClass);

    rWinHandle = CreateWindowEx(NULL,                                // No additional Window Styles
                                className,                           // Class Name
                                windowTitle,                         // Window Title
                                WS_OVERLAPPEDWINDOW | WS_VISIBLE,    // Window Appearance Styles
                                100,                                 // x
                                100,                                 // y
                                _appData.clientWidth,                // Window width
                                _appData.clientHeight,               // Window height
                                NULL,                                // No Parents
                                NULL,                                // No popup menus
                                hInst,                               // Exe File Handle
                                NULL);                               // No params to pass

    // Was this successful?
    DBG_ASSERT(rWinHandle != NULL);
}

void Application::SetupVulkanInstance()
{
    const char *layers[]{ "VK_LAYER_NV_optimus" };
    uint32_t layerCount(1), extensionCount(0);

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
    const char *extensions[]
    {
        "VK_KHR_surface",
        "VK_KHR_win32_surface",
        "VK_EXT_debug_report",
    };

    extensionCount = 3;
#else
    const char *extensions[]
    {
        "VK_KHR_surface",
        "VK_KHR_win32_surface",
    };

    extensionCount = 2;
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

    // pOutInstance initialisation
    {
        const char *appName = "Hello Vulkan";

        // Information about the application
        // to pass to the Vulkan driver.
        // Fill out application description
        VkApplicationInfo applicationInfo
        {
            .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,		// Mandatory, describes type of structure
            .pNext            = NULL,									// Mandatory, stores pointers to extension-specific structures
            .pApplicationName = appName,								// Name of the application
            .pEngineName      = NULL,									// Name of the engine
            .engineVersion    = 1,										// Version of the engine
            .apiVersion       = VK_API_VERSION_1_0						// Version of Vulkan used by application
        };

        // Fill out instance description
        VkInstanceCreateInfo instanceCreateInfo
        {
            .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	// Mandatory
            .pNext                   = NULL,									// Mandatory set
            .flags                   = 0,										// Mandatory set
            .pApplicationInfo        = &applicationInfo,						// Pass application info instance
            .enabledLayerCount       = layerCount,								// Number of enabled layers
            .ppEnabledLayerNames     = layers,                                  // Specified layer names
            .enabledExtensionCount   = extensionCount,							// Number of enabled extensions
            .ppEnabledExtensionNames = extensions,                              // Specified extension names
        };

        VkResult result = vkCreateInstance(&instanceCreateInfo,
                                           NULL,
                                           &_vkInstance);

        // Was creation successful?
        DBG_ASSERT_VULKAN_MSG(result,
                              "Failed to create Vulkan instance.");

        // Is instance handle valid?
        DBG_ASSERT(_vkInstance != NULL);
    }

    // Optional - Setup debug callbacks so Vulkan 
    // can tell us if something has gone wrong.
#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
    {
        // Register error logging function
        VkDebugReportCallbackEXT warningCallback{ VK_NULL_HANDLE }, errorCallback{ VK_NULL_HANDLE };
        PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT{ NULL };

        *(void **)&vkCreateDebugReportCallbackEXT = vkGetInstanceProcAddr(_vkInstance,
                                                                          "vkCreateDebugReportCallbackEXT");

        DBG_ASSERT(vkCreateDebugReportCallbackEXT);

        auto vulkanDebugReportCallback = &Application::VulkanDebugReportCallback;

        // Capture errors
        VkDebugReportCallbackCreateInfoEXT callbackCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT,
            .pfnCallback = (PFN_vkDebugReportCallbackEXT)&vulkanDebugReportCallback
        };

        VkResult result = vkCreateDebugReportCallbackEXT(_vkInstance,
                                                         &callbackCreateInfo,
                                                         nullptr,
                                                         &errorCallback);

        DBG_ASSERT_VULKAN_MSG(result,
                              "vkCreateDebugReportCallbackEXT (ERROR) failed.");

        // Capture warnings
        callbackCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        callbackCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)&vulkanDebugReportCallback;

        result = vkCreateDebugReportCallbackEXT(_vkInstance,
                                                &callbackCreateInfo,
                                                nullptr,
                                                &warningCallback);

        DBG_ASSERT_VULKAN_MSG(result,
                              "vkCreateDebugReportCallbackEXT (WARNING) failed.");
    }
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

    // pOutSurface initialisation
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo
        {
            .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .hinstance = hInstance,     // Param is NULL - GetModuleHandle returns handle to file used to create the calling process
            .hwnd      = _appData.windowHandle
        };

        // OutSurface must be empty before assignment
        DBG_ASSERT(_vkSurface == NULL);

        VkResult result = vkCreateWin32SurfaceKHR(_vkInstance,
                                                  &surfaceCreateInfo,
                                                  NULL,
                                                  &_vkSurface);

        DBG_ASSERT_VULKAN_MSG(result,
                              "Could not create Surface.");

        DBG_ASSERT(_vkSurface != NULL);
    }
}

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
VKAPI_ATTR VkBool32 VKAPI_CALL Application::VulkanDebugReportCallback(VkDebugReportFlagsEXT flags,
                                                                      VkDebugReportObjectTypeEXT objectType,
                                                                      uint64_t object,
                                                                      size_t location,
                                                                      int32_t messageCode,
                                                                      const char *layerPrefix,
                                                                      const char *message,
                                                                      void *pUserData)
{
    DebugHelpers::DPrintf(layerPrefix);
    DebugHelpers::DPrintf(" ");
    DebugHelpers::DPrintf(message);
    DebugHelpers::DPrintf("\n");

    DBG_ASSERT(false);

    return VK_FALSE;
}
#endif // ENABLE_VULKAN_DEBUG_CALLBACK
