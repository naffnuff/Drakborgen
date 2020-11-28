#pragma once

#include <vector>

class TileLogic
{
public:
	enum class Exit
	{
		North,
		South,
		West,
		East
	};

public:
	virtual ~TileLogic() { }

	virtual std::vector<Exit>& getExits() = 0;
};

