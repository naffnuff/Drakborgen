#pragma once

#include "Room.h"

class SplitRoom : public Room
{
public:
	SplitRoom(const std::string& imagePath)
		: Room(imagePath)
	{
	}

	std::vector<std::vector<Direction>> getExits(Direction entrance) const override;

private:
	std::vector<std::vector<Direction>> northSection = { { Direction::North }, { Direction::East } };
	std::vector<std::vector<Direction>> southSection = { { Direction::South }, { Direction::West } };
};

