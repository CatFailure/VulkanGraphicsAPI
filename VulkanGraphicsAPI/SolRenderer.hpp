#pragma once
#include "SolWindow.hpp"
#include "SolSwapchain.hpp"
#include "SolModel.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Interface;

namespace SolEngine
{
	class SolRenderer : public IDisposable
	{
	public:
		SolRenderer() = delete;
		SolRenderer(const ApplicationData& appData, SolWindow& rSolWindow, SolDevice& rSolDevice);
        ~SolRenderer();

        bool IsFrameInProgress() const { return _isFrameStarted; }

        VkRenderPass    GetSwapchainRenderPass()  const { return _pSolSwapchain->GetRenderPass(); }
        VkCommandBuffer GetCurrentCommandBuffer() const;
        size_t          GetFrameIndex()           const;
        float           GetAspectRatio()          const { return _pSolSwapchain->GetExtentAspectRatio(); }
        

        /// <summary>
        /// Called to begin the Draw frame and create the frame's Command Buffer.
        /// </summary>
        /// <returns></returns>
        VkCommandBuffer BeginFrame();

        /// <summary>
        /// Called to end the Draw frame.
        /// Submits the Command Buffer and Recreates Swapchain on Window Resize.
        /// </summary>
        void EndFrame();

        /// <summary>
        /// Called after BeginFrame to begin the Swapchain's Render pass.
        /// Sets the Viewport and Scissor to the Command Buffer.
        /// </summary>
        /// <param name="commandBuffer"></param>
        void BeginSwapchainRenderPass(const VkCommandBuffer commandBuffer);

        /// <summary>
        /// Called before EndFrame.
        /// </summary>
        /// <param name="commandBuffer"></param>
        void EndSwapchainRenderPass(const VkCommandBuffer commandBuffer);

        // Inherited via IDisposable
        virtual void Dispose() override;

	private:
        void PrintDeviceMemoryCapabilities();

        void CreateCommandBuffers();
        void FreeCommandBuffers();

        void RecreateSwapchain();

        uint32_t _physDeviceCount{ 0 }, _physDeviceQueueFamilyCount{ 0 };
        uint32_t _currentImageIndex{ 0 }, _currentFrameIndex{ 0 };
        bool     _isFrameStarted{ false };

        ApplicationData _appData;

        SolWindow& _rSolWindow;
        SolDevice& _rSolDevice;
        std::unique_ptr<SolSwapchain> _pSolSwapchain;

        std::vector<VkCommandBuffer> _commandBuffers;
	};
}
