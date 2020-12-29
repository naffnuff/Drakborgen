#include "Room.h"

Room::Room(const std::string& imagePath)
	: Tile::Tile(imagePath)
{

}

std::vector<Direction> Room::getExits() const
{
	return { Direction::East, Direction::West, Direction::North, Direction::South };
}
