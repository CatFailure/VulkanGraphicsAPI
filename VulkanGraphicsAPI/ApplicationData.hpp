#pragma once
#define WIN32_LEAN_AND_MEAN
#define GLFW_INCLUDE_VULKAN

#include <windows.h>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

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
