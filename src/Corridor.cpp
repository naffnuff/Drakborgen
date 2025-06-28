#include "Corridor.h"

#include <iostream>

State Corridor::enter() const
{
	std::cout << " -> turn continue" << std::endl;
	return State::TurnContinue;
}
