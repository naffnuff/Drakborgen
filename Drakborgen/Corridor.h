#pragma once

#include "Room.h"

class Corridor : public Room
{
public:
	template<typename... Args>
	Corridor(const std::string& imagePath, Args... args)
		: Room(imagePath, args...)
	{
	}

	State enter() const override;
};

