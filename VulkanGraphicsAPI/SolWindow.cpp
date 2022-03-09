#include "SolWindow.hpp"

namespace SolEngine
{
    SolWindow::SolWindow(const std::string &winTitle, 
                         const glm::uvec2 &winDimensions)
        : _winTitle(winTitle),
          _winDimensions(winDimensions)
    {
        CreateGLFWWindow();
    }

    SolWindow::~SolWindow()
    {
        Dispose();
    }

    void SolWindow::CreateWindowSurface(const VkInstance &vkInstance, VkSurfaceKHR *pSurface)
    {
        const VkResult result = glfwCreateWindowSurface(vkInstance, _pWindow, NULL, pSurface);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Window Surface.");
    }

    void SolWindow::Dispose()
    {
        glfwDestroyWindow(_pWindow);
        glfwTerminate();
    }

    void SolWindow::FramebufferResizeCallback(GLFWwindow *pWindow, 
                                              const int width, 
                                              const int height)
    {
        SolWindow *pSolWindow = reinterpret_cast<SolWindow *>(glfwGetWindowUserPointer(pWindow));

        pSolWindow->_isFramebufferResized = true;
        pSolWindow->_winDimensions = { width, height };
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
    }
}