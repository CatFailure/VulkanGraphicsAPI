#pragma once
#include "SolDevice.hpp"

using namespace SolEngine::Interface;

namespace SolEngine
{
    class SolSwapchain : public IDisposable
    {
    public:
        SolSwapchain(SolDevice &rSolDevice, const VkExtent2D &windowExtent);
        SolSwapchain(SolDevice &rSolDevice, const VkExtent2D &windowExtent, std::shared_ptr<SolSwapchain> pOldSwapchain);
        ~SolSwapchain();

        static constexpr uint32_t MAX_FRAMES_IN_FLIGHT{ 2 };

        // Public Accessors
        VkFramebuffer GetFramebuffer(const size_t index) const { return _swapchainFrameBuffers.at(index); }
        VkRenderPass  GetRenderPass()                    const { return _renderPass; }
        VkImageView   GetImageView(const size_t index)   const { return _swapchainImageViews.at(index); }
        size_t        GetImageCount()                    const { return _swapchainImages.size(); }
        VkFormat      GetImageFormat()                   const { return _swapchainImageFormat; }
        VkExtent2D    GetExtent()                        const { return _swapchainExtent; }
        Vector2u      GetExtentDimensions()              const { return { _swapchainExtent.width, _swapchainExtent.height }; }
        float         GetExtentAspectRatio()             const { return static_cast<float>(_swapchainExtent.width) / static_cast<float>(_swapchainExtent.height); }

        VkFormat FindDepthFormat();

        VkResult AcquireNextImage(uint32_t *pImageIndex);
        VkResult SubmitCommandBuffers(const VkCommandBuffer *pCommandBuffers, const uint32_t *pImageIndex);

        bool CompareSwapchanFormats(const SolSwapchain& swapchain) const;

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
        VkPresentModeKHR   ChoosePresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D         ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        SolDevice &_rSolDevice;
        VkExtent2D _windowExtent;

        // Swapchain
        VkSwapchainKHR                _swapchain{ NULL };
        std::shared_ptr<SolSwapchain> _pOldSwapchain;

        std::vector<VkImage>       _swapchainImages;
        std::vector<VkImageView>   _swapchainImageViews;
        std::vector<VkFramebuffer> _swapchainFrameBuffers;
        VkFormat                   _swapchainImageFormat;
        VkFormat                   _swapchainDepthFormat;
        VkExtent2D                 _swapchainExtent;

        VkRenderPass _renderPass{ NULL };

        // Depth
        std::vector<VkImage>        _depthImages;
        std::vector<VkImageView>    _depthImageViews;
        std::vector<VkDeviceMemory> _depthImageMemories;

        std::vector<VkFormat> _depthFormatCandidates
        {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };

        VkImageTiling        _depthImageTiling       { VK_IMAGE_TILING_OPTIMAL };
        VkFormatFeatureFlags _depthFormatFeatureFlags{ VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT };

        // Sync
        std::vector<VkSemaphore> _imageAvailableSemaphores;
        std::vector<VkSemaphore> _renderFinishedSemaphores;
        std::vector<VkFence>     _inFlightFences;
        std::vector<VkFence>     _inFlightImages;

        size_t   _currentFrame{ 0 };
        uint64_t _timeoutDuration{ (std::numeric_limits<uint64_t>::max)() };
    };
}
