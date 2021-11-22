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
    // Requested window size.
    const ApplicationData appData
    {
        .windowHandle = { NULL },
        .clientWidth  = 800,
        .clientHeight = 600
    };

    Application application;
    application.Initialise(appData, WndProc);

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
