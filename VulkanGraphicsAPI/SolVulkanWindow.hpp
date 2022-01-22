#pragma once
#include "ApplicationData.hpp"
#include "Vector.hpp"

using namespace SolEngine::Interface;
using namespace SolEngine::Math;
using namespace Utility;

namespace SolEngine
{
    class SolVulkanWindow : public IDisposable
    {
    public:
        SolVulkanWindow(const std::string &winTitle, const Vector2u &winDimensions);
        ~SolVulkanWindow();

        bool ShouldClose()      const { return glfwWindowShouldClose(_pWindow); }
        bool WasWindowResized() const { return _isFramebufferResized; }
        void ResetWindowResizedFlag() { _isFramebufferResized = false; }

        void       CreateWindowSurface(const VkInstance &vkInstance, VkSurfaceKHR *pSurface);
        VkExtent2D GetWindowExtent() const { return { _winDimensions._x, _winDimensions._y }; }

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        static void FramebufferResizeCallback(GLFWwindow *pWindow, const int width, const int height);

        void CreateGLFWWindow();

        Vector2u _winDimensions;
        bool     _isFramebufferResized{ false };

        std::string _winTitle{ NULL };
        GLFWwindow *_pWindow{ NULL };
    };
}
