#pragma once

#include <SFML/Graphics.hpp>

#include "Hero.h"
#include "Card.h"
#include "CardDisplay.h"
#include "Board.h"
#include "Deck.h"
#include "Tile.h"

class Game
{
private:
	struct Player
	{
		int heroIndex = 0;
		Board::Site boardSite;
		int life = 0;
	};

	enum class State {
		PickHero,
		PickStartTower,
		PlayerMove,
	};

public:
	Game();
	void run();
	Deck<Tile>& getTiles() { return tiles; }

private:
	sf::RenderWindow window;
	Board board;
	CardDisplay cardDisplay;
	Deck<Tile> tiles;
	State state = State::PickHero;

	int playerCount = 4;
	int currentPlayer = 0;

	bool leftMouseButtonDown = false;
	sf::Vector2f buttonPressedBoardPosition;
	sf::Vector2i buttonPressedMousePosition;

	std::vector<Hero> heroes;
	std::vector<Player> players;
};

