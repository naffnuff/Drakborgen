#include "Board.h"

Board::Board()
{
	if (!texture.loadFromFile("Media/spelplan.jpg"))
	{
		throw std::runtime_error("No board image found");
	}
	sprite.setTexture(texture);
}

void Board::placeTile(std::unique_ptr<Tile>& tile, int row, int column)
{
	if (hasTile(row, column) || !withinBounds(row, column))
	{
		throw std::runtime_error("Illegal tile placement");
	}
	constexpr float gridOriginX = 409.f;
	constexpr float gridOriginY = 119.f;
	constexpr float tileSize = 175.f;
	tile->setPosition(gridOriginX + tileSize * column, gridOriginY + tileSize * row);
	tiles[row][column] = std::move(tile);
	tile.reset();
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
	for (const std::array<std::unique_ptr<Tile>, 13>& row : tiles)
	{
		for (const std::unique_ptr<Tile>& tile : row)
		{
			if (tile)
			{
				target.draw(*tile, states);
			}
		}
	}
}

bool Board::hasTile(int row, int column)
{
	return !!tiles[row][column];
}

bool Board::withinBounds(int row, int column)
{
	return row >= 0 && row < rowCount && column >= 0 && column < columnCount;
}
