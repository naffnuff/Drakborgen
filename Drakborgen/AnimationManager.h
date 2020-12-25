#pragma once

#include "Card.h"

#include <vector>

class AnimationManager
{
private:
	struct Animation
	{
		Card* card;
		sf::Vector2f start;
		sf::Vector2f end;
		float time;
		float startTime;
	};

public:
	void update(float elapsedTime, float timeDelta);
	void clear();
	void add(Card& card, sf::Vector2f target, float time);

private:
	std::vector<Animation> animations;
};

