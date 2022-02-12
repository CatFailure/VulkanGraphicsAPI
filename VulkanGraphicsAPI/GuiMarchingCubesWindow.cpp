#include "GuiMarchingCubesWindow.hpp"

namespace SolEngine::GUI
{
    GuiMarchingCubesWindow::GuiMarchingCubesWindow(const char *windowTitle, 
                                                   const bool isActive,
                                                   const ImGuiWindowFlags windowFlags,
                                                   MarchingCubesData &rMarchingCubesData)
        : _rRealtimeMarchingCubesData(rMarchingCubesData),
          IGuiWindow(windowTitle, 
                     isActive, 
                     windowFlags)
    {
    }

    void GuiMarchingCubesWindow::RenderWindowContents()
    {
        ImGui::Begin(_windowTitle, &_isActive, _windowFlags);

        if (ImGui::Checkbox("Use Interpolation?",
                            &_rRealtimeMarchingCubesData.isInterpolated))
        {
            _rRealtimeMarchingCubesData.onIsInterpolatedChangedEvent.Invoke();
        } 

        if (ImGui::SliderFloat("Iso Surface", 
                               &_rRealtimeMarchingCubesData.isoLevel, 
                               -15.f, 
                               10.f))
        {
            _rRealtimeMarchingCubesData.onIsoLevelChangedEvent.Invoke();
        }

        ImGui::End();
    }
}
