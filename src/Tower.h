#pragma once

#include "Tile.h"

class Tower : public Tile
{
public:
	template<typename... Args>
	Tower(Args... args)
	{
		(exits.push_back({ args }), ...);
	}

	std::vector<std::vector<Direction>> getExits(Direction entrance) const override;

	State enter() const override;

private:
	std::vector<std::vector<Direction>> exits;
};

