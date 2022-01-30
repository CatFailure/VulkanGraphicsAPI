#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace SolEngine::Interface
{
	class IGuiWindow
	{
	public:
		IGuiWindow(const char *windowTitle, const bool isActive = true, const ImGuiWindowFlags windowFlags = 0)
			: _windowTitle(windowTitle),
			  _isActive(isActive), 
			  _windowFlags(windowFlags)
		{};

		virtual ~IGuiWindow() {}

		virtual void UpdateWindowContents() = 0;

	protected:
		const char *_windowTitle{ "ImGui Window" };
		bool _isActive{ true };
		ImGuiWindowFlags _windowFlags{ 0 };
	};
}
