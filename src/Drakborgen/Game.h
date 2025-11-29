#pragma once

#include "State.h"
#include "Hero.h"
#include "Card.h"
#include "CardDisplay.h"
#include "Board.h"
#include "Deck.h"
#include "Tile.h"
#include "TextBox.h"
#include "UniqueSprite.h"

using TileDeck = Deck<Tile, 115>;

class Engine;

class Game
{
private:
	struct Player
	{
		Hero hero;
		Board::MoveSite boardSite;
		int life = 0;
		int avatarIndex = 0;
	};

public:
	Game(Engine& engine, Random& random, Board& board);
	
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	TileDeck& getTiles() { return tiles; }

private:
	void setState(State newState);

	template<State>
	void onBegin() {}

	template<State>
	void onTick() {}

	template<State>
	void onLeftMouseClick() {}

	template<State>
	void onEnd() {}

	void animate(sf::Transformable& transformable, sf::Vector2f target, float time, std::function<void()> doneCallback);

	void pickHero(int heroIndex);
	void requestGameStart();
	void placeAtOrigin(Card* card) const;
	void moveOffScreen(Card* card, float time, std::function<void()> callback);
	void moveToCenter(Card* card, std::function<void()> callback);
	std::vector<std::unique_ptr<Card>> getHeroCards();
	void displayCard(std::unique_ptr<Card>&& card, std::function<void()> callback);
	void displayCards(std::vector<std::unique_ptr<Card>>&& cards, std::function<void(int)> callback);
	void panToNextFreeTower();
	void createPlayer(int heroIndex);
	void placeNewPlayer(Board::Site site, std::function<void()> callback);
	void movePlayer(int index, Board::MoveSite moveSite, std::function<void()> callback);
	void onNewPlayerPlaced();
	void onPlayerMoved(Tile& tile);
	void startNewGame();
	void startPlayerRound();

	void createStateLogicMap();

private:
	Engine& engine;

	Random& random;
	Board& board;
	CardDisplay cardDisplay;
	TileDeck tiles;

	int playerCount = 4;

	bool preGameSetup = true;

	std::vector<Hero> idleHeroes;
	std::vector<Player> players;

	int activePlayerIndex = -1;

	template<State state, State endState, typename Handler>
	friend struct StateHandlerInitializer;
};

