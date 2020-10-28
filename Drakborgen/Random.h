#pragma once

#include <random>

class Random
{
public:
	Random();

	template<typename Container>
	void shuffle(Container& container);

private:
	std::random_device randomDevice;
	std::mt19937 generator;
};

template<typename Container>
void Random::shuffle(Container& container)
{
	std::shuffle(container.begin(), container.end(), generator);
}
