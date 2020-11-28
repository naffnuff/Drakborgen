#pragma once

#include "TileLogic.h"

class Tower : public TileLogic
{
public:
	template<typename... Args>
	Tower(Args... args)
	{
		(exits.push_back(args), ...);
	}

	virtual std::vector<Exit>& getExits() override;

private:
	std::vector<Exit> exits;
};

