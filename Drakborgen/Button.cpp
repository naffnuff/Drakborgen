#include "Button.h"

#include "System.h"

Button::Button(const std::string& message, sf::Vector2f size, sf::Vector2f position, std::function<void()> callback)
	: callback(callback)
{
	if (!font.loadFromFile("Media/ITCBLKAD.TTF"))
	{
		THROW;
	}

	background.setFillColor(sf::Color::Black);
	background.setPosition(position);
	background.setSize(size);

	// select the font
	text.setFont(font); // font is a sf::Font

	// set the string to display
	text.setString(message);

	// set the character size
	text.setCharacterSize(60); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold);

	sf::Vector2f textSize = text.getGlobalBounds().getSize();
	text.setPosition({ position.x + size.x / 2.0f - textSize.x / 2.0f, position.y + size.y / 2.0f - textSize.y });
}

bool Button::hitTest(sf::Vector2f point, bool callbackOnHit) const
{
	sf::FloatRect buttonRectangle = background.getGlobalBounds();
	bool isHit = buttonRectangle.contains(point);
	if (isHit && callbackOnHit)
	{
		callback();
	}
	return isHit;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background, states);
	target.draw(text, states);
}
