#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS	// Make glm functions expect angles in radians instead of degrees
#define NOMINMAX			// Prevent Windows.h interacting with limits

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <stdlib.h>     // Variable argument functions, e.g., dprintf(..)
#include <stdarg.h>
#include <windows.h>    // OutputDebugStringA
#include <stdio.h>      // vsprintf_s
#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <cmath>
#include <limits>

#include <vector>
#include <array>
#include <string>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <deque>

#include "DebugHelpers.hpp"
#include "IDisposable.hpp"
#include "Singleton.hpp"
#include "SimplePushConstantData.hpp"
#include "PipelineConfigInfo.hpp"
#include "Vertex.hpp"