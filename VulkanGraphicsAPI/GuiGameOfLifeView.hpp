#pragma once
#include "imgui.h"
#include "Constants.hpp"
#include "GuiStrings.hpp"
#include "GameOfLifeSettings.hpp"
#include "Helpers.hpp"

using namespace Utility;
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

		uint32_t GetMaxCellNeighbourCount() const;

		void RenderGameOfLifeNeighbourhoodCombo(NeighbourhoodType& rNeighbourhoodType);
		void RenderGameOfLifeUnderpopulationSlider(int& rUnderpopulationCount, const int overpopulationCount);
		void RenderGameOfLifeOverpopulationSlider(int& rOverpopulationCount, const int underpopulationCount);
		void RenderGameOfLifeReproductionSlider(int& rReproductionCount);
		void RenderGameOfLifeResetButton();

		void OnNeighbourhoodTypeValueChanged(const NeighbourhoodType value);
		void OnUnderpopulationValueChanged(const int value);
		void OnOverpopulationValueChanged(const int value);
		void OnReproductionValueChanged(const int value);

		GameOfLifeSettings  _defaultGameOfLifeSettings{};
		GameOfLifeSettings& _rGameOfLifeSettings;

		const char* _neighbourhoodTypes[(size_t)NeighbourhoodType::COUNT]{ "Moore", "Von Neumann" };
	};
}
