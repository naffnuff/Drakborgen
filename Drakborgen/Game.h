#pragma once

#include "UniqueSprite.h"

#include "Hero.h"
#include "Card.h"
#include "CardDisplay.h"
#include "Board.h"
#include "Deck.h"
#include "Tile.h"
#include "Animation.h"
#include "Button.h"

#define CLASS(name) class name##State: public StateLogic { using StateLogic::StateLogic; public: State onLeftMouseClick(State state, Game& game) override; };
#define STATE(name) stateLogicMap[State::name] = std::make_unique<name##State>(State::name);

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
		NoState
	};

	class StateLogic
	{
	public:
		StateLogic(State thisState) : thisState(thisState) { }
		virtual ~StateLogic() { }
		
		State execute(Game& game);

	private:
		virtual State onLeftMouseClick(State state, Game&) { return state; }

	private:
		State thisState;
	};

	CLASS(PickHero);
	CLASS(PickStartTower);
	CLASS(PlayerMove);

public:
	Game();
	
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void run();
	sf::Vector2f getMouseBoardPosition() const;
	int getMouseOverItemIndex(sf::Vector2f mousePosition) const;
	std::vector<std::unique_ptr<Card>> getHeroCards();
	void displayCards(std::vector<std::unique_ptr<Card>>&& cards, std::function<void()> cardsDisplayedCallback);
	void createPlayer(int heroIndex);
	void placeNewPlayer(Board::Site site);
	void movePlayer(int index, Board::Site site);
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

	bool xCenteredBoard = false;
	bool yCenteredBoard = false;

	bool leftMouseButtonDown = false;
	sf::Vector2f buttonPressedBoardPosition;
	sf::Vector2f buttonPressedMousePosition;
	sf::Vector2f buttonReleasedMousePosition;
	int capturedItemIndex = 0;

	std::vector<Hero> heroes;
	std::vector<Player> players;

	std::vector<std::unique_ptr<Button>> buttons;

	int activePlayer = 0;

	std::map<State, std::unique_ptr<StateLogic>> stateLogicMap;

	AnimationManager animations;

private:
	void createStateLogicMap()
	{
		STATE(PickHero);
		STATE(PickStartTower);
		STATE(PlayerMove);
	}
};

