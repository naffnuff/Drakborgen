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
	heroes.push_back(Hero("rohan", "Riddar Rohan"));
	heroes.push_back(Hero("sigeir", "Sigeir Skarpyxe"));
	heroes.push_back(Hero("aelfric", "Aelfric Brunkåpa"));
	heroes.push_back(Hero("bardhor", "Bardhor Bågman"));

	for (Hero& hero : heroes)
	{
		cardDisplay.pushCard(hero.pullStatsCard());
	}
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::EventType::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					leftMouseButtonDown = true;
					buttonPressedMousePosition.x = event.mouseButton.x;
					buttonPressedMousePosition.y = event.mouseButton.y;
					buttonPressedBoardPosition = board.getPosition();
				}
			}
			if (event.type == sf::Event::EventType::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					leftMouseButtonDown = false;
					const bool isClick = buttonPressedMousePosition.x == event.mouseButton.x && buttonPressedMousePosition.y == event.mouseButton.y;
					if (state == State::PickHero)
					{
						if (isClick)
						{
							int heroIndex = cardDisplay.hitTest(buttonPressedMousePosition);
							if (heroIndex >= 0)
							{
								players.emplace_back();
								players.back().heroIndex = heroIndex;
								heroes[heroIndex].placeStatsCard(cardDisplay.pullCard(heroIndex));
								board.setClickSites({ { 0, 0 }, { 0, board.columnCount - 1 }, { board.rowCount - 1, 0 }, { board.rowCount - 1, board.columnCount - 1 } });
								state = State::PickStartTower;
							}
						}
					}
					else if (state == State::PickStartTower)
					{
						if (isClick)
						{
							sf::Vector2f boardPosition = board.getPosition();
							sf::Vector2f mouseBoardPosition = sf::Vector2f( event.mouseButton.x - boardPosition.x, event.mouseButton.y - boardPosition.y );
							if (board.testClickSites(mouseBoardPosition))
							{
								Board::Site site = board.getSite(mouseBoardPosition);
								int index = players.size() - 1;
								Player& player = players[index];
								player.boardSite = site;
								board.addPlayer("Media/hjaltekort/" + heroes[player.heroIndex].getId() + "gubbe.png", index);
								board.setPlayerSite(index, site);
								if(cardDisplay.empty())
								{
									board.clearClickSites();
									state = State::PlayerMove;
								}
								else
								{
									board.removeClickSite(site);
									state = State::PickHero;
								}
							}
						}
					}
					else if (state == State::PickStartTower)
					{
					}
				}
			}
			if (event.type == sf::Event::EventType::MouseMoved)
			{
				if (leftMouseButtonDown)
				{
					sf::Vector2f mouseMovementSincePressed(float(event.mouseMove.x - buttonPressedMousePosition.x), float(event.mouseMove.y - buttonPressedMousePosition.y));
					sf::Vector2f newBoardPosition = buttonPressedBoardPosition + mouseMovementSincePressed;
					board.setPosition(newBoardPosition);
				}
			}
		}

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
