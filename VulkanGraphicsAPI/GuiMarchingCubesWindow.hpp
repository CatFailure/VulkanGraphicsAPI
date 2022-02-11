#pragma once
#include "IGuiWindow.hpp"
#include "MarchingCubesData.hpp"

using namespace SolEngine::GUI::Data;
using namespace SolEngine::Interface;

namespace SolEngine::GUI
{
    class GuiMarchingCubesWindow : public IGuiWindow
    {
    public:
        GuiMarchingCubesWindow() = default;
        GuiMarchingCubesWindow(const char *windowTitle, const bool isActive, const ImGuiWindowFlags windowFlags, MarchingCubesData &rMarchingCubesData);

        // Inherited via IGuiWindow
        virtual void RenderWindowContents() override;

    private:
        MarchingCubesData &_rRealtimeMarchingCubesData;
    };
}
