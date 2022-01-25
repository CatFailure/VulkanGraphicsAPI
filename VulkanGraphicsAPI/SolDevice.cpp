#include "pch.hpp"
#include "SolDevice.hpp"

namespace SolEngine
{
    SolDevice::SolDevice(SolWindow &rSolWindow,
                         const ApplicationData &appData)
        : _rSolWindow(rSolWindow),
          _appData(appData)
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

    SolDevice::~SolDevice()
    {
        Dispose();
    }

    void SolDevice::CreateImageWithInfo(const VkImageCreateInfo &imageCreateInfo, 
                                        const VkMemoryPropertyFlags properties, 
                                        VkImage &rImage, 
                                        VkDeviceMemory &rImageMemory)
    {
        VkResult result = vkCreateImage(_device, 
                                        &imageCreateInfo,
                                        NULL, 
                                        &rImage);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Image.");

        VkMemoryRequirements imageMemoryRequirements{};

        vkGetImageMemoryRequirements(_device, 
                                     rImage,
                                     &imageMemoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo
        {
            .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize  = imageMemoryRequirements.size,
            .memoryTypeIndex = FindMemoryType(imageMemoryRequirements.memoryTypeBits, 
                                              properties)
        };

        result = vkAllocateMemory(_device, 
                                  &memoryAllocateInfo,
                                  NULL,
                                  &rImageMemory);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to allocate Image Memory.");

        result = vkBindImageMemory(_device, 
                                   rImage,
                                   rImageMemory,
                                   0);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Bind Image Memory.");
    }

    uint32_t SolDevice::FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags properties) const
    {
        VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties{};

        vkGetPhysicalDeviceMemoryProperties(_physicalDevice,
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

    VkFormat SolDevice::FindSupportedFormat(const std::vector<VkFormat> &candidates,
                                            const VkImageTiling tiling, 
                                            const VkFormatFeatureFlags features) const
    {
        for (const VkFormat &format : candidates)
        {
            VkFormatProperties properties{};

            vkGetPhysicalDeviceFormatProperties(_physicalDevice, 
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

    void SolDevice::CreateBuffer(const VkDeviceSize bufferSize,
                                 const VkBufferUsageFlags usage, 
                                 const VkMemoryPropertyFlags properties, 
                                 VkBuffer &rBuffer, 
                                 VkDeviceMemory &rBufferMemory)
    {
        const VkBufferCreateInfo bufferCreateInfo
        {
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size        = bufferSize,
            .usage       = usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        VkResult result = vkCreateBuffer(_device, &bufferCreateInfo, NULL, &rBuffer);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Create Buffer.");

        VkMemoryRequirements memoryRequirements{};

        vkGetBufferMemoryRequirements(_device,
                                      rBuffer, 
                                      &memoryRequirements);

        const VkMemoryAllocateInfo memoryAllocateInfo
        {
            .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize  = memoryRequirements.size,
            .memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, 
                                              properties)
        };

        result = vkAllocateMemory(_device, 
                                  &memoryAllocateInfo, 
                                  NULL,
                                  &rBufferMemory);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Allocate Buffer Memory.");

        // Bind buffer to allocated memory
        result = vkBindBufferMemory(_device, 
                                    rBuffer,
                                    rBufferMemory,
                                    0);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Bind Buffer Memory!");
    }

    void SolDevice::CreateStagingBuffer(VkBuffer *pOutBuffer, 
                                        VkDeviceMemory *pOutBufferMemory, 
                                        const VkDeviceSize bufferSize,
                                        const void *pSrcData)
    {
        // Staging buffers act as a "middle man" in the Device 
        // when copying data from Host to Device Local Memory
        //   Host (CPU))             |              Device (GPU)
        //                   Copy to temp 
        // ===============  Staging Buffer -> =========================
        // | void *pData |oooooooooooooooooooo| Staging Buffer Memory |
        // ===============                    =========================
        //                                                | CopyBuffer()
        //                                                V
        //                                  ==============================
        //                                  | Vertex/Index Buffer Memory |
        //                                  ==============================

        CreateBuffer(bufferSize, 
                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     *pOutBuffer, 
                     *pOutBufferMemory);

        // Create a region of host memory mapped to device (staging buffer) memory
        // - and point pBufferData to beginning of mapped memory range
        void *pBufferData;

        DBG_ASSERT_VULKAN_MSG(vkMapMemory(_device, 
                                          *pOutBufferMemory,
                                          0,
                                          bufferSize, 
                                          0,
                                          &pBufferData),
                              "Failed to map memory to buffer!");

        // Copy vertices data into the host mapped memory region
        memcpy(pBufferData, 
               pSrcData, 
               static_cast<uint32_t>(bufferSize));

        vkUnmapMemory(_device, 
                      *pOutBufferMemory);
    }

    void SolDevice::CopyBuffer(const VkBuffer srcBuffer, 
                               const VkBuffer dstBuffer, 
                               const VkDeviceSize size)
    {
        const VkCommandBuffer oneTimeCommandBuffer = BeginOneTimeCommandBuffer();

        const VkBufferCopy copyRegion
        {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = size
        };

        vkCmdCopyBuffer(oneTimeCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        EndOneTimeCommandBuffer(oneTimeCommandBuffer);
    }

    void SolDevice::DisposeBuffer(const VkBuffer buffer, 
                                  const VkDeviceMemory memory,
                                  const VkAllocationCallbacks *pAllocator)
    {
        vkDestroyBuffer(_device, buffer, pAllocator);
        vkFreeMemory(_device, memory, pAllocator);
    }

    void SolDevice::Dispose()
    {
        vkDestroyCommandPool(_device, _commandPool, NULL);
        vkDestroyDevice(_device, NULL);
        vkDestroySurfaceKHR(_instance, _surface, NULL);
        vkDestroyInstance(_instance, NULL);
    }

    void SolDevice::CreateVulkanInstance()
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
        const VkApplicationInfo applicationInfo
        {
            .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,	    // Mandatory, describes type of structure
            .pNext            = NULL,								    // Mandatory, stores pointers to extension-specific structures
            .pApplicationName = _appData.appName,					    // Name of the application
            .pEngineName      = _appData.engineName,					// Name of the engine
            .engineVersion    = 1,									    // Version of the engine
            .apiVersion       = VK_API_VERSION_1_2					    // Version of Vulkan used by application
        };

        // Fill out instance description
        const VkInstanceCreateInfo instanceCreateInfo
        {
            .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,  // Mandatory
            .pNext                   = NULL,								    // Mandatory set
            .flags                   = 0,									    // Mandatory set
            .pApplicationInfo        = &applicationInfo,					    // Pass application info instance
            .enabledLayerCount       = GetEnabledLayerCount(),	                // Number of enabled layers
            .ppEnabledLayerNames     = _enabledLayerNames.data(),               // Specified layer names
            .enabledExtensionCount   = GetEnabledExtensionCount(),              // Number of enabled extensions
            .ppEnabledExtensionNames = _enabledExtensionNames.data(),           // Specified extension names
        };

        const VkResult result = vkCreateInstance(&instanceCreateInfo,
                                                 NULL,
                                                 &_instance);

        // Was creation successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Vulkan instance.");

        // Is instance handle valid?
        DBG_ASSERT(_instance != NULL);
    }

    void SolDevice::CreateVulkanPhysicalDevice()
    {
        uint32_t physicalDeviceCount = 0;

        // Query how many devices are present
        VkResult result = vkEnumeratePhysicalDevices(_instance,
                                                     &physicalDeviceCount,
                                                     NULL);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to query the number of physical devices present.\n");

        // Make sure at least 1 device is present.
        DBG_ASSERT_MSG(physicalDeviceCount != 0, "Could not detect any physical devices present with Vulkan support.\n");

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);

        result = vkEnumeratePhysicalDevices(_instance,
                                            &physicalDeviceCount,
                                            &physicalDevices.at(0));

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to enumerate physical devices present.\n");

        // Need at least 1 device.
        const bool isOneDevicePresent = physicalDevices.size() > 0;

        DBG_ASSERT(isOneDevicePresent);

        // Use the first available device.
        _physicalDevice = physicalDevices.at(0);

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

    void SolDevice::CreateVulkanDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

        const float queuePriority(1.0f);
        const QueueFamilyIndices queueFamilyIndices = QueryQueueFamilies(_physicalDevice);
        const std::set<uint32_t> uniqueQueueFamilies
        {
            queueFamilyIndices.graphicsFamily, 
            queueFamilyIndices.presentFamily 
        };

        // Initialise Device Queues
        for (const uint32_t queueFamily : uniqueQueueFamilies)
        {
            const VkDeviceQueueCreateInfo deviceQueueCreateInfo
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = queueFamily,
                .queueCount = 1,
                .pQueuePriorities = &queuePriority
            };

            deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
        }

        const VkPhysicalDeviceFeatures deviceFeatures
        {
            .shaderClipDistance = VK_TRUE,
        };

        const VkDeviceCreateInfo deviceCreateInfo
        {
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount    = static_cast<uint32_t>(deviceQueueCreateInfos.size()),
            .pQueueCreateInfos       = deviceQueueCreateInfos.data(),
            .enabledLayerCount       = GetEnabledLayerCount(),
            .ppEnabledLayerNames     = _enabledLayerNames.data(),
            .enabledExtensionCount   = GetDeviceExtensionCount(),
            .ppEnabledExtensionNames = _logicalDeviceExtensions.data(),
            .pEnabledFeatures        = &deviceFeatures
        };

        // Ideally, want to enumerate to find best device.
        // Just use the first for now.
        const VkResult result = vkCreateDevice(_physicalDevice,
                                               &deviceCreateInfo,
                                               NULL,
                                               &_device);

        // Was it successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Logical Device!");

        vkGetDeviceQueue(_device, queueFamilyIndices.graphicsFamily, 0, &_graphicsQueue);
        vkGetDeviceQueue(_device, queueFamilyIndices.presentFamily, 0, &_presentQueue);
    }

    void SolDevice::CreateVulkanCommandPool()
    {
        // Queue address
        VkQueue logicalDeviceQueue{ NULL };
        const uint32_t queueFamilyIndex = 0;

        // Request the device queue to submit work to
        vkGetDeviceQueue(_device,
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

        VkResult result = vkCreateCommandPool(_device,
                                              &commandPoolCreateInfo,
                                              NULL,
                                              &_commandPool);

        // Was is successful?
        DBG_ASSERT_VULKAN_MSG(result, "Failed to create Command Pool.");
    }

    VkCommandBuffer SolDevice::BeginOneTimeCommandBuffer()
    {
        const VkCommandBufferAllocateInfo allocateInfo
        {
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool        = _commandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };

        VkCommandBuffer commandBuffer;

        DBG_ASSERT_VULKAN_MSG(vkAllocateCommandBuffers(_device, &allocateInfo, &commandBuffer),
                              "Failed to allocate Command Buffers!");

        const VkCommandBufferBeginInfo beginInfo
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        };

        DBG_ASSERT_VULKAN_MSG(vkBeginCommandBuffer(commandBuffer, &beginInfo), 
                              "Failed to begin Command Buffer!");

        return commandBuffer;
    }

    void SolDevice::EndOneTimeCommandBuffer(const VkCommandBuffer commandBuffer)
    {
        VkResult result;

        result = vkEndCommandBuffer(commandBuffer);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to end Command Buffer!");

        const VkSubmitInfo submitInfo
        {
            .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers    = &commandBuffer
        };

        result = vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to Submit Graphics Queue!");

        result = vkQueueWaitIdle(_graphicsQueue);

        DBG_ASSERT_VULKAN_MSG(result, "Queue Wait Idle failed!");

        vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
    }

#ifdef ENABLE_VULKAN_DEBUG_CALLBACK
    void SolDevice::CreateVulkanDebugCallback()
    {
        // Register error logging function
        VkDebugReportCallbackEXT warningCallback{ VK_NULL_HANDLE }, errorCallback{ VK_NULL_HANDLE };
        PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT{ NULL };

        *(void **)&vkCreateDebugReportCallbackEXT = vkGetInstanceProcAddr(_instance, 
                                                                          "vkCreateDebugReportCallbackEXT");

        DBG_ASSERT(vkCreateDebugReportCallbackEXT);

        const VulkanDebugReportCallback_t pVulkanDebugReportCallback = &SolDevice::VulkanDebugReportCallback;

        // Capture errors
        VkDebugReportCallbackCreateInfoEXT callbackCreateInfo
        {
            .sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
            .flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT,
            .pfnCallback = (PFN_vkDebugReportCallbackEXT)&pVulkanDebugReportCallback
        };

        VkResult result = vkCreateDebugReportCallbackEXT(_instance,
                                                         &callbackCreateInfo,
                                                         nullptr,
                                                         &errorCallback);

        DBG_ASSERT_VULKAN_MSG(result, "vkCreateDebugReportCallbackEXT (ERROR) failed.");

        // Capture warnings
        callbackCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        callbackCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)&pVulkanDebugReportCallback;

        result = vkCreateDebugReportCallbackEXT(_instance,
                                                &callbackCreateInfo,
                                                nullptr,
                                                &warningCallback);

        DBG_ASSERT_VULKAN_MSG(result, "vkCreateDebugReportCallbackEXT (WARNING) failed.");
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL SolDevice::VulkanDebugReportCallback(VkDebugReportFlagsEXT flags,
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

    SwapchainSupportDetails SolDevice::QuerySwapchainSupport(const VkPhysicalDevice &physicalDevice) const
    {
        VkResult result;
        SwapchainSupportDetails supportDetails{};
        uint32_t surfaceFormatCount, presentModeCount;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice,
                                                  _surface,
                                                  &supportDetails.surfaceCapabilities);

        // Query for surface format count
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                                      _surface,
                                                      &surfaceFormatCount,
                                                      NULL);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to get Physical Device Surface Formats!");

        if (surfaceFormatCount != 0)
        {
            supportDetails.imageFormats.resize(surfaceFormatCount);

            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                                 _surface,
                                                 &surfaceFormatCount,
                                                 supportDetails.imageFormats.data());
        }

        // Query for present mode count
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
                                                           _surface,
                                                           &presentModeCount,
                                                           NULL);

        DBG_ASSERT_VULKAN_MSG(result, "Failed to get Physical Device Surface Present Modes!");

        if (presentModeCount != 0)
        {
            supportDetails.presentModes.resize(presentModeCount);

            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
                                                      _surface,
                                                      &presentModeCount,
                                                      supportDetails.presentModes.data());
        }

        return supportDetails;
    }

    QueueFamilyIndices SolDevice::QueryQueueFamilies(const VkPhysicalDevice &physicalDevice) const
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

            DBG_ASSERT_VULKAN_MSG(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 
                                                                       i,
                                                                       _surface, 
                                                                       &isPresentSupported), 
                                  "Failed to get Physical Device Surface Support!");

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