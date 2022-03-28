#pragma once
#include "imgui.h"
#include "GridSettings.hpp"

using namespace SolEngine::Settings;

namespace SolEngine::GUI::View
{
	class GuiGridView
	{
	public:
		GuiGridView() = delete;
		GuiGridView(GridSettings& rGridSettings);

	private:
		GridSettings& _rGridSettings;
	};
}
