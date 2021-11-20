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


// Win32 Entry point
int WINAPI WinMain(HINSTANCE hInstance,       // Handle to base address of the exe memory image
                   HINSTANCE hPrevInstance,   // Previous instance handle - Always 0
                   LPSTR lpCmdLine,           // Cmd line args (Alt. use GetCommandLine API Call)
                   int nCmdShow)              // How should the window appear when created (Alt. use GetStartupInfo API Call)
{
    // Requested window size.
    const int width(800), height(600);
    const LPCWSTR className = L"MyWindowClass";

    // Win32 handle identifier for use later 
    // when setting up vulkan surfaces.
    HWND windowHandle = NULL;
    SetupWin32Window(width, height, &windowHandle);

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