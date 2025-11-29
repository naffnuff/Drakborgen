#pragma once

#include "Visual.h"

#include "SFML/Graphics.hpp"

#include <functional>

class TextBox : public Visual
{
public:
	TextBox(const std::string& message, sf::Vector2f size, sf::Vector2f position, int textSize);

	TextBox(const TextBox&) = delete;
	TextBox& operator=(const TextBox&) = delete;

	sf::FloatRect getBounds() const override;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setText(const std::string& value);
	std::string getText() const;

private:
	sf::Font font;
	sf::Text text;
	sf::RectangleShape background;
};

