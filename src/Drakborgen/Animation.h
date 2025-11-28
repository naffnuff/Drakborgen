#pragma once

#include <functional>

#include "SFML/Graphics.hpp"

class AnimationManager
{
private:
	struct Animation
	{
		sf::Transformable* transformable;
		sf::Vector2f start;
		sf::Vector2f end;
		float time;
		float startTime;
		std::function<void()> doneCallback;
	};

public:
	void update(float elapsedTime, float timeDelta);
	void add(sf::Transformable& transformable, sf::Vector2f target, float time, std::function<void()> doneCallback);
	void remove(sf::Transformable& transformable);

private:
	std::vector<Animation> animations;
};

