#include <iostream>

#include "Animation.h"
#include "Setup.h"
#include "Game.h"
#include "Engine.h"
#include "MainMenu.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
	try
	{
		AnimationManager animations;
		Board board(animations);
		Engine engine(animations, board);
		Random random;
		MainMenu mainMenu(engine, random);
		Game game(engine, random, board);
		Setup::setUpGame(game);
		engine.run();
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