#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(const std::string& imagePath)
{
	if (!texture.loadFromFile(imagePath))
	{
		throw std::runtime_error("Could not find tile image " + imagePath);
	}
	sprite.setTexture(texture);
	sprite.setScale(0.96f, 0.96f);
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
}
