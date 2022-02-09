#pragma once
#include <chrono>

namespace SolEngine
{
	class SolClock
	{
	public:
		SolClock();

		/// <summary>
		/// Starts the clock with Current Time set.
		/// </summary>
		void  Start();

		/// <summary>
		/// Re-sets the Current Time to now, calculates and returns time since last restart as deltaTime.
		/// </summary>
		/// <returns></returns>
		float Restart();

		/// <summary>
		/// Resets clock back to 0.
		/// </summary>
		void  Reset();

		float GetDeltaTime() const { return _deltaTime; }
		float GetTotalTime() const { return _totalTime; }

	private:
		std::chrono::steady_clock::time_point _currentTime;	// Current time point.
		std::chrono::steady_clock::time_point _resetTime;	// Time point at last restart.

		float _deltaTime{ 0.f };
		float _totalTime{ 0.f };
	};
}
