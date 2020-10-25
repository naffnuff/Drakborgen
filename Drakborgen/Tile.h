#pragma once

#include <SFML/Graphics.hpp>

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile(const std::string& imagePath);
private:
	sf::Texture texture;
	sf::Sprite sprite;
};

