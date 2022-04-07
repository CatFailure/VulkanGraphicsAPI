#pragma once
#include "SolWindow.hpp"
#include "SolDevice.hpp"
#include "SolRenderer.hpp"
#include "GuiDiagnosticWindow.hpp"
#include "GuiSettingsWindow.hpp"
#include "CameraSettings.hpp"

using namespace SolEngine::Settings;

namespace SolEngine::GUI
{
    class GuiWindowManager : public IMonoBehaviour, private IDisposable
    {
    public:
        GuiWindowManager() = default;
        GuiWindowManager(SolDevice& rSolDevice, CameraSettings& rCameraSettings, const SolWindow& solWindow, 
                         const SolRenderer& solRenderer, const VkDescriptorPool& descriptorPool);
        ~GuiWindowManager();

        void NewFrame();
        void Render(const VkCommandBuffer commandBuffer);

        // Inherited via IMonoBehaviour
        virtual void Update(const float deltaTime) override;

        template<typename _TyWindow, typename ...TyArgs>
        GuiWindowManager& CreateGuiWindow(const char* windowTitle, 
                                          const bool isActive = true, 
                                          const ImGuiWindowFlags flags = 0, 
                                          TyArgs&&... args)
        {
            _guiWindows.push_back(std::make_unique<_TyWindow>(windowTitle, 
                                                              isActive, 
                                                              flags,
                                                              args...));

            return *this;
        }

    private:
        void InitialiseImGui(SolDevice& rSolDevice, const SolWindow& solWindow, 
                             const SolRenderer& solRenderer, const VkDescriptorPool& descriptorPool);
        void InitialiseImGuiFont(SolDevice& rSolDevice);

        // Inherited via IDisposable
        virtual void Dispose() override;

        CameraSettings& _rCameraSettings;
        std::vector<std::unique_ptr<GuiWindowBase>> _guiWindows;
    };
}


