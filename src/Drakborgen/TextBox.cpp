#include "TextBox.h"

#include "System.h"
#include "Setup.h"

TextBox::TextBox(const std::string& message, sf::Vector2f size, sf::Vector2f position, int textSize)
{
	if (!font.loadFromFile(Setup::getMediaPath() + "ITCBLKAD.TTF"))
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
	text.setCharacterSize(textSize); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold);

	sf::Vector2f globalTextSize = text.getGlobalBounds().getSize();
	text.setPosition({ position.x + size.x / 2.0f - globalTextSize.x / 2.0f, position.y + size.y / 2.0f - 45.0f });
}

sf::FloatRect TextBox::getBounds() const
{
	return background.getGlobalBounds();
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background, states);
	target.draw(text, states);
}

void TextBox::setText(const std::string& value)
{
	text.setString(sf::String::fromUtf8(value.begin(), value.end()));
}

std::string TextBox::getText() const
{
	return text.getString();
}
