#pragma once

namespace SolEngine::Enumeration
{
	enum class SimulationState
	{
		UNKNOWN = -1,
		PAUSED  = UNKNOWN + 1,
		PLAY    = PAUSED + 1
	};
}
