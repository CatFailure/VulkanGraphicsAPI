#pragma once
#include "MouseButton.hpp"

using namespace SolEngine::Enumeration;

namespace SolEngine::Input
{
    struct Cursor
    {
        bool mouseButtons[(size_t)MouseButton::COUNT]{ false };

        bool isButtonDown(const MouseButton button) const { return mouseButtons[(size_t)button]; }
    };
}
