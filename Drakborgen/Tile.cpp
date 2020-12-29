#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(const std::string& imagePath)
	: sprite(imagePath)
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

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(sprite)
	{
		states.transform *= getTransform();
		target.draw(sprite.get(), states);
	}
}
