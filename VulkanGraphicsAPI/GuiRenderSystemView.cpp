#include "GuiRenderSystemView.hpp"

namespace SolEngine::GUI::View
{
	GuiRenderSystemView::GuiRenderSystemView(RenderSettings& rRenderSettings)
		: _rRenderSettings(rRenderSettings)
	{}

	void GuiRenderSystemView::RenderViewContents()
	{
		if (!ImGui::CollapsingHeader(HEADER_RENDER_SYSTEM))
		{
			return;
		}
	}
}
