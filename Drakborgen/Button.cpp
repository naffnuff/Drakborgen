#include "Button.h"

#include "System.h"

Button::Button(const std::string& message, sf::Vector2f size, sf::Vector2f position, int textSixe)
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
	setText(message);

	// set the character size
	text.setCharacterSize(textSixe); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold);

	sf::Vector2f textSize = text.getGlobalBounds().getSize();
	text.setPosition({ position.x + size.x / 2.0f - textSize.x / 2.0f, position.y + size.y / 2.0f - 45.0f });
}

bool Button::hitTest(sf::Vector2f point) const
{
	sf::FloatRect buttonRectangle = background.getGlobalBounds();
	return buttonRectangle.contains(point);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background, states);
	target.draw(text, states);
}

void Button::setText(const std::string& value)
{
	text.setString(sf::String::fromUtf8(value.begin(), value.end()));
}

std::string Button::getText() const
{
	return text.getString();
}
