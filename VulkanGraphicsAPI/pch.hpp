#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

#include <iostream>
#include <stdlib.h>     // Variable argument functions, e.g., dprintf(..)
#include <stdarg.h>
#include <windows.h>    // OutputDebugStringA
#include <stdio.h>      // vsprintf_s
#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <utility>

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