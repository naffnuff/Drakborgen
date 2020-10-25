#pragma once

#include <SFML/Graphics.hpp>

#include "Board.h"

class Game
{
public:
	Game();
	void run();

private:
	sf::RenderWindow window;
	Board board;

	bool leftMouseButtonDown = false;
	sf::Vector2f buttonPressedBoardPosition;
	sf::Vector2i buttonPressedMousePosition;
};

