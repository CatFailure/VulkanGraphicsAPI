#include <vulkan/vulkan.hpp>

int main()
{
    const char* appName = "Hello Vulkan!";

    // Fill out application description
    VkApplicationInfo applicationInfo
    {
        .sType			  = VK_STRUCTURE_TYPE_APPLICATION_INFO,		// Mandatory
        .pNext			  = NULL,									// Mandatory
        .pApplicationName = appName,								// Name of the application
        .pEngineName	  = NULL,									// Name of the engine
        .engineVersion	  = 1,										// Version of the engine
        .apiVersion		  = VK_API_VERSION_1_0						// Version of Vulkan used by application
    };

    // Fill out instance description
    VkInstanceCreateInfo instanceCreateInfo
    {
        .sType					 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	// Mandatory
        .pNext					 = NULL,									// Mandatory set
        .flags					 = 0,										// Mandatory set
        .pApplicationInfo		 = &applicationInfo,						// Pass application info instance
        .enabledLayerCount		 = 0,										// Don't enable any layers
        .ppEnabledLayerNames	 = NULL,
        .enabledExtensionCount   = 0,										// Don't enable any extensions
        .ppEnabledExtensionNames = NULL,
    };

    // Create desired instance
    VkInstance instance = VK_NULL_HANDLE;
    VkResult result = vkCreateInstance(&instanceCreateInfo, 
                                       NULL, 
                                       &instance);

    assert(result == VK_SUCCESS && 
           "Failed to create Vulkan Instance.");

    vkDestroyInstance(instance, NULL);

    return 0;
}