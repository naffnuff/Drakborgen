#include "Button.h"

#include "Visual.h"

Button::Button(Visual* visual, std::function<void()> callback)
	: visual(visual)
	, callback(callback)
{
}

bool Button::hitTest(sf::Vector2f point) const
{
	sf::FloatRect bounds = visual->getBounds();
	return bounds.contains(point);
}

void Button::click()
{
	callback();
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*visual, states);
}
