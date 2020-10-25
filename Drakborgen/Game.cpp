#include "Game.h"

#include <iostream>
#include <memory>

#include "Tile.h"

Game::Game()
	: window(sf::VideoMode::getDesktopMode(), "Drakborgen", sf::Style::Fullscreen)
{
	std::unique_ptr<Tile> tile;
	board.moveTile(tile, 1, 1);
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
