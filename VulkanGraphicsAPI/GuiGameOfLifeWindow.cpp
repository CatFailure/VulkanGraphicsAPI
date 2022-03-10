#include "GuiGameOfLifeWindow.hpp"

namespace SolEngine::GUI
{
	GuiGameOfLifeWindow::GuiGameOfLifeWindow(const char* windowTitle, 
											 const bool isActive, 
											 const ImGuiWindowFlags windowFlags, 
											 GameOfLifeSettings& rGameOfLifeSettings)
		: IGuiWindow(windowTitle,
					 isActive,
					 windowFlags),
		_rGameOfLifeSettings(rGameOfLifeSettings)
	{}

	void GuiGameOfLifeWindow::RenderWindowContents()
	{
		int minLiveNeighbourCount = (int)_rGameOfLifeSettings.minLiveNeighbourCount;
		int maxLiveNeighbourCount = (int)_rGameOfLifeSettings.maxLiveNeighbourCount;

		ImGui::Begin(_windowTitle, 
					 &_isActive, 
					 _windowFlags);

		if (ImGui::SliderInt("Min. Live Neighbours",
							 &minLiveNeighbourCount,
							 0,
							 maxLiveNeighbourCount - 1))
		{
			OnMinLiveNeighboursChanged(minLiveNeighbourCount);
		}

		if (ImGui::SliderInt("Max. Live Neighbours",
							 &maxLiveNeighbourCount,
							 minLiveNeighbourCount + 1,
							 CELL_NEIGHBOURS_COUNT))
		{
			OnMaxLiveNeighboursChanged(maxLiveNeighbourCount);
		}

		if (ImGui::Button("Reset"))
		{
			_rGameOfLifeSettings.Reset();
		}

		ImGui::End();
	}

	void GuiGameOfLifeWindow::OnMinLiveNeighboursChanged(const int value)
	{
		_rGameOfLifeSettings.minLiveNeighbourCount = (NeighbourCount_t)value;
	}

	void GuiGameOfLifeWindow::OnMaxLiveNeighboursChanged(const int value)
	{
		_rGameOfLifeSettings.maxLiveNeighbourCount = (NeighbourCount_t)value;
	}
}