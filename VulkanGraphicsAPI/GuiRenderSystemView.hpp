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
		static constexpr size_t	POLYGON_MODE_COUNT{ 3U };
		static constexpr size_t	CULL_MODE_COUNT	  { 4U };

		void RenderPolygonModeCombo();
		void RenderCullModeCombo();

		void OnPolygonModeChanged(const int mode);
		void OnCullModeChanged(const int mode);

		RenderSettings& _rRenderSettings;

		const char* _polygonModes[POLYGON_MODE_COUNT]{ "Fill", "Wireframe", "Point" };
		const char* _cullModes[CULL_MODE_COUNT]		 { "None", "Front", "Back", "Front and Back" };
	};
}
