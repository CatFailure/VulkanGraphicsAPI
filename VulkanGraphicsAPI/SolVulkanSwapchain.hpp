#pragma once
#include "SolVulkanDevice.hpp"

namespace SolEngine
{
    class SolVulkanSwapchain : public IDisposable
    {
    public:
        SolVulkanSwapchain(SolVulkanDevice &rDevice, const VkExtent2D &windowExtent);
        SolVulkanSwapchain(SolVulkanDevice &rDevice, const VkExtent2D &windowExtent, std::shared_ptr<SolVulkanSwapchain> pOldSwapchain);
        ~SolVulkanSwapchain();

        static constexpr uint32_t MAX_FRAMES_IN_FLIGHT{ 2 };

        size_t SwapchainImageCount() const { return _vkSwapchainImages.size(); }

        // Inherited via IDisposable
        virtual void Dispose() override;
    private:
        void CreateSwapchain(uint32_t *pOutImageCount, VkSurfaceFormatKHR *pOutSurfaceImageFormat, VkExtent2D *pOutSwapchainExtent);
        void CreateSwapchainImages(uint32_t &rImageCount, const VkSurfaceFormatKHR &surfaceImageFormat, const VkExtent2D &swapchainExtent);
        void CreateSwapchainImageViews();
        void CreateDepthResources();
        void CreateRenderPass();
        void CreateFramebuffers();
        void CreateSyncObjects();

        VkSurfaceFormatKHR ChooseImageFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities);
        VkFormat FindDepthFormat();

        SolVulkanDevice &_rSolDevice;
        VkExtent2D _windowExtent;

        // Swapchain
        VkSwapchainKHR _vkSwapchain{ NULL };
        std::shared_ptr<SolVulkanSwapchain> _pOldSwapchain;

        std::vector<VkImage> _vkSwapchainImages;
        std::vector<VkImageView> _vkSwapchainImageViews;
        std::vector<VkFramebuffer> _vkSwapchainFramebuffers;
        VkFormat _vkSwapchainImageFormat;
        VkExtent2D _vkSwapchainExtent;

        VkRenderPass _vkRenderPass{ NULL };

        // Depth
        std::vector<VkImage> _vkDepthImages;
        std::vector<VkImageView> _vkDepthImageViews;
        std::vector<VkDeviceMemory> _vkDepthImageMemories;

        std::vector<VkFormat> _vkDepthFormatCandidates
        {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };

        VkImageTiling _vkDepthImageTiling{ VK_IMAGE_TILING_OPTIMAL };
        VkFormatFeatureFlags _vkDepthFormatFeatureFlags{ VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT };

        // Sync
        std::vector<VkSemaphore> _vkImageAvailableSemaphores;
        std::vector<VkSemaphore> _vkRenderFinishedSemaphores;
        std::vector<VkFence> _vkInFlightFences;
        std::vector<VkFence> _vkImagesInFlight;
    };
}
