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
		std::cerr << "Uncaught std::exception: " << e.what();
	}
}