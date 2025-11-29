#include "Vault.h"

#include "System.h"

#include <iostream>

std::unique_ptr<Vault> Vault::makeVault(Direction section)
{
	switch (section)
	{
	case Direction::North:
		return std::make_unique<Vault>(
			Direction::North,
			Direction::East,
			Direction::West,
			std::vector<Direction>({ Direction::South, Direction::East }),
			std::vector<Direction>({ Direction::South, Direction::South }),
			std::vector<Direction>({ Direction::South, Direction::West })
		);
	case Direction::South:
		return std::make_unique<Vault>(
			Direction::South,
			Direction::East,
			Direction::West,
			std::vector<Direction>({ Direction::North, Direction::East }),
			std::vector<Direction>({ Direction::North, Direction::North }),
			std::vector<Direction>({ Direction::North, Direction::West })
		);
	default:
		THROW;
	}
}

std::vector<std::vector<Direction>> Vault::getExits(Direction) const
{
	return exits;
}

State Vault::enter() const
{
	std::cout << " -> turn end" << std::endl;
	return State::TurnEnd;
}
