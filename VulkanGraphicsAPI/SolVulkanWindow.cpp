#include "pch.hpp"
#include "SolVulkanWindow.hpp"

namespace SolEngine
{
    SolVulkanWindow::SolVulkanWindow(const std::string &winTitle, 
                                     const Vector2<uint32_t> &winDimensions)
    {
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
    }

    void SolVulkanWindow::FramebufferResizeCallback(GLFWwindow *pWindow, 
                                                    const const Vector2<uint32_t> &newWinDimensions)
    {
        SolVulkanWindow *pVulkanWindow = reinterpret_cast<SolVulkanWindow *>(glfwGetWindowUserPointer(pWindow));

        pVulkanWindow->_isFramebufferResized = true;
        pVulkanWindow->_winDimensions = newWinDimensions;
    }
}