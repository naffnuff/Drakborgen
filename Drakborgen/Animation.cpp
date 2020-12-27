#include "Animation.h"

void AnimationManager::update(float elapsedTime, float)
{
	std::vector<std::function<void()>> doneCallbacks;
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
				animations[i].transformable->setPosition(animations[i].end);
				doneCallbacks.push_back(animations[i].doneCallback);
				animations.erase(animations.begin() + i);
				break;
			}
			else
			{
				float spaceProgress = std::sqrt(1.0f - std::pow(1.0f - timeProgress, 2.0f));
				sf::Vector2f movement = (animations[i].end - animations[i].start) * spaceProgress;
				animations[i].transformable->setPosition(animations[i].start + movement);
			}
		}
	}
	for (std::function<void()> callback : doneCallbacks)
	{
		callback();
	}
}

void AnimationManager::add(sf::Transformable& transformable, sf::Vector2f target, float time, std::function<void()> doneCallback)
{
	animations.push_back({ &transformable, transformable.getPosition(), target, time, 0.0f, doneCallback });
}
