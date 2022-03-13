#pragma once
#include <random>

namespace Utility
{
	class RandomNumberGenerator
	{
	public:
		static bool GetRandomBool();

		static void SetSeed(const unsigned int seed);
	private:
		static unsigned int _seed;
	};
}
