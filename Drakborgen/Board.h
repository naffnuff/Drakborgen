#pragma once

#include <memory>
#include <array>

#include <SFML/Graphics.hpp>

#include "Tile.h"

class Board : public sf::Drawable, public sf::Transformable
{
public:
	static constexpr int rowCount = 10;
	static constexpr int columnCount = 13;

	Board();

	bool hasTile(int row, int column);
	void placeTile(std::unique_ptr<Tile>& tile, int row, int column);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool withinBounds(int row, int column);

private:
	sf::Texture texture;
	sf::Sprite sprite;
	std::array<std::array<std::unique_ptr<Tile>, columnCount>, rowCount> tiles;
};

