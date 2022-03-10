#pragma once
#include "IGuiWindow.hpp"
#include "GameOfLifeSettings.hpp"

using namespace SolEngine::Settings;
using namespace SolEngine::Interface;

namespace SolEngine::GUI
{
	class GuiGameOfLifeWindow : public IGuiWindow
	{
	public:
		GuiGameOfLifeWindow() = default;
		GuiGameOfLifeWindow(const char* windowTitle, const bool isActive, const ImGuiWindowFlags windowFlags, GameOfLifeSettings& rGameOfLifeSettings);

	private:
		GameOfLifeSettings& _rGameOfLifeSettings;
	};
}
