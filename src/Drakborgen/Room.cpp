#include "Room.h"

#include "System.h"

#include <iostream>

std::vector<std::vector<Direction>> Room::getExits(Direction entrance) const
{
	if (entrance == Direction::Invalid || entrance == Direction::Count)
	{
		THROW;
	}
	std::vector<std::vector<Direction>> result = exits;
	transposeExits(result);
	return result;
}

State Room::enter() const
{
	std::cout << " -> turn end" << std::endl;
	return State::TurnEnd;
}
