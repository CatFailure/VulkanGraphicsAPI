#pragma once
#include "Vector.hpp"

using namespace SolEngine::Math;

namespace SolEngine::Data
{
    struct ApplicationData
    {
        const std::string windowTitle;
        const LPCWSTR	  windowClassName;
        const char		  *engineName;
        const char		  *appName;

        Vector2<uint32_t> windowDimensions;

        VkExtent2D GetExtent() { return { windowDimensions._x, windowDimensions._y }; }
    };
}
