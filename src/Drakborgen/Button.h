#pragma once

#include "SFML/Graphics.hpp"

#include <functional>

class Visual;

class Button : public sf::Drawable
{
public:
	Button(Visual* visual, std::function<void()> callback);

	bool hitTest(sf::Vector2f point) const;

	void click();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Visual* visual = nullptr;
	std::function<void()> callback;
};

