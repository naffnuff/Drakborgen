#pragma once

#include "Tile.h"

class Vault : public Tile
{
public:
	template<typename... Args>
	Vault(Args... args)
	{
		(exits.push_back({ args }), ...);
	}

	static std::unique_ptr<Vault> makeVault(Direction section);

	std::vector<std::vector<Direction>> getExits(Direction entrance) const override;

	State enter() const override;

private:
	std::vector<std::vector<Direction>> exits;
};

