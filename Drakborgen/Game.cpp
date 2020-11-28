#include "Game.h"

#include <iostream>
#include <memory>

#include "Tile.h"

Game::Game()
#ifdef _DEBUG
	: window(sf::VideoMode::getDesktopMode(), "Drakborgen")
#else
	: window(sf::VideoMode::getDesktopMode(), "Drakborgen", sf::Style::Fullscreen)
#endif // DEBUG
	, cardDisplay(window)
	, tiles(random)
{
	createStateLogicMap();
}

Game::State Game::StateLogic::execute()
{
	State state = thisState;
	sf::Event event;
	while (mGame.window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			mGame.window.close();
		}
		else if (event.type == sf::Event::EventType::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				mGame.leftMouseButtonDown = true;
				mGame.buttonPressedMousePosition.x = event.mouseButton.x;
				mGame.buttonPressedMousePosition.y = event.mouseButton.y;
				mGame.buttonPressedBoardPosition = mGame.board.getPosition();
			}
		}
		if (event.type == sf::Event::EventType::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Left && mGame.leftMouseButtonDown)
			{
				mGame.leftMouseButtonDown = false;
				mGame.buttonReleasedMousePosition.x = event.mouseButton.x;
				mGame.buttonReleasedMousePosition.y = event.mouseButton.y;
				if (mGame.buttonPressedMousePosition == mGame.buttonReleasedMousePosition)
				{
					state = onLeftMouseClick(state, mGame);
				}
			}
		}
		if (event.type == sf::Event::EventType::MouseMoved)
		{
			if (mGame.leftMouseButtonDown)
			{
				sf::Vector2f mouseMovementSincePressed(float(event.mouseMove.x - mGame.buttonPressedMousePosition.x), float(event.mouseMove.y - mGame.buttonPressedMousePosition.y));
				sf::Vector2f newBoardPosition = mGame.buttonPressedBoardPosition + mouseMovementSincePressed;
				mGame.board.setPosition(newBoardPosition);
			}
		}
	}
	return state;
}

Game::State Game::PickHero::onLeftMouseClick(State state, Game& game)
{
	std::cout << "pick hero ";
	int heroIndex = game.cardDisplay.hitTest(game.buttonPressedMousePosition);
	if( heroIndex >= 0 )
	{
		game.onHeroPicked(heroIndex);
		std::cout << " -> pick start tower" << std::endl;
		return State::PickStartTower;
	}
	std::cout << " -> pick hero" << std::endl;
	return state;
}

Game::State Game::PickStartTower::onLeftMouseClick(State state, Game& game)
{
	std::cout << "pick start tower ";
	sf::Vector2f boardPosition = game.board.getPosition();
	sf::Vector2f mouseBoardPosition = sf::Vector2f(game.buttonReleasedMousePosition.x - boardPosition.x, game.buttonReleasedMousePosition.y - boardPosition.y );
	if (game.board.testClickSites(mouseBoardPosition))
	{
		Board::Site pickedSite = game.onStartingTowerPicked(mouseBoardPosition);
		if (game.cardDisplay.empty())
		{
			game.startNewGame();
			std::cout << " -> pick player move" << std::endl;
			return State::PlayerMove;
		}
		else
		{
			game.board.removeClickSite(pickedSite);
			std::cout << " -> pick hero" << std::endl;
			return State::PickHero;
		}
	}
	std::cout << " -> pick start tower" << std::endl;
	return state;
}

Game::State Game::PlayerMove::onLeftMouseClick(State state, Game& /*game*/)
{
	return state;
}

void Game::run()
{
	sf::Clock clock;
	float lastTimestamp = 0.0f;
	tiles.shuffle();
	/*for (int row = 0; row < board.rowCount; ++row)
	{
		for (int column = 0; column < board.columnCount; ++column)
		{
			if (!tiles.isEmpty() && !board.hasTile({ row, column }))
			{
				board.placeTile(tiles.pullNextItem(), { row, column });
			}
		}
	}*/

	heroes.reserve(4);
	heroes.push_back(Hero("rohan", "Riddar Rohan", 15));
	heroes.push_back(Hero("sigeir", "Sigeir Skarpyxe", 14));
	heroes.push_back(Hero("aelfric", "Aelfric Brunkåpa", 10));
	heroes.push_back(Hero("bardhor", "Bardhor Bågman", 8));

	board.setGameStartClickSites();

	for (Hero& hero : heroes)
	{
		cardDisplay.pushCard(hero.pullStatsCard());
	}

	State state = State::PickHero;

	while (window.isOpen())
	{
		state = stateLogicMap[state]->execute();

		sf::Time time = clock.getElapsedTime();
		float timestamp = time.asSeconds();
		float timeDelta = timestamp - lastTimestamp;
		lastTimestamp = timestamp;

		board.update(timestamp, timeDelta);

		window.clear();

		window.draw(board);
		window.draw(cardDisplay);

		window.display();
	}
}

void Game::onHeroPicked(int heroIndex)
{
	heroes[heroIndex].placeStatsCard(cardDisplay.pullCard(heroIndex));
	players.push_back({ std::move(heroes[heroIndex]), Board::invalidSite, heroes[heroIndex].getMaxLife() });
	heroes.erase(heroes.begin() + heroIndex);
	for (int i = int(heroes.size()) - 1; i >= 0; --i)
	{
		heroes[i].placeStatsCard(cardDisplay.pullCard(i));
	}
	board.showClickSites(true);
}

Board::Site Game::onStartingTowerPicked(sf::Vector2f mouseBoardPosition)
{
	board.showClickSites(false);
	Board::Site site = board.getSite(mouseBoardPosition);
	int index = int(players.size()) - 1;
	Player& player = players[index];
	player.boardSite = site;
	board.addPlayer("Media/hjaltekort/" + player.hero.getId() + "gubbe.png", index);
	board.setPlayerSite(index, site);
	for (int i = 0; i < heroes.size(); ++i)
	{
		cardDisplay.pushCard(heroes[i].pullStatsCard());
	}
	return site;
}

void Game::startNewGame()
{
	board.clearClickSites();
	random.shuffle(players);
	startPlayerRound();
}

void Game::startPlayerRound()
{
	Player& player = players[activePlayer];
	board.setPlayerMoveClickSites(player.boardSite);
	board.showClickSites(true);
}
