#pragma once

#include "UniqueSprite.h"

#include "Hero.h"
#include "Card.h"
#include "CardDisplay.h"
#include "Board.h"
#include "Deck.h"
#include "Tile.h"

#define STATE(name) stateLogicMap[State::name] = std::make_unique<name>(State::name, *this);

using TileDeck = Deck<Tile, 115>;

class Game
{
private:
	struct Player
	{
		Hero hero;
		Board::Site boardSite;
		int life = 0;
	};

	enum class State
	{
		PickHero,
		PickStartTower,
		PlayerMove,
	};

	class StateLogic
	{
	public:
		StateLogic(State state, Game& game) : thisState(state), mGame(game) { }
		
		State execute();

	private:
		virtual State onLeftMouseClick(State state, Game&) { return state; };

	private:
		const State thisState;
		Game& mGame;
	};

	class PickHero : public StateLogic
	{
		using StateLogic::StateLogic;
	public:
		State onLeftMouseClick(State state, Game& game);
	};

	class PickStartTower : public StateLogic
	{
		using StateLogic::StateLogic;
	public:
		State onLeftMouseClick(State state, Game& game);
	};

	class PlayerMove : public StateLogic
	{
		using StateLogic::StateLogic;
	public:
		State onLeftMouseClick(State state, Game& game);
	};

public:
	Game();
	
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void run();
	void onHeroPicked(int heroIndex);
	Board::Site onStartingTowerPicked(sf::Vector2f mouseBoardPosition);
	void startNewGame();
	void startPlayerRound();
	TileDeck& getTiles() { return tiles; }

private:
	Random random;
	sf::RenderWindow window;
	Board board;
	CardDisplay cardDisplay;
	TileDeck tiles;

	int playerCount = 4;
	int currentPlayer = 0;

	bool leftMouseButtonDown = false;
	sf::Vector2f buttonPressedBoardPosition;
	sf::Vector2i buttonPressedMousePosition;
	sf::Vector2i buttonReleasedMousePosition;

	std::vector<Hero> heroes;
	std::vector<Player> players;

	int activePlayer = 0;

	std::map<State, std::unique_ptr<StateLogic>> stateLogicMap;

private:
	void createStateLogicMap()
	{
		STATE(PickHero);
		STATE(PickStartTower);
		STATE(PlayerMove);
	}
};

