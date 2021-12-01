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

        // Public Accessors
        VkFramebuffer Framebuffer(const size_t index) const { return _vkSwapchainFramebuffers.at(index); }
        VkRenderPass RenderPass() const { return _vkRenderPass; }
        VkImageView ImageView(const size_t index) const { return _vkSwapchainImageViews.at(index); }
        size_t ImageCount() const { return _vkSwapchainImages.size(); }
        VkFormat ImageFormat() const { return _vkSwapchainImageFormat; }

        VkExtent2D Extent() const { return _vkSwapchainExtent; }
        Vector2<uint32_t> ExtentDimensions() const { return { _vkSwapchainExtent.width, _vkSwapchainExtent.height }; }
        float ExtentAspectRatio() const { return static_cast<float>(_vkSwapchainExtent.width) / static_cast<float>(_vkSwapchainExtent.height); }

        VkFormat FindDepthFormat();

        VkResult AcquireNextImage(uint32_t *pImageIndex);

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

        size_t _currentFrame{ 0 };
        uint64_t _timeout{ (std::numeric_limits<uint64_t>::max)() };
    };
}
