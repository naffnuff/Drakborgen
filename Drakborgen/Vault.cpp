#include "Vault.h"

#include "System.h"

std::unique_ptr<Vault> Vault::makeVault(Direction section)
{
	switch (section)
	{
	case Direction::North:
		return std::make_unique<Vault>(
			std::vector<Direction>({ Direction::North }),
			std::vector<Direction>({ Direction::East }),
			std::vector<Direction>({ Direction::West }),
			std::vector<Direction>({ Direction::South, Direction::East }),
			std::vector<Direction>({ Direction::South, Direction::South }),
			std::vector<Direction>({ Direction::South, Direction::West })
			);
	case Direction::South:
		return std::make_unique<Vault>(
			std::vector<Direction>({ Direction::South }),
			std::vector<Direction>({ Direction::East }),
			std::vector<Direction>({ Direction::West }),
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
