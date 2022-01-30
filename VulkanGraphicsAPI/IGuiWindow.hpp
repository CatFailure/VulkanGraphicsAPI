#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "SolEvent.hpp"

using namespace SolEngine::Events;

namespace SolEngine::Interface
{
	class IGuiWindow : public IMonoBehaviour
	{
	public:
		IGuiWindow(const char *windowTitle, const bool isActive = true, const ImGuiWindowFlags windowFlags = 0)
			: _windowTitle(windowTitle),
			  _isActive(isActive), 
			  _windowFlags(windowFlags)
		{};

		virtual ~IGuiWindow() {}

		virtual void Update(const float deltaTime) override
		{
			if (!ShouldUpdate())
			{
				TickUpdateDelayRemaining(deltaTime);

				return;
			}

			ResetUpdateDelayRemaining();

			_onUpdateEvent.Invoke();
		}

		virtual void RenderWindowContents() = 0;

	protected:
		void TickUpdateDelayRemaining(const float deltaTime) { _updateDelayRemaning -= deltaTime; }
		bool ShouldUpdate() const { return !(_updateDelayRemaning > 0.f); }
		void ResetUpdateDelayRemaining() { _updateDelayRemaning = _updateFrequency; }

		const char *_windowTitle{ "ImGui Window" };
		bool _isActive{ true };
		ImGuiWindowFlags _windowFlags{ 0 };

		float _updateFrequency{ 1.f };
		float _updateDelayRemaning{ 0.f };

		SolEvent<> _onUpdateEvent;
	};
}
