#include "RandomNumberGenerator.hpp"

namespace Utility
{
	unsigned int RandomNumberGenerator::_seed{ 0U };

	bool RandomNumberGenerator::GetRandomBool()
	{
		const bool value = rand() % 2;

		return value;
	}

	void RandomNumberGenerator::SetSeed(const unsigned int seed)
	{ 
		_seed = seed;
		srand(seed);
	}
}