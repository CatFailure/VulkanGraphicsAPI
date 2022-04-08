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

		RenderPolygonModeCombo();
		RenderCullModeCombo();
	}

	void GuiRenderSystemView::RenderPolygonModeCombo()
	{
		int selectedMode = _rRenderSettings.polygonMode;

		if (ImGui::Combo("Polygon Mode",
						 &selectedMode, 
						 _polygonModes, 
						 POLYGON_MODE_COUNT))
		{
			OnPolygonModeChanged(selectedMode);
		}

		// Tooltip - Neighbourhood Type
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_RENDER_POLYGON_MODE);
		}
		ImGui::EndTooltip();
	}

	void GuiRenderSystemView::RenderCullModeCombo()
	{
		int selectedMode = _rRenderSettings.cullMode;

		if (ImGui::Combo("Cull Mode",
						 &selectedMode, 
						 _cullModes, 
						 CULL_MODE_COUNT))
		{
			OnCullModeChanged(selectedMode);
		}

		// Tooltip - Neighbourhood Type
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_RENDER_CULL_MODE);
		}
		ImGui::EndTooltip();
	}

	void GuiRenderSystemView::OnPolygonModeChanged(const int mode)
	{
		_rRenderSettings.polygonMode = (VkPolygonMode)mode;
		_rRenderSettings.isRendererOutOfDate = true;
	}

	void GuiRenderSystemView::OnCullModeChanged(const int mode)
	{
		_rRenderSettings.cullMode = (VkCullModeFlagBits)mode;
		_rRenderSettings.isRendererOutOfDate = true;
	}
}
