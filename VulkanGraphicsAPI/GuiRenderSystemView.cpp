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
		ImGui::SameLine();
		RenderResetPolygonModeButton();
		RenderCullModeCombo();
		ImGui::SameLine(RESET_CULL_MODE_BUTTON_PADDING);
		RenderResetCullModeButton();
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

		// Tooltip - Polygon Mode
		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_RENDER_POLYGON_MODE,
						_polygonModes[(size_t)_defaultRenderSettings.polygonMode]);
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

		// Tooltip - Cull Mode
		if (!ImGui::IsItemHovered())
		{
			return;
		}

		ImGui::BeginTooltip();
		{
			ImGui::Text(TOOLTIP_RENDER_CULL_MODE,
						_cullModes[(size_t)_defaultRenderSettings.cullMode]);
		}
		ImGui::EndTooltip();
	}

	void GuiRenderSystemView::RenderResetPolygonModeButton()
	{
		ImGui::PushID(RESET_POLYGON_MODE_BUTTON_ID);	// Since there are multiple buttons with a "Reset" label, we have to define a unique ID here
		if (ImGui::Button(LABEL_RENDER_POLYGON_MODE_RESET))
		{
			OnPolygonModeReset();
		}
		ImGui::PopID();
	}

	void GuiRenderSystemView::RenderResetCullModeButton()
	{
		ImGui::PushID(RESET_CULL_MODE_BUTTON_ID);	// Since there are multiple buttons with a "Reset" label, we have to define a unique ID here
		if (ImGui::Button(LABEL_RENDER_CULL_MODE_RESET))
		{
			OnCullModeReset();
		}
		ImGui::PopID();
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

	void GuiRenderSystemView::OnPolygonModeReset()
	{
		OnPolygonModeChanged(_defaultRenderSettings.polygonMode);
	}

	void GuiRenderSystemView::OnCullModeReset()
	{
		OnCullModeChanged(_defaultRenderSettings.cullMode);
	}
}
