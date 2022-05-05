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
		static constexpr const char* RESET_POLYGON_MODE_BUTTON_ID{"Label##ResetPolygonMode"};
		static constexpr const char* RESET_CULL_MODE_BUTTON_ID	 {"Label##ResetCullMode"};

		static constexpr float RESET_CULL_MODE_BUTTON_PADDING{ 312.f };	// Makes Reset Buttons line up - Since the GUI is auto-scaled this will never change

		static constexpr size_t	POLYGON_MODE_COUNT{ 3U };
		static constexpr size_t	CULL_MODE_COUNT	  { 4U };

		void RenderPolygonModeCombo();
		void RenderCullModeCombo();

		void RenderResetPolygonModeButton();
		void RenderResetCullModeButton();

		void OnPolygonModeChanged(const int mode);
		void OnCullModeChanged(const int mode);

		void OnPolygonModeReset();
		void OnCullModeReset();

		const RenderSettings _defaultRenderSettings{};
		RenderSettings&		 _rRenderSettings;

		const char* _polygonModes[POLYGON_MODE_COUNT]{ "Fill", "Wireframe", "Point" };
		const char* _cullModes[CULL_MODE_COUNT]		 { "None", "Front", "Back", "Front and Back" };
	};
}
