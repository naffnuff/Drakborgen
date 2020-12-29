#pragma once

#include <functional>

#include "SFML/Graphics.hpp"

class Button : public sf::Drawable
{
public:
	Button(const std::wstring& message, sf::Vector2f size, sf::Vector2f position, int textSixe);

	Button(const Button&) = delete;
	Button& operator=(const Button&) = delete;

	bool hitTest(sf::Vector2f point) const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Font font;
	sf::Text text;
	sf::RectangleShape background;
};

