#pragma once

#include "Game.h"

class Setup
{
public:
	Setup() = delete;
	Setup(const Setup&) = delete;
	Setup& operator=(const Setup&) = delete;

	static void setUpGame(Game& game);

private:
	static void createTiles(TileDeck& tiles);
	static void createTile(TileDeck& tiles, const std::string& number);
};

