#include "AnimationManager.h"

void AnimationManager::update(float elapsedTime, float)
{
	for (int i = int(animations.size()) - 1; i >= 0; --i)
	{
		if (animations[i].startTime == 0.0f)
		{
			animations[i].startTime = elapsedTime;
		}
		else
		{
			float timeProgress = (elapsedTime - animations[i].startTime) / animations[i].time;
			if (timeProgress >= 1.0f)
			{
				animations[i].card->getSprite().setPosition(animations[i].end);
				animations.erase(animations.begin() + i);
				break;
			}
			else
			{
				float spaceProgress = std::sqrt(1.0f - std::pow(1.0f - timeProgress, 2.0f));
				sf::Vector2f movement = (animations[i].end - animations[i].start) * spaceProgress;
				animations[i].card->getSprite().setPosition(animations[i].start + movement);
			}
		}
	}
}

void AnimationManager::clear()
{
	animations.clear();
}

void AnimationManager::add(Card& card, sf::Vector2f target, float time)
{
	animations.push_back({ &card, card.getSprite().getPosition(), target, time, 0.0f });
}
