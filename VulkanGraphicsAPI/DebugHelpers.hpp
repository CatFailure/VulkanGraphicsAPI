#pragma once
#define WIN32_LEAN_AND_MEAN

#if _DEBUG || _DEBUG_LAPTOP 
#define ENABLE_LOGGING        // Debug Logging
#endif

#include <stdarg.h>             // Variable argument functions, e.g., dprintf(..)
#include <stdio.h>              // vsprintf_s
#include <windows.h>            // OutputDebugStringA
#include <vulkan/vulkan.hpp>

namespace Utility
{
    // DebugBreak() (Win32) causes Visual Studio to halt.
    // Debug defines
#ifdef _WIN32
#define DBG_ASSERT(f)           \
            if (!f)             \
            {                   \
                __debugbreak(); \
            }
#else
#define DBG_ASSERT(f)       \
            if (!f)         \
            {               \
                assert(0);  \
            }
#endif // _WIN32

// NAN testing
#define DBG_VALID(f)                \
            if (f != f)             \
            {                       \
                DBG_ASSERT(false);  \
            }                       

#define DBG_ASSERT_MSG(val, errmsg)                   \
            if (!val)                                 \
            {                                         \
                DebugHelpers::DPrintf(errmsg);        \
                DBG_ASSERT(false);                    \
            }

#define DBG_ASSERT_VULKAN(val) \
            DBG_ASSERT(val == VK_SUCCESS);

#define DBG_ASSERT_VULKAN_MSG(val, errmsg)            \
            if (val != VK_SUCCESS)                    \
            {                                         \
                DebugHelpers::DPrintf(errmsg);        \
                DBG_ASSERT(false);                    \
            }

    class DebugHelpers
    {
    public:
        static void DPrintf(const char *format, ...);
        static void CheckVkResult(VkResult result) { DBG_ASSERT_VULKAN_MSG(result, "[vulkan] Error"); }
    };
}
