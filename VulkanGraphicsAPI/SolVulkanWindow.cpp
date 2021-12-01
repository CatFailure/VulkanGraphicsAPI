#include "pch.hpp"
#include "SolVulkanWindow.hpp"

namespace SolEngine
{
    SolVulkanWindow::SolVulkanWindow(const std::string &winTitle, 
                                     const Vector2<uint32_t> &winDimensions)
        : _winTitle(winTitle),
          _winDimensions(winDimensions)
    {
        CreateGLFWWindow();
    }

    SolVulkanWindow::~SolVulkanWindow()
    {
        Dispose();
    }

    void SolVulkanWindow::CreateWindowSurface(const VkInstance &vkInstance, VkSurfaceKHR *pSurface)
    {
        const VkResult result = glfwCreateWindowSurface(vkInstance, _pWindow, NULL, pSurface);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Window Surface.");
    }

    void SolVulkanWindow::Dispose()
    {
        glfwDestroyWindow(_pWindow);
        glfwTerminate();
    }

    void SolVulkanWindow::FramebufferResizeCallback(GLFWwindow *pWindow, 
                                                    const int width, 
                                                    const int height)
    {
        SolVulkanWindow *pVulkanWindow = reinterpret_cast<SolVulkanWindow *>(glfwGetWindowUserPointer(pWindow));

        pVulkanWindow->_isFramebufferResized = true;
        pVulkanWindow->_winDimensions = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
    }

    void SolVulkanWindow::CreateGLFWWindow()
    {
        glfwInit();										// Initialise the GLFW library
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	// Tell GLFW to not create an OpenGL context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);		// Disable Window Resizing, needs handling in a special way

        _pWindow = glfwCreateWindow(_winDimensions._x, 
                                    _winDimensions._y,
                                    _winTitle.c_str(), 
                                    NULL, 
                                    NULL);

        glfwSetWindowUserPointer(_pWindow, this);
        glfwSetFramebufferSizeCallback(_pWindow, FramebufferResizeCallback);
    }
}