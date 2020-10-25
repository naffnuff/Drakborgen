#pragma once

#include <memory>
#include <array>

#include <SFML/Graphics.hpp>

#include "Tile.h"

class Board : public sf::Drawable, public sf::Transformable
{
public:
	Board();

	void moveTile(std::unique_ptr<Tile>& tile, int row, int column);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Texture texture;
	sf::Sprite sprite;
	std::array<std::array<std::unique_ptr<Tile>, 13>, 10> tiles;
};

