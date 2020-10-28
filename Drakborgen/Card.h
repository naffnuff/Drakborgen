#pragma once

#include <SFML/Graphics.hpp>

class Card : public sf::Drawable, public sf::Transformable
{
public:
	Card(const std::string& imagePath);

	void centerAround(sf::Vector2f position);
	sf::FloatRect getBounds();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Texture texture;
	sf::Sprite sprite;
};

