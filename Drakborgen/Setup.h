#pragma once

#include "Game.h"

class Setup
{
public:
	static void setUpGame(Game& game);

private:
	static void createTiles(Deck<Tile>& tiles);
	static void createTile(Deck<Tile>& tiles, const std::string& number);
};

