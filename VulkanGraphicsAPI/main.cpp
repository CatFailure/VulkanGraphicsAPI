#define ENABLE_VULKAN_DEBUG_CALLBACK
#include "DebugHelpers.hpp"

using namespace Utility;

LRESULT CALLBACK WndProc(HWND hWnd,
                         UINT uMsg,
                         WPARAM wParam,
                         LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags,
                                                         VkDebugReportObjectTypeEXT objectType, 
                                                         uint64_t object,
                                                         size_t location, 
                                                         int32_t messageCode, 
                                                         const char *pLayerPrefix, 
                                                         const char *pMessage,
                                                         void *pUserData)
{
    DebugHelpers::DPrintf(pLayerPrefix);
    DebugHelpers::DPrintf(" ");
    DebugHelpers::DPrintf(pMessage);
    DebugHelpers::DPrintf("\n");
    
    DBG_ASSERT(false);

    return VK_FALSE;
}
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

void SetupWin32Window(const int width,
                      const int height,
                      HWND *pOutWindowHandle)
{
    // Defines window properties
    HINSTANCE hInst{ GetModuleHandle(NULL) };
    LPCWSTR className{ L"VulkanWindowClass" }, windowTitle{ L"Hello Vulkan!" };

    WNDCLASSEX windowClass{};
    ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

    windowClass.cbSize        = sizeof(WNDCLASSEX);                 // Size of struct in bytes
    windowClass.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW; // Window Styles
    windowClass.lpfnWndProc   = WndProc;                            // Callback func for message handling
    windowClass.hInstance     = hInst;                              // File Handle
    windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);        // Cursor type
    windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;               // Preferred Window colour
    windowClass.lpszClassName = className;                          // Class name

    RegisterClassEx(&windowClass);

    *pOutWindowHandle = CreateWindowEx(NULL,                                // No additional Window Styles
                                       className,                           // Class Name
                                       windowTitle,                         // Window Title
                                       WS_OVERLAPPEDWINDOW | WS_VISIBLE,    // Window Appearance Styles
                                       100,                                 // x
                                       100,                                 // y
                                       width,                               // Window width
                                       height,                              // Window height
                                       NULL,                                // No Parents
                                       NULL,                                // No popup menus
                                       hInst,                               // Exe File Handle
                                       NULL);                               // No params to pass

    // Was this successful?
    DBG_ASSERT(*pOutWindowHandle != NULL);
}

void SetupVulkanInstance(HWND windowHandle,         // Win32 Handle
                         VkInstance *pOutInstance,  // Instance Handle
                         VkSurfaceKHR *pOutSurface) // Surface Handle
{
    const char *layers[] { "VK_LAYER_NV_optimus" };
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
        // Information about the application
        // to pass to the Vulkan driver.
        VkApplicationInfo applicationInfo
        {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Hello Vulkan",
            .engineVersion = 1,
            .apiVersion = VK_API_VERSION_1_0
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
                                           pOutInstance);

        // Was creation successful?
        DBG_ASSERT_VULKAN_MSG(result,
                              "Failed to create Vulkan instance.");

        // Is instance handle valid?
        DBG_ASSERT(*pOutInstance != NULL);
    }

    // Optional - Setup debug callbacks so Vulkan 
    // can tell us if something has gone wrong.
#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
    {
        // Register error logging function
        VkDebugReportCallbackEXT warningCallback{ VK_NULL_HANDLE }, errorCallback{ VK_NULL_HANDLE };
        PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT{ NULL };

        *(void **)&vkCreateDebugReportCallbackEXT = vkGetInstanceProcAddr(*pOutInstance,
                                                                          "vkCreateDebugReportCallbackEXT");

        DBG_ASSERT(vkCreateDebugReportCallbackEXT);

        // Capture errors
        VkDebugReportCallbackCreateInfoEXT callbackCreateInfo
        {
            .sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT,
            .pfnCallback = &VulkanDebugReportCallback
        };

        VkResult result = vkCreateDebugReportCallbackEXT(*pOutInstance,
                                                         &callbackCreateInfo,
                                                         nullptr,
                                                         &errorCallback);

        DBG_ASSERT_VULKAN_MSG(result,
                              "vkCreateDebugReportCallbackEXT (ERROR) failed.");

        // Capture warnings
        callbackCreateInfo.flags       = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        callbackCreateInfo.pfnCallback = &VulkanDebugReportCallback;

        result = vkCreateDebugReportCallbackEXT(*pOutInstance,
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
            .hwnd      = windowHandle
        };

        // OutSurface must be empty before assignment
        DBG_ASSERT(*pOutSurface == NULL);

        VkResult result = vkCreateWin32SurfaceKHR(*pOutInstance, 
                                                  &surfaceCreateInfo, 
                                                  NULL, 
                                                  pOutSurface);

        DBG_ASSERT_VULKAN_MSG(result, 
                              "Could not create Surface.");

        DBG_ASSERT(pOutSurface != NULL);
    }
}

// Win32 Entry point
int WINAPI WinMain(HINSTANCE hInstance,       // Handle to base address of the exe memory image
                   HINSTANCE hPrevInstance,   // Previous instance handle - Always 0
                   LPSTR lpCmdLine,           // Cmd line args (Alt. use GetCommandLine API Call)
                   int nCmdShow)              // How should the window appear when created (Alt. use GetStartupInfo API Call)
{
    // Requested window size.
    const int width(800), height(600);
    const LPCWSTR className(L"MyWindowClass");

    // Win32 handle identifier for use later 
    // when setting up vulkan surfaces.
    HWND windowHandle{ NULL };
    VkInstance pInstance{ NULL };
    VkSurfaceKHR psurface{ NULL };

    SetupWin32Window(width, height, &windowHandle);
    SetupVulkanInstance(windowHandle, &pInstance, &psurface);

    MSG msg{};      // Structure for storing Win32 Messages.
    while (true)    // Start of main render loop
    {
        PeekMessage(&msg,           // Receive message info
                    NULL,           // Handle to Window whose message are to be retrieved. NULL = Retrieves messages for any window.
                    NULL,           // wMessageFilterMin (NULL - No range filtering performed)
                    NULL,           // wMessageFilterMax (NULL - No range filtering performed)
                    PM_REMOVE);     // How messages are to be handled

        // Has the window been closed?
        if (msg.message == WM_QUIT)
        {
            break;
        }

        // Process messages (e.g. Keypresses, mouse inputs)
        // Translates any Virtual-Key messages.
        TranslateMessage(&msg);

        // Execute appropriate function
        DispatchMessage(&msg);
    }

    return 0;
}

//int main()
//{
//    const char *pAppName = "Hello Vulkan";
//
//    // Fill out application description
//    VkApplicationInfo applicationInfo
//    {
//        .sType			  = VK_STRUCTURE_TYPE_APPLICATION_INFO,		// Mandatory, describes type of structure
//        .pNext			  = NULL,									// Mandatory, stores pointers to extension-specific structures
//        .pApplicationName = pAppName,								// Name of the application
//        .pEngineName	  = NULL,									// Name of the engine
//        .engineVersion	  = 1,										// Version of the engine
//        .apiVersion		  = VK_API_VERSION_1_0						// Version of Vulkan used by application
//    };
//
//    // Fill out instance description
//    VkInstanceCreateInfo instanceCreateInfo
//    {
//        .sType					 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	// Mandatory
//        .pNext					 = NULL,									// Mandatory set
//        .flags					 = 0,										// Mandatory set
//        .pApplicationInfo		 = &applicationInfo,						// Pass application info instance
//        .enabledLayerCount		 = 0,										// Don't enable any layers
//        .ppEnabledLayerNames	 = NULL,
//        .enabledExtensionCount   = 0,										// Don't enable any extensions
//        .ppEnabledExtensionNames = NULL,
//    };
//
//    // Create desired instance
//    VkInstance instance = VK_NULL_HANDLE;
//    VkResult result = vkCreateInstance(&instanceCreateInfo, 
//                                       NULL, 
//                                       &instance);
//
//    vkDestroyInstance(instance, NULL);
//
//    return 0;
//}