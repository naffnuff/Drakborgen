#include <iostream>

#include "Setup.h"
#include "Game.h"

int main()
{
	try
	{
		Game game;
		Setup::setUpGame(game);
		game.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what();
		return 1;
	}
	return 0;
}