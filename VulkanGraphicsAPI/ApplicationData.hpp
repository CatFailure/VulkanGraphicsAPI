#pragma once
#define WIN32_LEAN_AND_MEAN
#define GLFW_INCLUDE_VULKAN

#include <windows.h>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <filesystem>

using path = std::filesystem::path;

namespace SolEngine::Data
{
    // Information about the application window
    struct ApplicationData
    {
        const std::string windowTitle     { "Vulkan Window" };
        const LPCWSTR	  windowClassName { L"VulkanWindowClass" };
        const char*       engineName      { "Sol Engine" };
        const char*       appName         {"VulkanGraphicsAPI"};
        glm::uvec2        windowDimensions{ 800, 600 };
        path              exeDirectory    {};

        VkExtent2D GetExtent() const { return { windowDimensions.x, windowDimensions.y }; }
    };
}
