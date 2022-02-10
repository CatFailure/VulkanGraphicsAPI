#include "GuiDiagnosticWindow.hpp"

namespace SolEngine::GUI
{
    GuiDiagnosticWindow::GuiDiagnosticWindow(const char *windowTitle, 
                                             const bool isActive, 
                                             const ImGuiWindowFlags windowFlags, 
                                             DiagnosticData &rDiagnosticData)
        : _diagnosticData(_rRealTimeDiagnosticData),
          _rRealTimeDiagnosticData(rDiagnosticData),
          IGuiWindow(windowTitle, isActive, windowFlags)
    {
        _updateFrequency = 5.f;

        _onUpdateEvent.AddListener([this]() { OnUpdate_Method(); });
    }

    void GuiDiagnosticWindow::RenderWindowContents()
    {
        ImGui::Begin(_windowTitle, &_isActive, _windowFlags);

        ImGui::Text("Framerate: %.3f", 1.f / _diagnosticData.deltaTimeSeconds);
        ImGui::PlotLines("Frame Delta (ms)",
                         _deltaTimeBacklog, 
                         MAX_BACKLOGGED_DELTA_TIMES,
                         0, 
                         NULL, 
                         MIN_DELTA_TIME_SCALE, 
                         MAX_DELTA_TIME_SCALE);

        ImGui::Text("Vert Count: %zu", _diagnosticData.vertexCount);
        ImGui::Text("Tri Count: %zu", _diagnosticData.triCount);
        ImGui::Text("Allocated Memory (Bytes): %zu", _diagnosticData.memoryAllocatedBytes);

        ImGui::End();
    }

    void GuiDiagnosticWindow::OnUpdate_Method()
    {
        _diagnosticData = _rRealTimeDiagnosticData;

        PushBackDeltaTime();
    }

    void GuiDiagnosticWindow::PushBackDeltaTime()
    {
        // Move all the times back 1 index
        for (size_t i = 0; i < MAX_BACKLOGGED_DELTA_TIMES - 1; ++i)
        {
            _deltaTimeBacklog[i] = _deltaTimeBacklog[i + 1];
        }

        // Commit new deltatime at the end
        _deltaTimeBacklog[MAX_BACKLOGGED_DELTA_TIMES - 1] = 
            _diagnosticData.deltaTimeSeconds * TO_MILLISECONDS;
    }
}