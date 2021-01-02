#pragma once

#include "Tile.h"
#include "UniqueSprite.h"

enum class Direction
{
	North,
	East,
	South,
	West,
	Invalid
};

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile();
	Tile(const std::string& imagePath);

	Tile(const Tile&) = delete;
	Tile& operator=(const Tile&) = delete;
	
	virtual ~Tile() { }

	virtual std::vector<std::vector<Direction>> getExits() const = 0;

	void setOrientation(Direction direction);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	UniqueSprite sprite;

	Direction orientation = Direction::North;
};

