#include "Tile.h"

Tile::Tile(const std::string& imagePath)
{
	texture.loadFromFile(imagePath);
	sprite.setTexture(texture);
}
