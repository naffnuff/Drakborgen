#pragma once

#include "Tile.h"

#include "State.h"
#include "UniqueSprite.h"

#include <SFML/Graphics.hpp>

#include <vector>

enum class Direction
{
	Invalid = -1,
	North = 0,
	East = 1,
	South = 2,
	West = 3,
	Count = 4
};

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile();
	Tile(const std::string& imagePath);

	Tile(const Tile&) = delete;
	Tile& operator=(const Tile&) = delete;
	
	virtual ~Tile() { }

	virtual std::vector<std::vector<Direction>> getExits(Direction entrance) const = 0;

	virtual State enter() const = 0;

	void setOrientation(Direction direction);
	Direction applyOrientation(Direction direction, bool invert) const;

	void transposeExits(std::vector<std::vector<Direction>>& exits) const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::string imagePath;

	UniqueSprite sprite;

	Direction orientation = Direction::North;
};

inline std::ostream& operator<<(std::ostream& o, Direction direction)
{
	std::string name;
	switch (direction)
	{
	case Direction::Invalid:
		return o << "Invalid";
	case Direction::North:
		return o << "North";
	case Direction::East:
		return o << "East";
	case Direction::South:
		return o << "South";
	case Direction::West:
		return o << "West";
	case Direction::Count:
		return o << "Count";
	default:
		return o;
	}
}
