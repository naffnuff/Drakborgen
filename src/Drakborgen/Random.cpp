#include "Random.h"

Random::Random()
	: generator(randomDevice())
{
}

bool Random::nextBool()
{
	unsigned int nextInt = generator();
	return nextInt % 2 == 0;
}
