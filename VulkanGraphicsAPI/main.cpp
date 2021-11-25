#include "pch.hpp"
#include "Application.hpp"

using namespace Engine;

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

int WINAPI WinMain(HINSTANCE hInstance,       // Handle to base address of the exe memory image
                   HINSTANCE hPrevInstance,   // Previous instance handle - Always 0
                   LPSTR lpCmdLine,           // Cmd line args (Alt. use GetCommandLine API Call)
                   int nCmdShow)              // How should the window appear when created (Alt. use GetStartupInfo API Call)
{
    // Defines window properties
    HINSTANCE hInst{ GetModuleHandle(NULL) };
    HWND winHandle{ NULL };
    LPCWSTR className{ L"VulkanWindowClass" }, windowTitle{ L"Hello Vulkan!" };
    uint32_t clientWidth(800), clientHeight(600);

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

    winHandle = CreateWindowEx(NULL,                                // No additional Window Styles
                               className,                           // Class Name
                               windowTitle,                         // Window Title
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE,    // Window Appearance Styles
                               100,                                 // x
                               100,                                 // y
                               clientWidth,                         // Window width
                               clientHeight,                        // Window height
                               NULL,                                // No Parents
                               NULL,                                // No popup menus
                               hInst,                               // Exe File Handle
                               NULL);                               // No params to pass

    // Was this successful?
    DBG_ASSERT(winHandle != NULL);

    Application application(winHandle);

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

    application.Dispose();

    return 0;
}
