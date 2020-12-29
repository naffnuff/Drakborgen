#include "Game.h"

#include <iostream>
#include <sstream>
#include <memory>

#include "Tile.h"

Game::Game()
#ifndef _DEBUG
	: window(sf::VideoMode(1920, 1200), "Drakborgen")
#else
	: window(sf::VideoMode::getDesktopMode(), "Drakborgen", sf::Style::Fullscreen)
#endif // DEBUG
	, board(animations)
	, cardDisplay(window)
	, tiles(random)
{
	sf::Vector2f boardSize = board.getSize();
	sf::Vector2u windowSize = window.getSize();
	xCenteredBoard = boardSize.x < windowSize.x;
	yCenteredBoard = boardSize.y < windowSize.y;
	sf::Vector2f boardPosition;
	if (xCenteredBoard)
	{
		boardPosition.x = float(windowSize.x / 2) - boardSize.x / 2.0f;
	}
	if (yCenteredBoard)
	{
		boardPosition.y = float(windowSize.y / 2) - boardSize.y / 2.0f;
	}
	board.setPosition(boardPosition);

	createStateLogicMap();
}

void Game::run()
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

	idleHeroes.reserve(4);
	idleHeroes.push_back(Hero("rohan", L"Riddar Rohan", 15));
	idleHeroes.push_back(Hero("sigeir", L"Sigeir Skarpyxe", 14));
	idleHeroes.push_back(Hero("aelfric", L"Aelfric Brunkåpa", 10));
	idleHeroes.push_back(Hero("bardhor", L"Bardhor Bågman", 8));

	board.setGameStartMoveSites();

	displayCards(getHeroCards(), []() {});

	State state = State::PickHero;

	while (window.isOpen())
	{
		sf::Time time = clock.getElapsedTime();
		float timestamp = time.asSeconds();
		float timeDelta = timestamp - lastTimestamp;
		lastTimestamp = timestamp;

		animations.update(timestamp, timeDelta);

		board.update(timestamp, timeDelta);

		state = processEvents(state);

		window.clear();

		window.draw(board);
		window.draw(cardDisplay);
		for (const std::unique_ptr<Button>& button : buttons)
		{
			window.draw(*button);
		}

		window.display();
	}
}

Game::State Game::processEvents(State state)
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
				capturedItemIndex = getMouseOverItemIndex(buttonPressedMousePosition);
			}
		}
		else if (event.type == sf::Event::EventType::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Left && leftMouseButtonDown)
			{
				leftMouseButtonDown = false;
				buttonReleasedMousePosition.x = float(event.mouseButton.x);
				buttonReleasedMousePosition.y = float(event.mouseButton.y);
				int mouseOverItemIndex = getMouseOverItemIndex(buttonReleasedMousePosition);
				if (mouseOverItemIndex == capturedItemIndex)
				{
					if (stateLogicMap.find(state) == stateLogicMap.end())
					{
						THROW;
					}
					state = stateLogicMap[state]();
				}
			}
		}
		else if (event.type == sf::Event::EventType::MouseMoved)
		{
			if (leftMouseButtonDown && capturedItemIndex == -1)
			{
				if (!xCenteredBoard || !yCenteredBoard)
				{
					sf::Vector2f mouseMovementSincePressed(float(event.mouseMove.x - buttonPressedMousePosition.x), float(event.mouseMove.y - buttonPressedMousePosition.y));
					sf::Vector2f newBoardPosition = buttonPressedBoardPosition + mouseMovementSincePressed;
					if (xCenteredBoard)
					{
						newBoardPosition.x = float(window.getSize().x / 2) - board.getSize().x / 2.0f;
					}
					else if (newBoardPosition.x > 0.0f)
					{
						newBoardPosition.x = 0.0f;
						buttonPressedMousePosition.x = float(event.mouseMove.x);
						buttonPressedBoardPosition.x = board.getPosition().x;
					}
					else if (newBoardPosition.x + board.getSize().x < window.getSize().x)
					{
						newBoardPosition.x = window.getSize().x - board.getSize().x;
						buttonPressedMousePosition.x = float(event.mouseMove.x);
						buttonPressedBoardPosition.x = board.getPosition().x;
					}
					if (xCenteredBoard)
					{
						newBoardPosition.y = float(window.getSize().y / 2) - board.getSize().y / 2.0f;
					}
					else if (newBoardPosition.y > 0.0f)
					{
						newBoardPosition.y = 0.0f;
						buttonPressedMousePosition.y = float(event.mouseMove.y);
						buttonPressedBoardPosition.y = board.getPosition().y;
					}
					else if (newBoardPosition.y + board.getSize().y < window.getSize().y)
					{
						newBoardPosition.y = window.getSize().y - board.getSize().y;
						buttonPressedMousePosition.y = float(event.mouseMove.y);
						buttonPressedBoardPosition.y = board.getPosition().y;
					}
					board.setPosition(newBoardPosition);
				}
			}
		}
	}
	return state;
}

template<>
Game::State Game::onLeftMouseClick<Game::State::PickHero>(State state)
{
	std::cout << "pick hero ";
	if (capturedItemIndex >= 0)
	{
		buttons.clear();
		if (capturedItemIndex < cardDisplay.cardCount())
		{
			createPlayer(capturedItemIndex);
			board.showMoveSites(true);
			std::cout << " -> pick start tower" << std::endl;
			return State::PickStartTower;
		}
		else
		{
			for (int i = int(idleHeroes.size()) - 1; i >= 0; --i)
			{
				std::unique_ptr<Card> card = cardDisplay.pullCard(i);
				std::cout << idleHeroes[i].getId() << " " << card->imagePath << std::endl;
				idleHeroes[i].placeStatsCard(std::move(card));
			}
			startNewGame();
			std::cout << " -> pick player move" << std::endl;
			return State::PlayerMove;
		}
	}
	std::cout << " -> pick hero" << std::endl;
	return state;
}

template<>
Game::State Game::onLeftMouseClick<Game::State::PickStartTower>(State state)
{
	std::cout << "pick start tower ";
	sf::Vector2f mouseBoardPosition = getMouseBoardPosition();
	if (board.testMoveSites(mouseBoardPosition))
	{
		board.showMoveSites(false);
		Board::Site site = board.getSite(mouseBoardPosition);
		board.removeMoveSite(site);
		placeNewPlayer(site, [this]() { onNewPlayerPlaced(); });
		if (idleHeroes.empty())
		{
			std::cout << " -> pick player move" << std::endl;
			return State::PlayerMove;
		}
		else
		{
			std::cout << " -> pick hero" << std::endl;
			return State::PickHero;
		}
	}
	std::cout << " -> pick start tower" << std::endl;
	return state;
}

template<>
Game::State Game::onLeftMouseClick<Game::State::PlayerMove>(State state)
{
	std::cout << "player move ";
	if (capturedItemIndex >= cardDisplay.cardCount())
	{
		//board.showMoveSites(false);
		std::unique_ptr<Card> card = players[activePlayerIndex].hero.pullStatsCard();
		displayCard(std::move(card), []() {});
		std::cout << "-> view stats card" << std::endl;
		return State::ViewStatsCard;
	}
	else
	{
		sf::Vector2f mouseBoardPosition = getMouseBoardPosition();
		if (board.testMoveSites(mouseBoardPosition))
		{
			Board::MoveSite moveSite = board.getMoveSite(mouseBoardPosition);
			if (board.hasTile(moveSite.site))
			{
				movePlayer(activePlayerIndex, moveSite, [this]() { onPlayerMoved(); });
			}
			else
			{
				std::unique_ptr<Tile> tile = tiles.pullNextItem();

				board.placeTile(std::move(tile), moveSite);
				movePlayer(activePlayerIndex, moveSite, [this]() { onPlayerMoved(); });
			}
			board.clearMoveSites();
		}
	}
	std::cout << " -> player move" << std::endl;
	return state;
}

template<>
Game::State Game::onLeftMouseClick<Game::State::ViewStatsCard>(State state)
{
	std::cout << "view stats card ";
	if (capturedItemIndex > -1)
	{
		players[activePlayerIndex].hero.placeStatsCard(cardDisplay.pullCard());
		//board.showMoveSites(true);
		std::cout << " -> pick player move" << std::endl;
		return State::PlayerMove;
	}
	std::cout << "-> view stats card" << std::endl;
	return state;
}

void Game::placeAtOrigin(std::unique_ptr<Card>& card) const
{
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2f cardSize = card->getGlobalBounds().getSize();
	card->setPosition({ -cardSize.x, float(windowSize.y) });
}

sf::Vector2f Game::getMouseBoardPosition() const
{
	sf::Vector2f boardPosition = board.getPosition();
	sf::Vector2f mouseBoardPosition = sf::Vector2f(buttonReleasedMousePosition.x - boardPosition.x, buttonReleasedMousePosition.y - boardPosition.y);
	return mouseBoardPosition;
}

int Game::getMouseOverItemIndex(sf::Vector2f mousePosition) const
{
	int index = cardDisplay.hitTest(mousePosition);
	if (index == -1)
	{
		for (int i = 0; i < buttons.size(); ++i)
		{
			if (buttons[i]->hitTest(mousePosition))
			{
				index = cardDisplay.cardCount() + i;
				break;
			}
		}
	}
	return index;
}

std::vector<std::unique_ptr<Card>> Game::getHeroCards()
{
	std::vector<std::unique_ptr<Card>> heroCards;
	for (Hero& hero : idleHeroes)
	{
		heroCards.push_back(hero.pullStatsCard());
	}
	return heroCards;
}

void Game::displayCard(std::unique_ptr<Card>&& card, std::function<void()> cardDisplayedCallback)
{
	placeAtOrigin(card);
	sf::Vector2f cardSize = card->getGlobalBounds().getSize();
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2f animationTarget(windowSize.x / 2.0f - cardSize.x / 2.0f, windowSize.y / 2.0f - cardSize.y / 2.0f);
	animations.add(*card, animationTarget, 0.5f, cardDisplayedCallback);
	cardDisplay.pushCard(std::move(card));
}

void Game::displayCards(std::vector<std::unique_ptr<Card>>&& cards, std::function<void()> cardsDisplayedCallback)
{
	std::vector<sf::Vector2f> layout = cardDisplay.getLayout(cards);
	for (int i = 0; i < cards.size(); ++i)
	{
		placeAtOrigin(cards[i]);
		animations.add(*cards[i], layout[i], float(i + 1) / float(cards.size()), i == 0 ? cardsDisplayedCallback : []() { });
		cardDisplay.pushCard(std::move(cards[i]));
	}
}

void Game::createPlayer(int heroIndex)
{
	idleHeroes[heroIndex].placeStatsCard(cardDisplay.pullCard(heroIndex));
	players.push_back({ std::move(idleHeroes[heroIndex]), Board::invalidSite, idleHeroes[heroIndex].getMaxLife() });
	idleHeroes.erase(idleHeroes.begin() + heroIndex);
	for (int i = int(idleHeroes.size()) - 1; i >= 0; --i)
	{
		idleHeroes[i].placeStatsCard(cardDisplay.pullCard(i));
	}
}

void Game::placeNewPlayer(Board::Site site, std::function<void()> callback)
{
	int index = int(players.size()) - 1;
	Player& player = players[index];
	player.boardSite = site;
	player.avatarIndex = index;
	board.addPlayer("Media/hjaltekort/" + player.hero.getId() + "gubbe.png", index);
	board.setPlayerSite(index, { site, Direction::Invalid }, callback);
}

void Game::movePlayer(int index, Board::MoveSite moveSite, std::function<void()> callback)
{
	Player& player = players[index];
	player.boardSite = moveSite.site;
	board.setPlayerSite(player.avatarIndex, moveSite, callback);
}

void Game::onNewPlayerPlaced()
{
	if (idleHeroes.empty())
	{
		startNewGame();
	}
	else
	{
		sf::Vector2f buttonSize(500.0, 200.0);
		sf::Vector2f buttonPosition(window.getSize().x * 3.0f / 4.0f - buttonSize.x / 2.0f, window.getSize().y * 3.0f / 4.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>(L"Låt äventyret börja!", buttonSize, buttonPosition, 60));
		std::function<void()> cardsDisplayedCallback =
			[this]()
		{
			std::cout << "Hej" << std::endl;
		};
		displayCards(getHeroCards(), cardsDisplayedCallback);
	}
}

void Game::onPlayerMoved()
{
	//++game.activePlayer;
	startPlayerRound();
}

void Game::startNewGame()
{
	board.clearMoveSites();
	random.shuffle(players);
	startPlayerRound();
}

void Game::startPlayerRound()
{
	Player& player = players[activePlayerIndex];
	sf::Vector2f size(270.0f, 100.0f);
	sf::Vector2f position(0.0f, 370.0f);
	std::wstringstream ss;
	ss << player.hero.getName() << std::endl;
	ss << L"Kroppspoäng: " << player.life << " / " << player.hero.getMaxLife();
	buttons.emplace_back(std::make_unique<Button>(ss.str(), size, position, 30));
	board.setPlayerMoveSites(player.boardSite);
	board.showMoveSites(true);
}

#define STATE(state, event) stateLogicMap[State::state] = [this]() { return on##event##<Game::State::##state##>(State::state); }

void Game::createStateLogicMap()
{
	STATE(PickHero, LeftMouseClick);
	STATE(PickStartTower, LeftMouseClick);
	STATE(PlayerMove, LeftMouseClick);
	STATE(ViewStatsCard, LeftMouseClick);
}
