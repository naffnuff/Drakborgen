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
		StateLogic(State state, Game& game) : state(state), game(game) { }
		
		State execute();

	private:
		virtual State onLeftMouseClick(State state, Game& game) { return state; };

	private:
		State state;
		Game& game;
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
	
	Game(Game&) = delete;
	Game& operator=(Game&) = delete;

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
	sf::Vector2i buttonReleasedMousePosition;

	std::vector<Hero> heroes;
	std::vector<Player> players;

	std::map<State, std::unique_ptr<StateLogic>> stateLogicMap;

private:
	void createStateLogicMap()
	{
		STATE(PickHero);
		STATE(PickStartTower);
		STATE(PlayerMove);
	}
};

