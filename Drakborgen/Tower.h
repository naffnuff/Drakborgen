#pragma once

#include "Tile.h"

class Tower : public Tile
{
public:
	template<typename... Args>
	Tower(Args... args)
	{
		(exits.push_back(args), ...);
	}

	virtual std::vector<Direction> getExits() const override;

private:
	std::vector<Direction> exits;
};

