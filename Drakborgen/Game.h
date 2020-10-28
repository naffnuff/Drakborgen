#pragma once

#include "UniqueSprite.h"

#include "Hero.h"
#include "Card.h"
#include "CardDisplay.h"
#include "Board.h"
#include "Deck.h"
#include "Tile.h"

using TileDeck = Deck<Tile, 115>;

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
	
	Game(Game&) = delete;
	void operator=(Game&) = delete;

	void run();
	TileDeck& getTiles() { return tiles; }

private:
	sf::RenderWindow window;
	Board board;
	CardDisplay cardDisplay;
	TileDeck tiles;
	State state = State::PickHero;

	int playerCount = 4;
	int currentPlayer = 0;

	bool leftMouseButtonDown = false;
	sf::Vector2f buttonPressedBoardPosition;
	sf::Vector2i buttonPressedMousePosition;

	std::vector<Hero> heroes;
	std::vector<Player> players;
};

