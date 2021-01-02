#pragma once

#include "Tile.h"

class Room : public Tile
{
public:
	template<typename... Args>
	Room(const std::string& imagePath, Args... args)
		: Tile(imagePath)
	{
		(exits.push_back({ args }), ...);
	}

	virtual std::vector<std::vector<Direction>> getExits() const override;

private:
	std::vector<std::vector<Direction>> exits;
};

