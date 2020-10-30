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
{
	createStateLogicMap();
}

Game::State Game::StateLogic::execute()
{
	sf::Event event;
	while (game.window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			game.window.close();
		}
		else if (event.type == sf::Event::EventType::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				game.leftMouseButtonDown = true;
				game.buttonPressedMousePosition.x = event.mouseButton.x;
				game.buttonPressedMousePosition.y = event.mouseButton.y;
				game.buttonPressedBoardPosition = game.board.getPosition();
			}
		}
		if (event.type == sf::Event::EventType::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				game.leftMouseButtonDown = false;
				game.buttonReleasedMousePosition.x = event.mouseButton.x;
				game.buttonReleasedMousePosition.y = event.mouseButton.y;
				if (game.buttonPressedMousePosition == game.buttonReleasedMousePosition)
				{
					state = onLeftMouseClick(state, game);
				}
			}
		}
		if (event.type == sf::Event::EventType::MouseMoved)
		{
			if (game.leftMouseButtonDown)
			{
				sf::Vector2f mouseMovementSincePressed(float(event.mouseMove.x - game.buttonPressedMousePosition.x), float(event.mouseMove.y - game.buttonPressedMousePosition.y));
				sf::Vector2f newBoardPosition = game.buttonPressedBoardPosition + mouseMovementSincePressed;
				game.board.setPosition(newBoardPosition);
			}
		}
	}
	return state;
}

Game::State Game::PickHero::onLeftMouseClick(State state, Game& game)
{
	int heroIndex = game.cardDisplay.hitTest(game.buttonPressedMousePosition);
	if( heroIndex >= 0 )
	{
		game.heroes[heroIndex].placeStatsCard(game.cardDisplay.pullCard(heroIndex));
		game.players.push_back({ std::move(game.heroes[heroIndex]), Board::invalidSite, game.heroes[heroIndex].getMaxLife()});
		game.heroes.erase(game.heroes.begin() + heroIndex);

		game.board.showClickSites(true);
		return State::PickStartTower;
	}
	return state;
}

Game::State Game::PickStartTower::onLeftMouseClick(State state, Game& game)
{
	sf::Vector2f boardPosition = game.board.getPosition();
	sf::Vector2f mouseBoardPosition = sf::Vector2f(game.buttonReleasedMousePosition.x - boardPosition.x, game.buttonReleasedMousePosition.y - boardPosition.y );
	if (game.board.testClickSites(mouseBoardPosition))
	{
		game.board.showClickSites(false);
		Board::Site site = game.board.getSite(mouseBoardPosition);
		int index = int(game.players.size()) - 1;
		Player& player = game.players[index];
		player.boardSite = site;
		game.board.addPlayer("Media/hjaltekort/" + player.hero.getId() + "gubbe.png", index);
		game.board.setPlayerSite(index, site);
		if(game.cardDisplay.empty())
		{
			game.board.clearClickSites();
			return State::PlayerMove;
		}
		else
		{
			game.board.removeClickSite(site);
			return State::PickHero;
		}
	}
	return state;
}

Game::State Game::PlayerMove::onLeftMouseClick(State state, Game& game)
{
	return state;
}

void Game::run()
{
	sf::Clock clock;
	float lastTimestamp = 0.0f;
	tiles.shuffle();
	for (int row = 0; row < board.rowCount; ++row)
	{
		for (int column = 0; column < board.columnCount; ++column)
		{
			if (!tiles.isEmpty() && !board.hasTile({ row, column }))
			{
				board.placeTile(tiles.pullNextItem(), { row, column });
			}
		}
	}

	//heroes.reserve(4);
	heroes.push_back(Hero("rohan", "Riddar Rohan", 15));
	heroes.push_back(Hero("sigeir", "Sigeir Skarpyxe", 14));
	heroes.push_back(Hero("aelfric", "Aelfric Brunkåpa", 10));
	heroes.push_back(Hero("bardhor", "Bardhor Bågman", 8));
	
	board.setClickSites({ { 0, 0 }, { 0, board.columnCount - 1 }, { board.rowCount - 1, 0 }, { board.rowCount - 1, board.columnCount - 1 } });

	for (Hero& hero : heroes)
	{
		cardDisplay.pushCard(hero.pullStatsCard());
	}

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
