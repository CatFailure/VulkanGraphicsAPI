#pragma once
#include "ApplicationData.hpp"
#include "Vector.hpp"

using namespace SolEngine::Interface;
using namespace SolEngine::Math;
using namespace Utility;

namespace SolEngine
{
    class SolWindow : public IDisposable
    {
    public:
        SolWindow(const std::string &winTitle, const Vector2u &winDimensions);
        ~SolWindow();

        bool ShouldClose()      const { return glfwWindowShouldClose(_pWindow); }
        bool WasWindowResized() const { return _isFramebufferResized; }
        void ResetWindowResizedFlag() { _isFramebufferResized = false; }

        void        CreateWindowSurface(const VkInstance &vkInstance, VkSurfaceKHR *pSurface);
        VkExtent2D  GetWindowExtent() const { return { _winDimensions._x, _winDimensions._y }; }
        GLFWwindow *GetWindow()       const { return _pWindow; }

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        static void FramebufferResizeCallback(GLFWwindow *pWindow, const int width, const int height);

        void CreateGLFWWindow();

        Vector2u _winDimensions;                    // Window Dimensions
        bool     _isFramebufferResized{ false };    // Was the window resized?

        std::string _winTitle{ NULL };  // Application Window Title
        GLFWwindow *_pWindow{ NULL };   // Application Window
    };
}
