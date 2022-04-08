#pragma once
#include "imgui.h"
#include "RenderSettings.hpp"
#include "GuiStrings.hpp"

using namespace SolEngine::Settings;

namespace SolEngine::GUI::View
{
	class GuiRenderSystemView
	{
	public:
		GuiRenderSystemView(RenderSettings& rRenderSettings);

		void RenderViewContents();

	private:
		void RenderPolygonModeCombo();

		RenderSettings& _rRenderSettings;
	};
}
