#pragma once
#include "imgui.h"
#include "Constants.hpp"
#include "GuiStrings.hpp"
#include "GameOfLifeSettings.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Settings;

namespace SolEngine::GUI::View
{
	class GuiGameOfLifeView
	{
	public:
		GuiGameOfLifeView(GameOfLifeSettings& rGameOfLifeSettings);

		void RenderViewContents();

	private:
		static constexpr const char* RESET_SETTINGS_BUTTON_ID{"Label##ResetGameOfLifeSettings"};

		void RenderGameOfLifeUnderpopulationSlider(int& rUnderpopulationCount, const int overpopulationCount);
		void RenderGameOfLifeOverpopulationSlider(int& rOverpopulationCount, const int underpopulationCount);
		void RenderGameOfLifeReproductionSlider(int& rReproductionCount);
		void RenderGameOfLifeResetButton();

		void OnUnderpopulationValueChanged(const int value);
		void OnOverpopulationValueChanged(const int value);
		void OnReproductionValueChanged(const int value);

		GameOfLifeSettings  _defaultGameOfLifeSettings{};
		GameOfLifeSettings& _rGameOfLifeSettings;
	};
}
