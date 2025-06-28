#include <iostream>

#include "Setup.h"
#include "Game.h"

#ifdef _WIN32
#include <windows.h>
#endif

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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WORD(FOREGROUND_RED | FOREGROUND_INTENSITY));
		std::cerr << std::endl << e.what() << std::endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WORD(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
		return 1;
	}
	return 0;
}