#pragma once
#include "IGuiWindow.hpp"
#include "GameOfLifeSettings.hpp"
#include "Constants.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Settings;
using namespace SolEngine::Interface;

namespace SolEngine::GUI
{
	class GuiGameOfLifeWindow : public IGuiWindow
	{
	public:
		GuiGameOfLifeWindow() = delete;
		GuiGameOfLifeWindow(const char* windowTitle, const bool isActive, const ImGuiWindowFlags windowFlags, GameOfLifeSettings& rGameOfLifeSettings);

		// Inherited via IGuiWindow
		virtual void RenderWindowContents() override;

	private:
		void OnMinLiveNeighboursChanged(const int value);
		void OnMaxLiveNeighboursChanged(const int value);

		GameOfLifeSettings& _rGameOfLifeSettings;
	};
}
