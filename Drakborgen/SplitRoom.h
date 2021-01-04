#pragma once

#include "Tile.h"

class SplitRoom : public Tile
{
public:
	SplitRoom(const std::string& imagePath)
		: Tile(imagePath)
	{
	}

	std::vector<std::vector<Direction>> getExits(Direction entrance) const override;

private:
	std::vector<std::vector<Direction>> northSection = { { Direction::North }, { Direction::East } };
	std::vector<std::vector<Direction>> southSection = { { Direction::South }, { Direction::West } };
};

