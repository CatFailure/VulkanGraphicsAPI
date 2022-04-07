#include "GuiWindowManager.hpp"

namespace SolEngine::GUI
{
	GuiWindowManager::GuiWindowManager(SolDevice& rSolDevice, 
                                       const SolWindow& solWindow,                                
                                       const SolRenderer& solRenderer, 
                                       const VkDescriptorPool& descriptorPool)
	{
        InitialiseImGui(rSolDevice, 
                        solWindow, 
                        solRenderer, 
                        descriptorPool);

        InitialiseImGuiFont(rSolDevice);
	}

	GuiWindowManager::~GuiWindowManager()
	{
		Dispose();
	}

    void GuiWindowManager::NewFrame()
    {
        // Start Dear ImGui frame...
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GuiWindowManager::Render(const VkCommandBuffer commandBuffer)
    {
        for (std::unique_ptr<IGuiWindow>& rpGuiWindow : _guiWindows)
        {
            rpGuiWindow->Render();
        }

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

	void GuiWindowManager::Update(const float deltaTime)
	{
        for (std::unique_ptr<IGuiWindow>& rpGuiWindow : _guiWindows)
        {
            rpGuiWindow->Update(deltaTime);
        }
	}

	void GuiWindowManager::InitialiseImGui(SolDevice& rSolDevice,
                                           const SolWindow& solWindow, 
                                           const SolRenderer& solRenderer, 
                                           const VkDescriptorPool& descriptorPool)
	{
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGuiIO& rIo = ImGui::GetIO(); (void)rIo;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(solWindow.GetWindow(), true);

        const ImGui_ImplVulkan_InitInfo initInfo
        {
            .Instance        = rSolDevice.GetInstance(),
            .PhysicalDevice  = rSolDevice.GetPhysicalDevice(),
            .Device          = rSolDevice.GetDevice(),
            .QueueFamily     = rSolDevice.QueryPhysicalDeviceQueueFamilies().graphicsFamily,
            .Queue           = rSolDevice.GetGraphicsQueue(),
            .PipelineCache   = VK_NULL_HANDLE,
            .DescriptorPool  = descriptorPool,
            .Subpass         = 0,
            .MinImageCount   = SolSwapchain::MAX_FRAMES_IN_FLIGHT,
            .ImageCount      = SolSwapchain::MAX_FRAMES_IN_FLIGHT,
            .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
            .Allocator       = VK_NULL_HANDLE,
            .CheckVkResultFn = DebugHelpers::CheckVkResult
        };

        ImGui_ImplVulkan_Init(&initInfo, solRenderer.GetSwapchainRenderPass());
	}

    void GuiWindowManager::InitialiseImGuiFont(SolDevice& rSolDevice)
    {
        const VkCommandBuffer commandBuffer = rSolDevice.BeginOneTimeCommandBuffer();

        // Use the default font
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

        rSolDevice.EndOneTimeCommandBuffer(commandBuffer);

        // Destroy ImGui fonts after uploading...
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

	void GuiWindowManager::Dispose()
	{
        // ImGui Cleanup...
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}
}
