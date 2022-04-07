#pragma once
#include <glm/glm.hpp>
#include "Singleton.hpp"
#include "MouseButton.hpp"

using namespace Utility;
using namespace SolEngine::Enumeration;

namespace SolEngine::Input
{
    struct Cursor : Singleton<Cursor>
    {
        void UpdateLastMousePosition()
        {
            // Record the mouse position at the end of the frame
            lastMousePosition = mousePosition;
        }

        bool       IsButtonDown(const MouseButton button) const { return mouseButtons[(size_t)button]; }
        glm::dvec2 GetMouseDelta()                        const { return mousePosition - lastMousePosition; }

        bool       isMouseEntered                          { false };
        bool       mouseButtons[(size_t)MouseButton::COUNT]{ false };

        glm::dvec2 lastMousePosition{ 0.f };
        glm::dvec2 mousePosition    { 0.f };
    };
}
