#pragma once

namespace pbls
{
	void SeedRandom(unsigned int seed);

	float Random(); //0 - 1
	float RandomRange(float min, float max); //Random number with defined values

	int RandomInt();
	int RandomInt(int max);
	int RandomRangeInt(int min, int max);
}