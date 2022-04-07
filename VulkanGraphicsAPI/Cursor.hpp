#pragma once
#include "Singleton.hpp"
#include "MouseButton.hpp"

using namespace Utility;
using namespace SolEngine::Enumeration;

namespace SolEngine::Input
{
    struct Cursor : Singleton<Cursor>
    {
        bool mouseButtons[(size_t)MouseButton::COUNT]{ false };

        bool isButtonDown(const MouseButton button) const { return mouseButtons[(size_t)button]; }
    };
}
