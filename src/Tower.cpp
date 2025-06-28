#include "Tower.h"

#include <iostream>

std::vector<std::vector<Direction>> Tower::getExits(Direction) const
{
	return exits;
}

State Tower::enter() const
{
	std::cout << " -> turn continue" << std::endl;
	return State::TurnContinue;
}
