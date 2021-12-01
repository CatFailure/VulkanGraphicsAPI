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
        glfwInit();                                    // Initialise GLFW Library
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // Don't create OpenGL Context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);     // Allow window resizing

        _pWindow = glfwCreateWindow(_winDimensions._x,
                                    _winDimensions._y, 
                                    _winTitle.c_str(), 
                                    nullptr, 
                                    nullptr);

        glfwSetWindowUserPointer(_pWindow, this);
        glfwSetFramebufferSizeCallback(_pWindow, FramebufferResizeCallback);    // Bind Resize Callback
    }
}