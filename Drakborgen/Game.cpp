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
{
	{
		std::unique_ptr<Tile> tile = std::make_unique<Tile>("Media/rumsbrickor/rumsbricka0000.png");
		board.placeTile(tile, 0, 1);
	}
	{
		std::unique_ptr<Tile> tile = std::make_unique<Tile>("Media/rumsbrickor/rumsbricka0001.png");
		board.placeTile(tile, 0, 2);
	}
	{
		std::unique_ptr<Tile> tile = std::make_unique<Tile>("Media/rumsbrickor/rumsbricka0002.png");
		board.placeTile(tile, 1, 0);
	}
	{
		std::unique_ptr<Tile> tile = std::make_unique<Tile>("Media/rumsbrickor/rumsbricka0003.png");
		board.placeTile(tile, 2, 0);
	}
}

void Game::run()
{
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

		window.clear();

		window.draw(board);

		window.display();
	}
}
