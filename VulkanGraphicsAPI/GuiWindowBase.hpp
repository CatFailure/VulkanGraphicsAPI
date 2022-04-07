#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "SolEvent.hpp"
#include "IMonoBehaviour.hpp"

using namespace SolEngine::Interface;
using namespace SolEngine::Events;

namespace SolEngine::Interface
{
	class GuiWindowBase : public IMonoBehaviour
	{
	public:
		GuiWindowBase(const char* windowTitle, 
					  const bool isActive = true, 
					  const ImGuiWindowFlags windowFlags = 0)
			: _windowTitle(windowTitle),
			  _isActive(isActive), 
			  _windowFlags(windowFlags)
		{};

		virtual ~GuiWindowBase() {}

		virtual void Update(const float deltaTime) override
		{
			if (!ShouldUpdateData())
			{
				TickUpdateDataDelayRemaining(deltaTime);

				return;
			}

			ResetUpdateDelayRemaining();

			_onUpdateDataEvent.Invoke();
		}

		virtual void Render() = 0;

	protected:
		void TickUpdateDataDelayRemaining(const float deltaTime) { _updateDataDelayRemaining -= deltaTime; }
		bool ShouldUpdateData() const							 { return !(_updateDataDelayRemaining > 0.f); }
		void ResetUpdateDelayRemaining()						 { _updateDataDelayRemaining = 1.f / _updateDataFrequency; }

		const char*		 _windowTitle{ "ImGui Window" };
		bool			 _isActive	 { true };
		ImGuiWindowFlags _windowFlags{ 0 };

		float _updateDataFrequency	   { 1.f };
		float _updateDataDelayRemaining{ 0.f };

		SolEvent<> _onUpdateDataEvent;
	};
}
