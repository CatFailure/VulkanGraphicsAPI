#pragma once
#include "imgui.h"
#include "GridSettings.hpp"
#include "GuiStrings.hpp"

using namespace SolEngine::GUI;
using namespace SolEngine::Settings;

namespace SolEngine::GUI::View
{
	class GuiGridView
	{
	public:
		GuiGridView() = delete;
		GuiGridView(GridSettings& rGridSettings);

		void RenderViewContents();

	private:
		GridSettings  _defaultGridSettings{};
		GridSettings& _rGridSettings;
	};
}
