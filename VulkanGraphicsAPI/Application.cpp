#include "pch.hpp"
#include "Application.hpp"

namespace Engine
{
    Application::Application(const ApplicationData &appData)
        : _appData(appData)
    {}

    void Application::Dispose()
    {
        vkDestroyInstance(_vkInstance, NULL);
    }

    void Application::Initialise(const WNDPROC wndProcCallback)
    {
        SetupWin32Window(wndProcCallback);
        SetupVulkanInstance();
        SetupVulkanPhysicalDevices();
        SetupVulkanDevice();
        SetupVulkanSwapchain();
        PrintDeviceMemoryCapabilities();
        SetupVulkanDrawCommandBuffer();
    }

    void Application::SetupWin32Window(const WNDPROC wndProcCallback)
    {
        // Defines window properties
        HINSTANCE hInst{ GetModuleHandle(NULL) };
        HWND &rWinHandle = _appData.windowHandle;
        LPCWSTR className{ L"VulkanWindowClass" }, windowTitle{ L"Hello Vulkan!" };

        WNDCLASSEX windowClass{};
        ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

        windowClass.cbSize        = sizeof(WNDCLASSEX);                 // Size of struct in bytes
        windowClass.style         = CS_OWNDC | CS_VREDRAW | CS_HREDRAW; // Window Styles
        windowClass.lpfnWndProc   = wndProcCallback;                    // Callback func for message handling
        windowClass.hInstance     = hInst;                              // File Handle
        windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);        // Cursor type
        windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;               // Preferred Window colour
        windowClass.lpszClassName = className;                          // Class name

        RegisterClassEx(&windowClass);

        rWinHandle = CreateWindowEx(NULL,                                // No additional Window Styles
                                    className,                           // Class Name
                                    windowTitle,                         // Window Title
                                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,    // Window Appearance Styles
                                    100,                                 // x
                                    100,                                 // y
                                    _appData.clientWidth,                // Window width
                                    _appData.clientHeight,               // Window height
                                    NULL,                                // No Parents
                                    NULL,                                // No popup menus
                                    hInst,                               // Exe File Handle
                                    NULL);                               // No params to pass

// Was this successful?
        DBG_ASSERT(rWinHandle != NULL);
    }

    void Application::SetupVulkanInstance()
    {
        SetupVulkanInstance_InitVkInstance();

        // Optional - Setup debug callbacks so Vulkan 
        // can tell us if something has gone wrong.
#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        SetupVulkanInstance_InitVkDebugCallback();
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        SetupVulkanInstance_InitVkSurface();
    }

    void Application::SetupVulkanInstance_InitVkInstance()
    {
        uint32_t extensionCount(0);

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        const char *extensions[]
        {
            "VK_KHR_surface",
            "VK_KHR_win32_surface",
            "VK_EXT_debug_report",
        };

        extensionCount = 3;
#else
        const char *extensions[]
        {
            "VK_KHR_surface",
            "VK_KHR_win32_surface",
        };

        extensionCount = 2;
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        const char *appName = "Hello Vulkan";

        // Information about the application
        // to pass to the Vulkan driver.
        // Fill out application description
        VkApplicationInfo applicationInfo
        {
            .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,		// Mandatory, describes type of structure
            .pNext            = NULL,									// Mandatory, stores pointers to extension-specific structures
            .pApplicationName = appName,								// Name of the application
            .pEngineName      = NULL,									// Name of the engine
            .engineVersion    = 1,										// Version of the engine
            .apiVersion       = VK_API_VERSION_1_0						// Version of Vulkan used by application
        };

        // Fill out instance description
        VkInstanceCreateInfo instanceCreateInfo
        {
            .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	// Mandatory
            .pNext                   = NULL,									// Mandatory set
            .flags                   = 0,										// Mandatory set
            .pApplicationInfo        = &applicationInfo,						// Pass application info instance
            .enabledLayerCount       = ENABLED_LAYER_COUNT,					    // Number of enabled layers
            .ppEnabledLayerNames     = _enabledLayerNames.data(),               // Specified layer names
            .enabledExtensionCount   = extensionCount,							// Number of enabled extensions
            .ppEnabledExtensionNames = extensions,                              // Specified extension names
        };

        VkResult result = vkCreateInstance(&instanceCreateInfo,
                                           NULL,
                                           &_vkInstance);

        // Was creation successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Vulkan instance.");

        // Is instance handle valid?
        DBG_ASSERT(_vkInstance != NULL);
    }

    void Application::SetupVulkanInstance_InitVkSurface()
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo
        {
            .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .hinstance = hInstance,     // Param is NULL - GetModuleHandle returns handle to file used to create the calling process
            .hwnd      = _appData.windowHandle
        };

        // OutSurface must be empty before assignment
        DBG_ASSERT(_vkSurface == NULL);

        VkResult result = vkCreateWin32SurfaceKHR(_vkInstance,
                                                  &surfaceCreateInfo,
                                                  NULL,
                                                  &_vkSurface);

        DBG_ASSERT_VULKAN_MSG(result, "Could not create Surface.\n");

        DBG_ASSERT(_vkSurface != NULL);
    }

    void Application::SetupVulkanPhysicalDevices()
    {
        // Query how many devices are present
        VkResult result = vkEnumeratePhysicalDevices(_vkInstance,
                                                     &_physDeviceCount,
                                                     NULL);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to query the number of physical devices present.\n");

        // Make sure at least 1 device is present.
        DBG_ASSERT_MSG(_physDeviceCount != 0, "Could not detect any physical devices present with Vulkan support.\n");

        std::vector<VkPhysicalDevice> physicalDevices(_physDeviceCount);

        result = vkEnumeratePhysicalDevices(_vkInstance,
                                            &_physDeviceCount,
                                            &physicalDevices.at(0));

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to enumerate physical devices present.\n");

        // Need at least 1 device.
        const bool isOneDevicePresent = physicalDevices.size() > 0;

        DBG_ASSERT(isOneDevicePresent);

        // Use the first available device.
        _vkPhysicalDevice = physicalDevices.at(0);

        // Print out details of all found devices.
        for (VkPhysicalDevice &rDevice : physicalDevices)
        {
            VkPhysicalDeviceProperties deviceProperties{};
            const uint32_t deviceApiVersion = deviceProperties.apiVersion;
            ZeroMemory(&deviceProperties, sizeof(VkPhysicalDeviceProperties));

            // Fill structure with device info.
            vkGetPhysicalDeviceProperties(rDevice, &deviceProperties);

            // Print out device properties.
            DebugHelpers::DPrintf("Driver Version: %d\n", deviceProperties.driverVersion);
            DebugHelpers::DPrintf("Device Name: %s\n", deviceProperties.deviceName);
            DebugHelpers::DPrintf("Device Type: %d\n", deviceProperties.deviceType);
            DebugHelpers::DPrintf("API Version: %d.%d.%d\n", 
                                  VK_API_VERSION_MAJOR(deviceApiVersion),
                                  VK_API_VERSION_MAJOR(deviceApiVersion),
                                  VK_API_VERSION_PATCH(deviceApiVersion));
        }
    }

    void Application::SetupVulkanDevice()
    {
        VkPhysicalDeviceMemoryProperties deviceMemoryProperties{};

        vkGetPhysicalDeviceMemoryProperties(_vkPhysicalDevice, 
                                            &deviceMemoryProperties);

        // Initialise Queues
        const float queueProperties[] = { 1.0f };
        const VkDeviceQueueCreateInfo queueCreateInfo
        {
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = 0,  // Use first queue family in list
            .queueCount       = 1,
            .pQueuePriorities = queueProperties
        };

        const char *deviceExtensions[]{ "VK_KHR_swapchain" };
        const VkPhysicalDeviceFeatures deviceFeatures
        {
            .shaderClipDistance = VK_TRUE,
        };

        const VkDeviceCreateInfo deviceCreateInfo
        {
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = &queueCreateInfo,
            .enabledLayerCount       = ENABLED_LAYER_COUNT,
            .ppEnabledLayerNames     = _enabledLayerNames.data(),
            .enabledExtensionCount   = 1,
            .ppEnabledExtensionNames = deviceExtensions,
            .pEnabledFeatures        = &deviceFeatures
        };

        // Ideally, want to enumerate to find best device.
        // Just use the first for now.
        VkResult result = vkCreateDevice(_vkPhysicalDevice,
                                         &deviceCreateInfo,
                                         NULL, 
                                         &_vkLogicalDevice);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Logical Device!");
    }

    void Application::SetupVulkanSwapchain()
    {
        SetupVulkanSwapchain_CreateSwapchain();
        SetupVulkanSwapchain_CreateImages();
        SetupVulkanSwapchain_CreateImageViews();
    }

    void Application::SetupVulkanSwapchain_CreateSwapchain()
    {
        // Structure listing surface capabilities
        VkSurfaceCapabilitiesKHR surfaceCapabilities{};
        
        // Fill structure with data
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_vkPhysicalDevice, 
                                                  _vkSurface, 
                                                  &surfaceCapabilities);

        // Retrieve and use the actual back/front buffer widths
        VkExtent2D surfaceResolution = surfaceCapabilities.currentExtent;
        _surfaceBufferWidth = surfaceResolution.width;
        _surfaceBufferHeight = surfaceResolution.height;

        VkSwapchainCreateInfoKHR swapchainCreateInfo
        {
            .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface          = _vkSurface,
            .minImageCount    = SWAPCHAIN_BUFFER_COUNT,
            .imageFormat      = VK_FORMAT_B8G8R8A8_UNORM,
            .imageColorSpace  = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
            .imageExtent      = surfaceResolution,
            .imageArrayLayers = 1,
            .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .preTransform     = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode      = VK_PRESENT_MODE_MAILBOX_KHR,
            .clipped          = true,    // Clipping outside of extents?
            .oldSwapchain     = NULL
        };

        VkResult result = vkCreateSwapchainKHR(_vkLogicalDevice, 
                                               &swapchainCreateInfo, 
                                               NULL, 
                                               &_vkSwapchain);

        // Was this successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Swapchain.");
    }

    void Application::SetupVulkanSwapchain_CreateImages()
    {
        // How many images need to be created?
        vkGetSwapchainImagesKHR(_vkLogicalDevice, 
                                _vkSwapchain, 
                                &_swapChainImageCount,
                                NULL);

        // Make sure the images match what is expected
        const bool isCorrectSwapChainImageCount = _swapChainImageCount == SWAPCHAIN_BUFFER_COUNT;
        DBG_ASSERT(isCorrectSwapChainImageCount);

        _vkSwapchainImages = std::vector<VkImage>(_swapChainImageCount);
        
        // Link the images to the Swapchain
        VkResult result = vkGetSwapchainImagesKHR(_vkLogicalDevice,
                                                  _vkSwapchain,
                                                  &_swapChainImageCount,
                                                  _vkSwapchainImages.data());

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Swapchain Images.");
    }

    void Application::SetupVulkanSwapchain_CreateImageViews()
    {
        _vkSwapchainImageViews = std::vector<VkImageView>(SWAPCHAIN_BUFFER_COUNT);

        for (uint32_t i(0); i < SWAPCHAIN_BUFFER_COUNT; ++i)
        {
            // Create VkImageViews for the Swapchain
            VkImageViewCreateInfo imageViewCreateInfo
            {
                .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image    = _vkSwapchainImages.at(i),
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format   = VK_FORMAT_B8G8R8A8_UNORM,
                .components
                {
                    .r = VK_COMPONENT_SWIZZLE_R,
                    .g = VK_COMPONENT_SWIZZLE_G,
                    .b = VK_COMPONENT_SWIZZLE_B,
                    .a = VK_COMPONENT_SWIZZLE_A
                },
                .subresourceRange
                {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel   = 0,
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = 1
                }
            };

            VkResult result = vkCreateImageView(_vkLogicalDevice, 
                                                &imageViewCreateInfo,
                                                NULL, 
                                                &_vkSwapchainImageViews.at(i));

            // Was it successful?
            DBG_ASSERT_VULKAN_MSG(result, "Failed to create ImageView");
        }
    }

    void Application::PrintDeviceMemoryCapabilities()
    {
        // Query device for memory count
        vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, 
                                                 &_physDeviceQueueFamilyCount,
                                                 NULL);

        // Create vector of the size of the family count
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(_physDeviceQueueFamilyCount);

        // Retrieve properties
        vkGetPhysicalDeviceQueueFamilyProperties(_vkPhysicalDevice, 
                                                 &_physDeviceQueueFamilyCount,
                                                 &queueFamilyProperties.at(0));

        // Print out the families
        for (uint32_t i(0); i < _physDeviceCount; ++i)
        {
            for (uint32_t j(0); j < _physDeviceQueueFamilyCount; ++j)
            {
                const VkQueueFamilyProperties& currentQueueFamilyProperties = queueFamilyProperties.at(j);
                const VkQueueFlags& queueFlags = currentQueueFamilyProperties.queueFlags;

                DebugHelpers::DPrintf("Queue count: %d\n", currentQueueFamilyProperties.queueCount);
                DebugHelpers::DPrintf("Supporting operating on this queue:\n");

                if (queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    DebugHelpers::DPrintf("\t\tGraphics\n");
                }

                if (queueFlags & VK_QUEUE_COMPUTE_BIT)
                {
                    DebugHelpers::DPrintf("\t\tCompute\n");
                }

                if (queueFlags & VK_QUEUE_TRANSFER_BIT)
                {
                    DebugHelpers::DPrintf("\t\tTransfer\n");
                }

                if (queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
                {
                    DebugHelpers::DPrintf("\t\tSparse Binding\n");
                }
            }
        }
    }

    void Application::SetupVulkanDrawCommandBuffer()
    {
        // Queue address
        VkQueue logicalDeviceQueue{ NULL };
        const uint32_t queueFamilyIndex = 0;

        // Request the device queue to submit work to
        vkGetDeviceQueue(_vkLogicalDevice, 
                         queueFamilyIndex,
                         0, 
                         &logicalDeviceQueue);

        // Create Command Pool and Buffers
        VkCommandPoolCreateInfo commandPoolCreateInfo
        {
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = queueFamilyIndex
        };

        VkResult result = vkCreateCommandPool(_vkLogicalDevice, 
                                              &commandPoolCreateInfo, 
                                              NULL, 
                                              &_vkCommandPool);

        // Was is successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Command Pool.");

        // Define type of Command Buffer
        VkCommandBufferAllocateInfo commandBufferAllocateInfo
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = _vkCommandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = _commandBufferCount
        };

        result = vkAllocateCommandBuffers(_vkLogicalDevice, 
                                          &commandBufferAllocateInfo, 
                                          &_vkDrawCommandBuffer);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to allocate Draw Command Buffer.")
    }

    void Application::SetupFrameBufferRenderPass()
    {
#ifdef DEPTH_BUFFER
        // (Optional) - Setup a depth buffer
        VkImage depthImage{ NULL };
        VkImageView depthImageView{ NULL };

        // Create the Depth Image
        VkImageCreateInfo depthImageCreateInfo
        {
            .sType     = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = VK_IMAGE_TYPE_2D,
            .format    = VK_FORMAT_D16_UNORM,
            .extent 
            {
                .width  = _surfaceBufferWidth,
                .height = _surfaceBufferHeight,
                .depth  = 1
            },
            .mipLevels   = 1,
            .arrayLayers = 1,
            .samples               = VK_SAMPLE_COUNT_1_BIT,
            .tiling                = VK_IMAGE_TILING_OPTIMAL,
            .usage                 = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .sharingMode           = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices   = NULL,
            .initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED
        };

        VkResult result = vkCreateImage(_vkLogicalDevice, 
                                        &depthImageCreateInfo, 
                                        NULL, 
                                        &depthImage);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Depth Image.");
#endif // DEPTH_BUFFER
    }

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
    void Application::SetupVulkanInstance_InitVkDebugCallback()
    {
        // Register error logging function
        VkDebugReportCallbackEXT warningCallback{ VK_NULL_HANDLE }, errorCallback{ VK_NULL_HANDLE };
        PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT{ NULL };

        *(void **)&vkCreateDebugReportCallbackEXT = vkGetInstanceProcAddr(_vkInstance, "vkCreateDebugReportCallbackEXT");

        DBG_ASSERT(vkCreateDebugReportCallbackEXT);

        VulkanDebugReportCallback_t pVulkanDebugReportCallback = &Application::VulkanDebugReportCallback;

        // Capture errors
        VkDebugReportCallbackCreateInfoEXT callbackCreateInfo
        {
            .sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT,
            .pfnCallback = (PFN_vkDebugReportCallbackEXT)&pVulkanDebugReportCallback
        };

        VkResult result = vkCreateDebugReportCallbackEXT(_vkInstance,
                                                         &callbackCreateInfo,
                                                         nullptr,
                                                         &errorCallback);

        DBG_ASSERT_VULKAN_MSG(result, "vkCreateDebugReportCallbackEXT (ERROR) failed.");

        // Capture warnings
        callbackCreateInfo.flags       = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        callbackCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)&pVulkanDebugReportCallback;

        result = vkCreateDebugReportCallbackEXT(_vkInstance,
                                                &callbackCreateInfo,
                                                nullptr,
                                                &warningCallback);

        DBG_ASSERT_VULKAN_MSG(result, "vkCreateDebugReportCallbackEXT (WARNING) failed.");
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Application::VulkanDebugReportCallback(VkDebugReportFlagsEXT flags,
                                                                          VkDebugReportObjectTypeEXT objectType,
                                                                          uint64_t object,
                                                                          size_t location,
                                                                          int32_t messageCode,
                                                                          const char *layerPrefix,
                                                                          const char *message,
                                                                          void *pUserData)
    {
        DebugHelpers::DPrintf(layerPrefix);
        DebugHelpers::DPrintf(" ");
        DebugHelpers::DPrintf(message);
        DebugHelpers::DPrintf("\n");

        DBG_ASSERT(false);

        return VK_FALSE;
    }
#endif // ENABLE_VULKAN_DEBUG_CALLBACK
}
