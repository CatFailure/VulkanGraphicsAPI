#pragma once
#define VK_USE_PLATFORM_WIN32_KHR

#include "ApplicationData.hpp"
#include "DebugHelpers.hpp"
#include "IDisposable.hpp"
#include "Cursor.hpp"

using namespace SolEngine::Interface;
using namespace SolEngine::Input;
using namespace Utility;

namespace SolEngine
{
    class SolWindow : public IDisposable
    {
    public:
        SolWindow(const std::string& winTitle, const glm::uvec2& winDimensions);
        ~SolWindow();

        bool ShouldClose()      const { return glfwWindowShouldClose(_pWindow); }
        bool WasWindowResized() const { return _isFramebufferResized; }
        void ResetWindowResizedFlag() { _isFramebufferResized = false; }

        void        CreateWindowSurface(const VkInstance& vkInstance, VkSurfaceKHR* pSurface);
        VkExtent2D  GetWindowExtent() const { return { _winDimensions.x, _winDimensions.y }; }
        GLFWwindow* GetWindow()       const { return _pWindow; }

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        static void FramebufferResizeCallback(GLFWwindow* pWindow, const int width, const int height);
        static void CursorPositionCallback(GLFWwindow* pWindow, const double xPos, const double yPos);
        static void CursorEnterCallback(GLFWwindow* pWindow, const int entered);
        static void MouseButtonCallback(GLFWwindow* pWindow, const int button, const int action, const int mods);
        static void FileDroppedCallback(GLFWwindow* pWindow, const int count, const char* filepaths[]);

        void CreateGLFWWindow();

        glm::uvec2 _winDimensions;                    // Window Dimensions
        bool       _isFramebufferResized{ false };    // Was the window resized?

        std::string _winTitle{ NULL };  // Application Window Title
        GLFWwindow* _pWindow { NULL };  // Application Window
    };
}
