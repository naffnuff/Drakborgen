#include "Tile.h"

#include <SFML/Graphics.hpp>

#include <iostream>

Tile::Tile()
{
}

Tile::Tile(const std::string& imagePath)
	: imagePath(imagePath)
	, sprite(imagePath)
{
}

void Tile::setOrientation(Direction direction)
{
	orientation = direction;
	sf::Vector2f size = sprite.get().getGlobalBounds().getSize();
	setRotation(float(orientation) * 90.0f);
	switch (orientation)
	{
	case Direction::East:
		move({ size.x, 0.0f });
		break;
	case Direction::South:
		move({ size.x, size.y });
		break;
	case Direction::West:
		move({ 0.0f, size.y });
		break;
	default:
		break;
	}
}

Direction Tile::applyOrientation(Direction direction, bool invert) const
{
	return Direction((int(direction) + (invert ? -int(orientation) : int(orientation))) % int(Direction::Count));
}

void Tile::transposeExits(std::vector<std::vector<Direction>>& exits) const
{
	for (std::vector<Direction>& exit : exits)
	{
		for (Direction& direction : exit)
		{
			direction = applyOrientation(direction, false);
		}
	}
	std::cout << imagePath << std::endl;
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (sprite)
	{
		states.transform *= getTransform();
		target.draw(sprite.get(), states);
	}
}
