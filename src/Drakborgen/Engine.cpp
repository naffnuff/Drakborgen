#include "Engine.h"

#include "Animation.h"
#include "Board.h"
#include "TextBox.h"
#include "Button.h"
#include "System.h"

Engine::Engine(AnimationManager& animations, Board& board)
#if BLESS_THIS_MESS
	: window(sf::VideoMode::getDesktopMode(), "Drakborgen")
	//: window(sf::VideoMode(1024, 768), "Drakborgen")
#else
	: window(sf::VideoMode::getDesktopMode(), "Drakborgen", sf::Style::Fullscreen)
#endif // BLESS_THIS_MESS
	, animations(animations)
	, board(board)
{
	window.setVerticalSyncEnabled(true);
}

void Engine::run()
{
	sf::Clock clock;
	float lastTimestamp = 0.0f;
	//tiles.shuffle();
	//bool toogle = false;
	/*for (int row = 0; row < board.rowCount; ++row)
	{
		for (int column = 0; column < board.columnCount; ++column)
		{
			if (toogle && !tiles.isEmpty() && !board.hasTile({ row, column }))
			{
				board.placeTile(tiles.pullNextItem(), { row, column });
			}
			toogle = !toogle;
		}
	}*/

	setState(State::SelectNetRole);

	int fpsCount = 0;
	sf::Clock fpsTimer;

	while (window.isOpen())
	{
		// update the fps text every second
		if (fpsTimer.getElapsedTime().asSeconds() >= 1)
		{
			//std::cout << fpsCount << " fps" << std::endl;
			fpsTimer.restart();
			fpsCount = 0;
		}
		++fpsCount;

		sf::Time time = clock.getElapsedTime();
		float timestamp = time.asSeconds();
		float timeDelta = timestamp - lastTimestamp;
		lastTimestamp = timestamp;

		if (!network.isConnected())
		{
			//setState(State::AwaitingConnection);
		}

		invokeEventHandler(onTickTable);

		animations.update(timestamp, timeDelta);

		board.update(timestamp, timeDelta);

		processSystemEvents();

		window.clear();

		window.draw(board);

		for (const Button* button : buttons)
		{
			window.draw(*button);
		}
		// TODO: add these to be rendered
		//if (preGameSetup)
		//{
		//	for (Hero& hero : idleHeroes)
		//	{
		//		if (Card* card = hero.getStatsCard())
		//		{
		//			window.draw(*card);
		//		}
		//	}
		//}
		//if (activePlayerIndex >= 0 && players[activePlayerIndex].hero.getStatsCard())
		//{
		//	window.draw(*players[activePlayerIndex].hero.getStatsCard());
		//}

		window.display();
	}
}

sf::Vector2u Engine::getWindowSize()
{
	return window.getSize();
}

Network& Engine::getNetwork()
{
	return network;
}

void Engine::createButton(const std::string& message, sf::Vector2f size, sf::Vector2f position, int textSize, std::function<void()> callback)
{
	textBoxes.emplace_back(std::make_unique<TextBox>(message, size, position, textSize));
	internalButtons.emplace_back(std::make_unique<Button>(textBoxes.back().get(), callback));
	buttons.insert(internalButtons.back().get());
}

void Engine::clearButtons()
{
	buttons.clear();
	internalButtons.clear();
	textBoxes.clear();
}

void Engine::setState(State newState)
{
	if (newState != state)
	{
		invokeEventHandler(onEndTable);
		state = newState;
		invokeEventHandler(onBeginTable);
	}
}

void Engine::animate(sf::Transformable& transformable, sf::Vector2f target, float time, std::function<void()> doneCallback)
{
	animations.add(transformable, target, time, doneCallback);
}

void Engine::addButton(Button* button)
{
	buttons.insert(button);
}

void Engine::removeButton(Button* button)
{
	buttons.erase(button);
}

sf::Vector2f Engine::getMouseBoardPosition() const
{
	sf::Vector2f boardPosition = board.getPosition();
	sf::Vector2f mouseBoardPosition = sf::Vector2f(buttonReleasedMousePosition.x - boardPosition.x, buttonReleasedMousePosition.y - boardPosition.y);
	return mouseBoardPosition;
}

sf::Vector2f Engine::correctBoardPosition(sf::Vector2f boardPosition)
{
	if (xCenteredBoard)
	{
		boardPosition.x = float(window.getSize().x / 2) - board.getSize().x / 2.0f;
	}
	else if (boardPosition.x > 0.0f)
	{
		boardPosition.x = 0.0f;
	}
	else if (boardPosition.x + board.getSize().x < window.getSize().x)
	{
		boardPosition.x = window.getSize().x - board.getSize().x;
	}
	if (xCenteredBoard)
	{
		boardPosition.y = float(window.getSize().y / 2) - board.getSize().y / 2.0f;
	}
	else if (boardPosition.y > 0.0f)
	{
		boardPosition.y = 0.0f;
	}
	else if (boardPosition.y + board.getSize().y < window.getSize().y)
	{
		boardPosition.y = window.getSize().y - board.getSize().y;
	}
	return boardPosition;
}

void Engine::processSystemEvents()
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
				buttonPressedMousePosition.x = float(event.mouseButton.x);
				buttonPressedMousePosition.y = float(event.mouseButton.y);
				buttonPressedBoardPosition = board.getPosition();
				capturedButton = getHitButton(buttonPressedMousePosition);
			}
		}
		else if (event.type == sf::Event::EventType::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Left && leftMouseButtonDown)
			{
				leftMouseButtonDown = false;
				buttonReleasedMousePosition.x = float(event.mouseButton.x);
				buttonReleasedMousePosition.y = float(event.mouseButton.y);
				Button* mouseOverButton = getHitButton(buttonReleasedMousePosition);
				if (mouseOverButton)
				{
					mouseOverButton->click();
				}
				else if (capturedButton == nullptr)
				{
					invokeEventHandler(onLeftMouseClickTable);
				}
			}
		}
		else if (event.type == sf::Event::EventType::MouseMoved)
		{
			if (leftMouseButtonDown && capturedButton == nullptr)
			{
				if (!xCenteredBoard || !yCenteredBoard)
				{
					animations.remove(board);
					sf::Vector2f mouseMovementSincePressed(float(event.mouseMove.x - buttonPressedMousePosition.x), float(event.mouseMove.y - buttonPressedMousePosition.y));
					sf::Vector2f newBoardPosition = buttonPressedBoardPosition + mouseMovementSincePressed;
					sf::Vector2f correctedBoardPosition = correctBoardPosition(newBoardPosition);
					board.setPosition(correctedBoardPosition);
					if (newBoardPosition.x != correctedBoardPosition.x)
					{
						buttonPressedMousePosition.x = float(event.mouseMove.x);
						buttonPressedBoardPosition.x = correctedBoardPosition.x;
					}
					if (newBoardPosition.y != correctedBoardPosition.y)
					{
						buttonPressedMousePosition.y = float(event.mouseMove.y);
						buttonPressedBoardPosition.y = correctedBoardPosition.y;
					}
				}
			}
		}
	}
}

void Engine::invokeEventHandler(const EventTable& eventTable)
{
	if (std::function<void()> function = eventTable[int(state)])
	{
		function();
	}
	else
	{
		THROW;
	}
}

Button* Engine::getHitButton(sf::Vector2f point) const
{
	for (Button* button : buttons)
	{
		if (button->hitTest(point))
		{
			return button;
		}
	}
	return nullptr;
}
