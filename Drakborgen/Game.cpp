#include "Game.h"

#include <iostream>
#include <memory>

#include "Tile.h"

Game::Game()
#ifdef _DEBUG
	: window(sf::VideoMode(1920, 1200), "Drakborgen")
#else
	: window(sf::VideoMode::getDesktopMode(), "Drakborgen", sf::Style::Fullscreen)
#endif // DEBUG
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

Game::State Game::StateLogic::execute(Game& game)
{
	State state = thisState;
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
				game.buttonPressedMousePosition.x = float(event.mouseButton.x);
				game.buttonPressedMousePosition.y = float(event.mouseButton.y);
				game.buttonPressedBoardPosition = game.board.getPosition();
				game.capturedItemIndex = game.getMouseOverItemIndex(game.buttonPressedMousePosition);

			}
		}
		if (event.type == sf::Event::EventType::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Button::Left && game.leftMouseButtonDown)
			{
				game.leftMouseButtonDown = false;
				game.buttonReleasedMousePosition.x = float(event.mouseButton.x);
				game.buttonReleasedMousePosition.y = float(event.mouseButton.y);
				int mouseOverItemIndex = game.getMouseOverItemIndex(game.buttonPressedMousePosition);
				if (mouseOverItemIndex == game.capturedItemIndex)
				{
					state = onLeftMouseClick(state, game);
				}
			}
		}
		if (event.type == sf::Event::EventType::MouseMoved)
		{
			if (game.leftMouseButtonDown && game.capturedItemIndex == -1)
			{
				if (!game.xCenteredBoard || !game.yCenteredBoard)
				{
					sf::Vector2f mouseMovementSincePressed(float(event.mouseMove.x - game.buttonPressedMousePosition.x), float(event.mouseMove.y - game.buttonPressedMousePosition.y));
					sf::Vector2f newBoardPosition = game.buttonPressedBoardPosition + mouseMovementSincePressed;
					if (game.xCenteredBoard)
					{
						newBoardPosition.x = float(game.window.getSize().x / 2) - game.board.getSize().x / 2.0f;
					}
					else if (newBoardPosition.x > 0.0f)
					{
						newBoardPosition.x = 0.0f;
						game.buttonPressedMousePosition.x = float(event.mouseMove.x);
						game.buttonPressedBoardPosition.x = game.board.getPosition().x;
					}
					else if (newBoardPosition.x + game.board.getSize().x < game.window.getSize().x)
					{
						newBoardPosition.x = game.window.getSize().x - game.board.getSize().x;
						game.buttonPressedMousePosition.x = float(event.mouseMove.x);
						game.buttonPressedBoardPosition.x = game.board.getPosition().x;
					}
					if (game.xCenteredBoard)
					{
						newBoardPosition.y = float(game.window.getSize().y / 2) - game.board.getSize().y / 2.0f;
					}
					else if (newBoardPosition.y > 0.0f)
					{
						newBoardPosition.y = 0.0f;
						game.buttonPressedMousePosition.y = float(event.mouseMove.y);
						game.buttonPressedBoardPosition.y = game.board.getPosition().y;
					}
					else if (newBoardPosition.y + game.board.getSize().y < game.window.getSize().y)
					{
						newBoardPosition.y = game.window.getSize().y - game.board.getSize().y;
						game.buttonPressedMousePosition.y = float(event.mouseMove.y);
						game.buttonPressedBoardPosition.y = game.board.getPosition().y;
					}
					game.board.setPosition(newBoardPosition);
				}
			}
		}
	}
	return state;
}

Game::State Game::PickHeroState::onLeftMouseClick(State state, Game& game)
{
	std::cout << "pick hero ";
	int heroIndex = game.getMouseOverItemIndex(game.buttonPressedMousePosition);
	if(heroIndex >= 0)
	{
		if (heroIndex < game.heroes.size())
		{
			game.createPlayer(heroIndex);
			game.board.showClickSites(true);
			std::cout << " -> pick start tower" << std::endl;
			return State::PickStartTower;
		}
		else
		{
			game.buttons.clear();
			for (int i = int(game.heroes.size()) - 1; i >= 0; --i)
			{
				std::unique_ptr<Card> card = game.cardDisplay.pullCard(i);
				std::cout << game.heroes[i].getId() << " " << card->imagePath << std::endl;
				game.heroes[i].placeStatsCard(std::move(card));
			}
			game.startNewGame();
			std::cout << " -> pick player move" << std::endl;
			return State::PlayerMove;
		}
	}
	std::cout << " -> pick hero" << std::endl;
	return state;
}

Game::State Game::PickStartTowerState::onLeftMouseClick(State state, Game& game)
{
	std::cout << "pick start tower ";
	sf::Vector2f mouseBoardPosition = game.getMouseBoardPosition();
	if (game.board.testClickSites(mouseBoardPosition))
	{
		game.board.showClickSites(false);
		Board::Site site = game.board.getSite(mouseBoardPosition);
		game.board.removeClickSite(site);
		game.placeNewPlayer(site);
		if (game.heroes.empty())
		{
			game.startNewGame();
			std::cout << " -> pick player move" << std::endl;
			return State::PlayerMove;
		}
		else
		{
			sf::Vector2f buttonSize(500.0, 200.0);
			sf::Vector2f buttonPosition(game.window.getSize().x * 3.0f / 4.0f - buttonSize.x / 2.0f, game.window.getSize().y * 3.0f / 4.0f - buttonSize.y / 2.0f);
			game.buttons.push_back(std::make_unique<Button>("Let's rock!", buttonSize, buttonPosition, []() { std::cout << "Click!" << std::endl; }));
			std::function<void()> cardsDisplayedCallback =
				[&game]()
			{
				std::cout << "Hej" << std::endl;
			};
			game.displayCards(game.getHeroCards(), cardsDisplayedCallback);
			std::cout << " -> pick hero" << std::endl;
			return State::PickHero;
		}
	}
	std::cout << " -> pick start tower" << std::endl;
	return state;
}

Game::State Game::PlayerMoveState::onLeftMouseClick(State state, Game& game)
{
	std::cout << "player move ";
	sf::Vector2f mouseBoardPosition = game.getMouseBoardPosition();
	if (game.board.testClickSites(mouseBoardPosition))
	{
		game.board.clearClickSites();
		Board::Site site = game.board.getSite(mouseBoardPosition);
		game.movePlayer(game.activePlayer, site);
		//++game.activePlayer;
		game.startPlayerRound();
	}
	std::cout << " -> player move" << std::endl;
	return state;
}

void Game::run()
{
	sf::Clock clock;
	float lastTimestamp = 0.0f;
	tiles.shuffle();
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

	heroes.reserve(4);
	heroes.push_back(Hero("rohan", "Riddar Rohan", 15));
	heroes.push_back(Hero("sigeir", "Sigeir Skarpyxe", 14));
	heroes.push_back(Hero("aelfric", "Aelfric Brunkåpa", 10));
	heroes.push_back(Hero("bardhor", "Bardhor Bågman", 8));

	board.setGameStartClickSites();
	
	displayCards(getHeroCards(), []() { });

	State state = State::PickHero;

	while (window.isOpen())
	{
		state = stateLogicMap[state]->execute(*this);

		sf::Time time = clock.getElapsedTime();
		float timestamp = time.asSeconds();
		float timeDelta = timestamp - lastTimestamp;
		lastTimestamp = timestamp;

		animations.update(timestamp, timeDelta);

		board.update(timestamp, timeDelta);

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
			if (buttons[i]->hitTest(mousePosition, false))
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
	for (Hero& hero : heroes)
	{
		heroCards.push_back(hero.pullStatsCard());
	}
	return heroCards;
}

void Game::displayCards(std::vector<std::unique_ptr<Card>>&& cards, std::function<void()> cardsDisplayedCallback)
{
	std::vector<sf::Vector2f> layout = cardDisplay.getLayout(cards);
	for (int i = 0; i < cards.size(); ++i)
	{
		cards[i]->setPosition({ 0.0f, 0.0f });
		animations.add(*cards[i], layout[i], float(i + 1) / float(cards.size()), i == 0 ? cardsDisplayedCallback : []() { });
		cardDisplay.pushCard(std::move(cards[i]));
	}
}

void Game::createPlayer(int heroIndex)
{
	heroes[heroIndex].placeStatsCard(cardDisplay.pullCard(heroIndex));
	players.push_back({ std::move(heroes[heroIndex]), Board::invalidSite, heroes[heroIndex].getMaxLife() });
	heroes.erase(heroes.begin() + heroIndex);
	for (int i = int(heroes.size()) - 1; i >= 0; --i)
	{
		heroes[i].placeStatsCard(cardDisplay.pullCard(i));
	}
}

void Game::placeNewPlayer(Board::Site site)
{
	int index = int(players.size()) - 1;
	Player& player = players[index];
	player.boardSite = site;
	board.addPlayer("Media/hjaltekort/" + player.hero.getId() + "gubbe.png", index);
	board.setPlayerSite(index, site);
}

void Game::movePlayer(int index, Board::Site site)
{
	Player& player = players[index];
	player.boardSite = site;
	board.setPlayerSite(index, site);
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
