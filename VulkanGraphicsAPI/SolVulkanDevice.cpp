#include "pch.hpp"
#include "SolVulkanDevice.hpp"

namespace SolEngine
{
    SolVulkanDevice::SolVulkanDevice(SolVulkanWindow &rSolVulkanWindow, 
                                     ApplicationData &rAppData)
        : _rAppData(rAppData),
          _rSolVulkanWindow(rSolVulkanWindow)
    {
        CreateVulkanInstance();
        CreateVulkanSurface();
        CreateVulkanPhysicalDevice();
        CreateVulkanDevice();

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        // Optional - Setup debug callbacks so Vulkan 
        // can tell us if something has gone wrong.
        CreateVulkanDebugCallback();
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        CreateVulkanCommandPool();
    }

    SolVulkanDevice::~SolVulkanDevice()
    {
        Dispose();
    }

    void SolVulkanDevice::CreateImageWithInfo(const VkImageCreateInfo &imageCreateInfo, 
                                              VkMemoryPropertyFlags properties, 
                                              VkImage &rImage, 
                                              VkDeviceMemory &rImageMemory)
    {
        VkResult result = vkCreateImage(_vkDevice, 
                                        &imageCreateInfo,
                                        NULL, 
                                        &rImage);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Image.");

        VkMemoryRequirements imageMemoryRequirements{};

        vkGetImageMemoryRequirements(_vkDevice, 
                                     rImage,
                                     &imageMemoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo
        {
            .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize  = imageMemoryRequirements.size,
            .memoryTypeIndex = FindMemoryType(imageMemoryRequirements.memoryTypeBits, 
                                              properties)
        };

        result = vkAllocateMemory(_vkDevice, 
                                  &memoryAllocateInfo,
                                  NULL,
                                  &rImageMemory);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to allocate Image Memory.");

        result = vkBindImageMemory(_vkDevice, 
                                   rImage,
                                   rImageMemory,
                                   0);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Bind Image Memory.");
    }

    uint32_t SolVulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties{};

        vkGetPhysicalDeviceMemoryProperties(_vkPhysicalDevice,
                                            &physicalDeviceMemoryProperties);

        for (uint32_t i(0); i < physicalDeviceMemoryProperties.memoryTypeCount; ++i)
        {
            if ((typeFilter & (1 << i)) &&
                (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        return -1;
    }

    VkFormat SolVulkanDevice::FindSupportedFormat(const std::vector<VkFormat> &candidates,
                                                  VkImageTiling tiling, 
                                                  VkFormatFeatureFlags features)
    {
        for (const VkFormat &format : candidates)
        {
            VkFormatProperties properties{};

            vkGetPhysicalDeviceFormatProperties(_vkPhysicalDevice, 
                                                format, 
                                                &properties);

            if ((tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features) ||
                (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features))
            {
                return format;
            }
        }

        DBG_ASSERT_MSG(true, "Failed to find supported format.");

        return VkFormat();
    }

    void SolVulkanDevice::Dispose()
    {
        vkDestroyCommandPool(_vkDevice, _vkCommandPool, NULL);
        vkDestroyDevice(_vkDevice, NULL);
        vkDestroySurfaceKHR(_vkInstance, _vkSurface, NULL);
        vkDestroyInstance(_vkInstance, NULL);
    }

    void SolVulkanDevice::CreateVulkanInstance()
	{
        _enabledExtensionNames =
        {
            "VK_KHR_surface",
            "VK_KHR_win32_surface",
        };

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
        _enabledExtensionNames.push_back("VK_EXT_debug_report");
#endif // ENABLE_VULKAN_DEBUG_CALLBACK

        // Information about the application
        // to pass to the Vulkan driver.
        // Fill out application description
        VkApplicationInfo applicationInfo
        {
            .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,	    // Mandatory, describes type of structure
            .pNext            = NULL,								    // Mandatory, stores pointers to extension-specific structures
            .pApplicationName = _rAppData.appName,					    // Name of the application
            .pEngineName      = _rAppData.engineName,								    // Name of the engine
            .engineVersion    = 1,									    // Version of the engine
            .apiVersion       = VK_API_VERSION_1_0					    // Version of Vulkan used by application
        };

        // Fill out instance description
        VkInstanceCreateInfo instanceCreateInfo
        {
            .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	              // Mandatory
            .pNext                   = NULL,									              // Mandatory set
            .flags                   = 0,										              // Mandatory set
            .pApplicationInfo        = &applicationInfo,						              // Pass application info instance
            .enabledLayerCount       = EnabledLayerCount(),	                                  // Number of enabled layers
            .ppEnabledLayerNames     = _enabledLayerNames.data(),                             // Specified layer names
            .enabledExtensionCount   = EnabledExtensionCount(),                               // Number of enabled extensions
            .ppEnabledExtensionNames = _enabledExtensionNames.data(),                         // Specified extension names
        };

        VkResult result = vkCreateInstance(&instanceCreateInfo,
                                           NULL,
                                           &_vkInstance);

        // Was creation successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Vulkan instance.");

        // Is instance handle valid?
        DBG_ASSERT(_vkInstance != NULL);
	}

    void SolVulkanDevice::CreateVulkanPhysicalDevice()
    {
        uint32_t physicalDeviceCount = 0;

        // Query how many devices are present
        VkResult result = vkEnumeratePhysicalDevices(_vkInstance,
                                                     &physicalDeviceCount,
                                                     NULL);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to query the number of physical devices present.\n");

        // Make sure at least 1 device is present.
        DBG_ASSERT_MSG(physicalDeviceCount != 0, "Could not detect any physical devices present with Vulkan support.\n");

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);

        result = vkEnumeratePhysicalDevices(_vkInstance,
                                            &physicalDeviceCount,
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
            ZeroMemory(&deviceProperties, sizeof(VkPhysicalDeviceProperties));

            // Fill structure with device info.
            vkGetPhysicalDeviceProperties(rDevice, &deviceProperties);
            const uint32_t deviceApiVersion = deviceProperties.apiVersion;

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

    void SolVulkanDevice::CreateVulkanDevice()
    {
        VkPhysicalDeviceMemoryProperties deviceMemoryProperties{};
        // Query and store physical device properties
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

        const VkPhysicalDeviceFeatures deviceFeatures
        {
            .shaderClipDistance = VK_TRUE,
        };

        const VkDeviceCreateInfo deviceCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = &queueCreateInfo,
            .enabledLayerCount       = EnabledLayerCount(),
            .ppEnabledLayerNames     = _enabledLayerNames.data(),
            .enabledExtensionCount   = DeviceExtensionCount(),
            .ppEnabledExtensionNames = _logicalDeviceExtensions.data(),
            .pEnabledFeatures        = &deviceFeatures
        };

        // Ideally, want to enumerate to find best device.
        // Just use the first for now.
        VkResult result = vkCreateDevice(_vkPhysicalDevice,
                                         &deviceCreateInfo,
                                         NULL,
                                         &_vkDevice);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Logical Device!");
    }

    void SolVulkanDevice::CreateVulkanCommandPool()
    {
        // Queue address
        VkQueue logicalDeviceQueue{ NULL };
        const uint32_t queueFamilyIndex = 0;

        // Request the device queue to submit work to
        vkGetDeviceQueue(_vkDevice,
                         queueFamilyIndex,
                         0,
                         &logicalDeviceQueue);

        // Create Command Pool and Buffers
        const VkCommandPoolCreateInfo commandPoolCreateInfo
        {
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags            = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = queueFamilyIndex
        };

        VkResult result = vkCreateCommandPool(_vkDevice,
                                              &commandPoolCreateInfo,
                                              NULL,
                                              &_vkCommandPool);

        // Was is successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Command Pool.");
    }

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
    void SolVulkanDevice::CreateVulkanDebugCallback()
    {
        // Register error logging function
        VkDebugReportCallbackEXT warningCallback{ VK_NULL_HANDLE }, errorCallback{ VK_NULL_HANDLE };
        PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT{ NULL };

        *(void **)&vkCreateDebugReportCallbackEXT = vkGetInstanceProcAddr(_vkInstance, 
                                                                          "vkCreateDebugReportCallbackEXT");

        DBG_ASSERT(vkCreateDebugReportCallbackEXT);

        VulkanDebugReportCallback_t pVulkanDebugReportCallback = &SolVulkanDevice::VulkanDebugReportCallback;

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
        callbackCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        callbackCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)&pVulkanDebugReportCallback;

        result = vkCreateDebugReportCallbackEXT(_vkInstance,
                                                &callbackCreateInfo,
                                                nullptr,
                                                &warningCallback);

        DBG_ASSERT_VULKAN_MSG(result, "vkCreateDebugReportCallbackEXT (WARNING) failed.");
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL SolVulkanDevice::VulkanDebugReportCallback(VkDebugReportFlagsEXT flags,
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

    SwapchainSupportDetails SolVulkanDevice::QuerySwapchainSupport(const VkPhysicalDevice &physicalDevice)
    {
        SwapchainSupportDetails supportDetails{};
        uint32_t surfaceFormatCount, presentModeCount;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice,
                                                  _vkSurface,
                                                  &supportDetails.surfaceCapabilities);

        // Query for surface format count
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                             _vkSurface,
                                             &surfaceFormatCount,
                                             NULL);

        if (surfaceFormatCount != 0)
        {
            supportDetails.imageFormats.resize(surfaceFormatCount);

            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                                 _vkSurface,
                                                 &surfaceFormatCount,
                                                 supportDetails.imageFormats.data());
        }

        // Query for present mode count
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
                                                  _vkSurface,
                                                  &presentModeCount,
                                                  NULL);

        if (presentModeCount != 0)
        {
            supportDetails.presentModes.resize(presentModeCount);

            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
                                                      _vkSurface,
                                                      &presentModeCount,
                                                      supportDetails.presentModes.data());
        }

        return supportDetails;
    }

    QueueFamilyIndices SolVulkanDevice::QueryQueueFamilies(const VkPhysicalDevice &physicalDevice)
    {
        QueueFamilyIndices queueFamilyIndices{};
        uint32_t queueFamilyCount;

        // Query for Queue Family count
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
                                                 &queueFamilyCount,
                                                 NULL);

        if (queueFamilyCount == 0)
        {
            return queueFamilyIndices;
        }

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

        // Fill out QueueFamilies vector
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
                                                 &queueFamilyCount,
                                                 queueFamilies.data());

        for (uint32_t i(0); i < queueFamilyCount; ++i)
        {
            const VkQueueFamilyProperties& queueFamilyProperties = queueFamilies.at(i);

            if (queueFamilyProperties.queueCount > 0 &&
                queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queueFamilyIndices.graphicsFamily = i;
                queueFamilyIndices.graphicsFamilyHasValue = true;
            }

            VkBool32 isPresentSupported = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 
                                                 i,
                                                 _vkSurface, 
                                                 &isPresentSupported);

            if (queueFamilyProperties.queueCount > 0 && 
                isPresentSupported)
            {
                queueFamilyIndices.presentFamily = i;
                queueFamilyIndices.presentFamilyHasValue = true;
            }

            if (queueFamilyIndices.IsComplete())
            {
                break;
            }
        }

        return queueFamilyIndices;
    }
}