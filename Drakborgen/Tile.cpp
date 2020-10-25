#include "Tile.h"

Tile::Tile(const std::string& imagePath)
{
	texture.loadFromFile(imagePath);
	sprite.setTexture(texture);
	sprite.setScale(0.96f, 0.96f);
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
}
