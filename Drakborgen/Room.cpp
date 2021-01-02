#include "Room.h"

std::vector<std::vector<Direction>> Room::getExits() const
{
	return { { Direction::East }, { Direction::West }, { Direction::North }, { Direction::South } };
}
