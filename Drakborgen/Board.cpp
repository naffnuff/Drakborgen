#include "Board.h"

Board::Board()
{
	if (!texture.loadFromFile("Media/spelplan.jpg"))
	{
		throw std::runtime_error("No board image found");
	}
	sprite.setTexture(texture);
}

void Board::moveTile(std::unique_ptr<Tile>& tile, int row, int column)
{
	tiles[row][column] = std::move(tile);
	tile.reset();
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
}
