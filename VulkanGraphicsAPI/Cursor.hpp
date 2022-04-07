#pragma once
#include "Singleton.hpp"
#include "MouseButton.hpp"

using namespace Utility;
using namespace SolEngine::Enumeration;

namespace SolEngine::Input
{
    struct Cursor : Singleton<Cursor>
    {
        bool       isMouseEntered                          { false };
        bool       mouseButtons[(size_t)MouseButton::COUNT]{ false };
        glm::dvec2 mousePosition                           { 0.f };

        bool isButtonDown(const MouseButton button) const { return mouseButtons[(size_t)button]; }
    };
}
