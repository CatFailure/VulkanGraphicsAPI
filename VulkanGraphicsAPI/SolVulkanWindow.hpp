#pragma once
#include "ApplicationData.hpp"
#include "Vector.hpp"

using namespace Utility;
using namespace SolEngine::Math;

namespace SolEngine
{
    class SolVulkanWindow : public IDisposable
    {
    public:
        SolVulkanWindow(const std::string &winTitle, const Vector2<uint32_t> &winDimensions);
        ~SolVulkanWindow();

        VkExtent2D GetWindowExtent() const { return { _winDimensions._x, _winDimensions._y }; }
        bool ShouldClose() const { return glfwWindowShouldClose(_pWindow); }
        bool WasWindowResized() const { return _isFramebufferResized; }
        void ResetWindowResizedFlag() { _isFramebufferResized = false; }

        void CreateWindowSurface(const VkInstance &vkInstance, VkSurfaceKHR *pSurface);

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        static void FramebufferResizeCallback(GLFWwindow *pWindow, const const Vector2<uint32_t> &newWinDimensions);
        void CreateGLFWWindow();

        Vector2<uint32_t> _winDimensions;
        bool _isFramebufferResized{ false };

        std::string _winTitle{ NULL };
        GLFWwindow *_pWindow{ NULL };
    };
}
