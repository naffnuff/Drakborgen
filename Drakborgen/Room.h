#pragma once

#include "Tile.h"

class Room : public Tile
{
public:
	Room(const std::string& imagePath);

	virtual std::vector<Direction> getExits() const override;
};

