#include "Tile.h"

Tile::Tile(std::unique_ptr<TileLogic> tileLogic)
	: tileLogic(std::move(tileLogic))
{
}

Tile::Tile(const std::string& imagePath)
	: sprite(imagePath)
{
	sprite.get().setScale(0.96f, 0.96f);
}

std::unique_ptr<TileLogic>& Tile::getLogic()
{
	return tileLogic;
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(sprite)
	{
		states.transform *= getTransform();
		target.draw(sprite.get(), states);
	}
}
