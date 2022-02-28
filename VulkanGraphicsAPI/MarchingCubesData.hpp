#pragma once
#include "SolEvent.hpp"

using namespace SolEngine::Events;

namespace SolEngine::GUI::Data
{
	struct MarchingCubesData
	{
		bool isInterpolated{ true };
		float isoLevel	   { -2.5f };

		SolEvent<> onIsInterpolatedChangedEvent;
		SolEvent<> onIsoLevelChangedEvent;
	};
}
