#pragma once
#include "SolEvent.hpp"

using namespace SolEngine::Events;

namespace SolEngine::GUI::Data
{
	struct MarchingCubesData
	{
		bool isInterpolated{ true };
		float isoSurface   { 0.f };
		float step		   { 1.f };		// Adjusts the resolution of the nodes

		SolEvent<> onIsInterpolatedChangedEvent;
	};
}
