#pragma once

#include "UniqueSprite.h"

#include "State.h"
#include "Hero.h"
#include "Card.h"
#include "CardDisplay.h"
#include "Board.h"
#include "Deck.h"
#include "Tile.h"
#include "Animation.h"
#include "Button.h"

#include <map>

using TileDeck = Deck<Tile, 115>;

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
	Game();
	
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	TileDeck& getTiles() { return tiles; }

	void run();

private:
	State processEvents();

	State invokeEventHandler(std::map<State, std::function<State()>>& eventMap);

	sf::Vector2f correctBoardPosition(sf::Vector2f boardPostion);

	template<State>
	State onTick();

	template<State>
	State onLeftMouseClick();

	void placeAtOrigin(std::unique_ptr<Card>& card) const;
	void moveOffScreen(std::unique_ptr<Card>& card, float time, std::function<void()> callback);
	void moveToCenter(std::unique_ptr<Card>& card, std::function<void()> callback);
	sf::Vector2f getMouseBoardPosition() const;
	int getMouseOverItemIndex(sf::Vector2f mousePosition) const;
	std::vector<std::unique_ptr<Card>> getHeroCards();
	void displayCard(std::unique_ptr<Card>&& card, std::function<void()> callback);
	void displayCards(std::vector<std::unique_ptr<Card>>&& cards, std::function<void()> callback);
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
	Random random;
	sf::RenderWindow window;
	Board board;
	CardDisplay cardDisplay;
	TileDeck tiles;

	State state = State::PickHero;

	int playerCount = 4;

	bool xCenteredBoard = false;
	bool yCenteredBoard = false;

	bool leftMouseButtonDown = false;
	sf::Vector2f buttonPressedBoardPosition;
	sf::Vector2f buttonPressedMousePosition;
	sf::Vector2f buttonReleasedMousePosition;
	int capturedItemIndex = 0;

	bool preGameSetup = true;

	std::vector<Hero> idleHeroes;
	std::vector<Player> players;

	std::vector<std::unique_ptr<Button>> buttons;

	int activePlayerIndex = -1;

	std::map<State, std::function<State()>> onTickMap;
	std::map<State, std::function<State()>> onLeftMouseClickMap;

	AnimationManager animations;
};

