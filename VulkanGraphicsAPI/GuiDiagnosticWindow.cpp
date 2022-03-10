#include "GuiDiagnosticWindow.hpp"

namespace SolEngine::GUI
{
    GuiDiagnosticWindow::GuiDiagnosticWindow(const char *windowTitle, 
                                             const bool isActive, 
                                             const ImGuiWindowFlags windowFlags, 
                                             DiagnosticData &rDiagnosticData)
        : IGuiWindow(windowTitle, 
                     isActive, 
                     windowFlags),
        _rDiagnosticData(_rRealtimeDiagnosticData),
        _rRealtimeDiagnosticData(rDiagnosticData)
    {
        _updateDataFrequency = 5.f;

        _onUpdateDataEvent.AddListener([this]() { OnUpdateData_Method(); });
    }

    void GuiDiagnosticWindow::RenderWindowContents()
    {
        const float framesPerSecond = 1.f / _rDiagnosticData.deltaTimeSeconds;
        const float deltaTimeMS     = _rDiagnosticData.deltaTimeSeconds * SECONDS_TO_MILLISECONDS;

        ImGui::Begin(_windowTitle, &_isActive, _windowFlags);

        ImGui::Text("Framerate: %.3f (%.3f ms)", 
                    framesPerSecond, 
                    deltaTimeMS);

        ImGui::PlotLines("Frame Delta (ms)",
                         _deltaTimeBacklog, 
                         MAX_BACKLOGGED_DELTA_TIMES,
                         0, 
                         NULL, 
                         MIN_DELTA_TIME_SCALE, 
                         MAX_DELTA_TIME_SCALE);

        ImGui::Text("Vert Count: %zu", _rDiagnosticData.vertexCount);
        ImGui::Text("Tri Count: %zu", _rDiagnosticData.triCount);
        ImGui::Text("In-use Memory (Bytes): %zu/%zu (%.3f%%)", 
                    _rDiagnosticData.memoryUsedBytes, 
                    _rDiagnosticData.memoryAllocatedBytes,
                    _rDiagnosticData.MemoryUsedPercentage());

        ImGui::End();
    }

    void GuiDiagnosticWindow::OnUpdateData_Method()
    {
        _rDiagnosticData = _rRealtimeDiagnosticData;

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
            _rDiagnosticData.deltaTimeSeconds * SECONDS_TO_MILLISECONDS;
    }
}