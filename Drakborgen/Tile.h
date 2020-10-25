#pragma once

#include <SFML/Graphics.hpp>

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile(const std::string& imagePath);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Texture texture;
	sf::Sprite sprite;
};

