#include "GuiGridView.hpp"

namespace SolEngine::GUI::View
{
	GuiGridView::GuiGridView(GridSettings& rGridSettings)
		: _rGridSettings(rGridSettings)
	{}

	void GuiGridView::RenderViewContents()
	{
		if (!ImGui::CollapsingHeader(HEADER_GRID))	// This header should be collapsed by default
		{
			return;
		}
	}
}