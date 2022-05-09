#include "SolWindow.hpp"

namespace SolEngine
{
    SolWindow::SolWindow(const std::string& winTitle, 
                         const glm::uvec2& winDimensions, 
                         UserSettings& rSettings)
        : _winTitle(winTitle),
          _winDimensions(winDimensions),
          _rSettings(rSettings)
    {
        CreateGLFWWindow();
    }

    SolWindow::~SolWindow()
    {
        Dispose();
    }

    void SolWindow::CreateWindowSurface(const VkInstance& vkInstance, VkSurfaceKHR* pSurface)
    {
        const VkResult result = glfwCreateWindowSurface(vkInstance, 
                                                        _pWindow, 
                                                        NULL,
                                                        pSurface);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Window Surface.");
    }

    void SolWindow::Dispose()
    {
        glfwDestroyWindow(_pWindow);
        glfwTerminate();
    }

    void SolWindow::FramebufferResizeCallback(GLFWwindow* pWindow, 
                                              const int width, 
                                              const int height)
    {
        SolWindow* pSolWindow = reinterpret_cast<SolWindow*>(glfwGetWindowUserPointer(pWindow));

        pSolWindow->_isFramebufferResized = true;
        pSolWindow->_winDimensions = { width, height };
    }

    void SolWindow::CursorPositionCallback(GLFWwindow* pWindow, 
                                           const double xPos, 
                                           const double yPos)
    {
        Cursor& rCursor = Cursor::GetInstance();

        if (!rCursor.isMouseEntered)
        {
            return;
        }

        rCursor.mousePosition = { xPos, yPos };
    }

    void SolWindow::CursorEnterCallback(GLFWwindow* pWindow, 
                                        const int entered)
    {
        Cursor::GetInstance().isMouseEntered = entered;
    }

    void SolWindow::MouseButtonCallback(GLFWwindow* pWindow,
                                        const int button,
                                        const int action,
                                        const int mods)
    {
        Cursor& rCursor = Cursor::GetInstance();

        if (!rCursor.isMouseEntered)
        {
            return;
        }

        const bool isButtonPressed = action == GLFW_PRESS;

        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            rCursor.mouseButtons[(size_t)MouseButton::LEFT] = isButtonPressed;
            return;
        case GLFW_MOUSE_BUTTON_RIGHT:
            rCursor.mouseButtons[(size_t)MouseButton::RIGHT] = isButtonPressed;
            return;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            rCursor.mouseButtons[(size_t)MouseButton::MIDDLE] = isButtonPressed;
            return;
        default:
            return;
        }
    }

    void SolWindow::FileDroppedCallback(GLFWwindow* pWindow, 
                                        const int count, 
                                        const char* filepaths[])
    {
        if (count != MAX_FILE_DROP_COUNT)
        {
            return;
        }

        SolWindow* pSolWindow = reinterpret_cast<SolWindow*>(glfwGetWindowUserPointer(pWindow));

        SettingsFileLoader::GetInstance().LoadSettingsFromFile(filepaths[0], 
                                                               &pSolWindow->_rSettings);

#ifdef LAPTOP_BUILD
        // Toggle the simulation to play automatically on laptop builds (since there's no GUI to start it manually)
        pSolWindow->_rSettings.simulationSettings.state = SimulationState::PLAY;
#endif // LAPTOP_BUILD
    }

    void SolWindow::CreateGLFWWindow()
    {
        glfwInit();                                    // Initialise GLFW Library
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // Don't create OpenGL Context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);     // Allow window resizing

        _pWindow = glfwCreateWindow(_winDimensions.x,
                                    _winDimensions.y, 
                                    _winTitle.c_str(), 
                                    nullptr, 
                                    nullptr);

        glfwSetWindowUserPointer(_pWindow, this);
        glfwSetFramebufferSizeCallback(_pWindow, FramebufferResizeCallback);    // Bind Resize Callback
        glfwSetCursorPosCallback(_pWindow, CursorPositionCallback);             // Bind Mouse Position Callback
        glfwSetCursorEnterCallback(_pWindow, CursorEnterCallback);              // Bind Mouse Enter Callback
        glfwSetMouseButtonCallback(_pWindow, MouseButtonCallback);              // Bind Mouse Button Callback
        glfwSetDropCallback(_pWindow, FileDroppedCallback);                     // Bind File Dropped Callback
    }
}