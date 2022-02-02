#pragma once

namespace SolEngine::Data
{
    struct ApplicationData
    {
        const std::string windowTitle;
        const LPCWSTR	  windowClassName;
        const char		  *engineName;
        const char		  *appName;

        glm::uvec2 windowDimensions;

        VkExtent2D GetExtent() const { return { windowDimensions.x, windowDimensions.y }; }
    };
}
