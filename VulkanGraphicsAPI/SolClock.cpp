#include "pch.hpp"
#include "SolClock.hpp"

namespace SolEngine
{
	SolClock::SolClock()
	{
		Start();
	}

	void SolClock::Start()
	{
		Reset();

		_currentTime = std::chrono::high_resolution_clock::now();
	}

	float SolClock::Restart()
	{
		_resetTime = std::chrono::high_resolution_clock::now();
		_deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(_currentTime - _resetTime).count();
		_totalTime += _deltaTime;
		_currentTime = _resetTime;

		return _deltaTime;
	}

	void SolClock::Reset()
	{
		_resetTime = std::chrono::high_resolution_clock::time_point::min();
		_currentTime = _resetTime;

		_deltaTime = 0.f;
		_totalTime = 0.f;
	}
}