#pragma once
#include "SolEvent.hpp"

using namespace SolEngine::Events;

namespace SolEngine::GUI::Data
{
	struct MarchingCubesData
	{
		bool isInterpolated{ true };
		float isoLevel	   { -2.5f };
		float step		   { 1.f };		// Adjusts the resolution of the nodes

		SolEvent<> onIsInterpolatedChangedEvent;
		SolEvent<> onIsoLevelChangedEvent;
		SolEvent<> onStepChangedEvent;
	};
}
