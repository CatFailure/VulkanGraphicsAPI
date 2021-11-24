#include "pch.hpp"
#include "Application.hpp"

namespace Engine
{
    //const uint32_t Application::ENABLED_LAYER_COUNT;

    Application::Application(const ApplicationData &appData)
        : _appData(appData)
    {}

    void Application::Dispose()
    {
        vkDestroyInstance(_vkInstance, NULL);
    }

    void Application::Initialise(const WNDPROC wndProcCallback)
    {
        SetupWin32Window(wndProcCallback);
        SetupVulkanInstance();
        SetupPhysicalDevices();
        SetupVulkanDevice();
    }

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
        SetupVulkanInstance_InitVkInstance();

        // Optional - Setup debug callbacks so Vulkan 
        // can tell us if something has gone wrong.
#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        SetupVulkanInstance_InitVkDebugCallback();
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        SetupVulkanInstance_InitVkSurface();
    }

    void Application::SetupVulkanInstance_InitVkInstance()
    {
        uint32_t extensionCount(0);

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
            .enabledLayerCount       = ENABLED_LAYER_COUNT,								// Number of enabled layers
            .ppEnabledLayerNames     = _enabledLayerNames.data(),              // Specified layer names
            .enabledExtensionCount   = extensionCount,							// Number of enabled extensions
            .ppEnabledExtensionNames = extensions,                              // Specified extension names
        };

        VkResult result = vkCreateInstance(&instanceCreateInfo,
                                           NULL,
                                           &_vkInstance);

        // Was creation successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Vulkan instance.");

        // Is instance handle valid?
        DBG_ASSERT(_vkInstance != NULL);
    }

    void Application::SetupVulkanInstance_InitVkSurface()
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

        DBG_ASSERT_VULKAN_MSG(result, "Could not create Surface.\n");

        DBG_ASSERT(_vkSurface != NULL);
    }

    void Application::SetupPhysicalDevices()
    {
        // Query how many devices are present
        uint32_t deviceCount(0);

        VkResult result = vkEnumeratePhysicalDevices(_vkInstance,
                                                     &deviceCount,
                                                     NULL);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to query the number of physical devices present.\n");

        // Make sure at least 1 device is present.
        DBG_ASSERT_MSG(deviceCount != 0, "Could not detect any physical devices present with Vulkan support.\n");

        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);

        result = vkEnumeratePhysicalDevices(_vkInstance,
                                            &deviceCount, 
                                            &physicalDevices.at(0));

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to enumerate physical devices present.\n");

        // Need at least 1 device.
        const bool isOneDevicePresent = physicalDevices.size() > 0;

        DBG_ASSERT(isOneDevicePresent);

        // Use the first available device.
        _vkPhysicalDevice = physicalDevices.at(0);

        // Print out details of all found devices.
        for (VkPhysicalDevice &rDevice : physicalDevices)
        {
            VkPhysicalDeviceProperties deviceProperties{};
            const uint32_t deviceApiVersion = deviceProperties.apiVersion;
            ZeroMemory(&deviceProperties, sizeof(VkPhysicalDeviceProperties));

            // Fill structure with device info.
            vkGetPhysicalDeviceProperties(rDevice, &deviceProperties);

            // Print out device properties.
            DebugHelpers::DPrintf("Driver Version: %d\n", deviceProperties.driverVersion);
            DebugHelpers::DPrintf("Device Name: %s\n", deviceProperties.deviceName);
            DebugHelpers::DPrintf("Device Type: %d\n", deviceProperties.deviceType);
            DebugHelpers::DPrintf("API Version: %d.%d.%d\n", 
                                  VK_API_VERSION_MAJOR(deviceApiVersion),
                                  VK_API_VERSION_MAJOR(deviceApiVersion),
                                  VK_API_VERSION_PATCH(deviceApiVersion));
        }
    }

    void Application::SetupVulkanDevice()
    {
        VkPhysicalDeviceMemoryProperties deviceMemoryProperties{};

        vkGetPhysicalDeviceMemoryProperties(_vkPhysicalDevice, 
                                            &deviceMemoryProperties);

        // Initialise Queues
        const float queueProperties[] = { 1.0f };
        const VkDeviceQueueCreateInfo queueCreateInfo
        {
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = 0,  // Use first queue family in list
            .queueCount       = 1,
            .pQueuePriorities = queueProperties
        };

        const char *deviceExtensions[]{ "VK_KHR_swapchain" };
        const VkPhysicalDeviceFeatures deviceFeatures
        {
            .shaderClipDistance = VK_TRUE,
        };

        const VkDeviceCreateInfo deviceCreateInfo
        {
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = &queueCreateInfo,
            .enabledLayerCount       = ENABLED_LAYER_COUNT,
            .ppEnabledLayerNames     = _enabledLayerNames.data(),
            .enabledExtensionCount   = 1,
            .ppEnabledExtensionNames = deviceExtensions,
            .pEnabledFeatures        = &deviceFeatures
        };

        // Ideally, want to enumerate to find best device.
        // Just use the first for now.
        VkResult result = vkCreateDevice(_vkPhysicalDevice,
                                         &deviceCreateInfo,
                                         NULL, 
                                         &_vkLogicalDevice);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Logical Device!");
    }

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
    void Application::SetupVulkanInstance_InitVkDebugCallback()
    {
        // Register error logging function
        VkDebugReportCallbackEXT warningCallback{ VK_NULL_HANDLE }, errorCallback{ VK_NULL_HANDLE };
        PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT{ NULL };

        *(void **)&vkCreateDebugReportCallbackEXT = vkGetInstanceProcAddr(_vkInstance, "vkCreateDebugReportCallbackEXT");

        DBG_ASSERT(vkCreateDebugReportCallbackEXT);

        VulkanDebugReportCallback_t pVulkanDebugReportCallback = &Application::VulkanDebugReportCallback;

        // Capture errors
        VkDebugReportCallbackCreateInfoEXT callbackCreateInfo
        {
            .sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT,
            .pfnCallback = (PFN_vkDebugReportCallbackEXT)&pVulkanDebugReportCallback
        };

        VkResult result = vkCreateDebugReportCallbackEXT(_vkInstance,
                                                         &callbackCreateInfo,
                                                         nullptr,
                                                         &errorCallback);

        DBG_ASSERT_VULKAN_MSG(result, "vkCreateDebugReportCallbackEXT (ERROR) failed.");

        // Capture warnings
        callbackCreateInfo.flags       = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        callbackCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)&pVulkanDebugReportCallback;

        result = vkCreateDebugReportCallbackEXT(_vkInstance,
                                                &callbackCreateInfo,
                                                nullptr,
                                                &warningCallback);

        DBG_ASSERT_VULKAN_MSG(result, "vkCreateDebugReportCallbackEXT (WARNING) failed.");
    }

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
}
