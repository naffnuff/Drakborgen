#include "Game.h"

#include <iostream>
#include <sstream>
#include <memory>

#include "Tile.h"

Game::Game()
#ifdef _DEBUG
	//: window(sf::VideoMode::getDesktopMode(), "Drakborgen")
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
		if (preGameSetup)
		{
			for (Hero& hero : idleHeroes)
			{
				if (hero.getStatsCard())
				{
					window.draw(*hero.getStatsCard());
				}
			}
		}
		if (activePlayerIndex >= 0 && players[activePlayerIndex].hero.getStatsCard())
		{
			window.draw(*players[activePlayerIndex].hero.getStatsCard());
		}
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
	return state;
}

sf::Vector2f Game::correctBoardPosition(sf::Vector2f boardPostion)
{
	if (xCenteredBoard)
	{
		boardPostion.x = float(window.getSize().x / 2) - board.getSize().x / 2.0f;
	}
	else if (boardPostion.x > 0.0f)
	{
		boardPostion.x = 0.0f;
	}
	else if (boardPostion.x + board.getSize().x < window.getSize().x)
	{
		boardPostion.x = window.getSize().x - board.getSize().x;
	}
	if (xCenteredBoard)
	{
		boardPostion.y = float(window.getSize().y / 2) - board.getSize().y / 2.0f;
	}
	else if (boardPostion.y > 0.0f)
	{
		boardPostion.y = 0.0f;
	}
	else if (boardPostion.y + board.getSize().y < window.getSize().y)
	{
		boardPostion.y = window.getSize().y - board.getSize().y;
	}
	return boardPostion;
}

template<>
Game::State Game::onLeftMouseClick<Game::State::PickHero>(State state)
{
	std::cout << "pick hero ";
	if (capturedItemIndex >= 0)
	{
		buttons.clear();
		if (capturedItemIndex < cardDisplay.cardCount()) // hero card clicked
		{
			panToNextFreeTower();
			createPlayer(capturedItemIndex);
			board.showMoveSites(true);			
			std::cout << " -> pick start tower" << std::endl;
			return State::PickStartTower;
		}
		else // begin-game button clicked
		{
			for (int i = int(idleHeroes.size()) - 1; i >= 0; --i)
			{
				std::unique_ptr<Card> card = cardDisplay.pullCard(i);
				std::function<void()> callback = []() { };
				if (i == idleHeroes.size() - 1)
				{
					callback = [this]() { preGameSetup = false; };
				}
				moveOffScreen(card, float(i + 1) / float(idleHeroes.size()), callback);
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
		if (!card)
		{
			THROW;
		}
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
		std::unique_ptr<Card> card = cardDisplay.pullCard();
		moveOffScreen(card, 0.5f, [this]() { placeAtOrigin(players[activePlayerIndex].hero.getStatsCard()); });
		players[activePlayerIndex].hero.placeStatsCard(std::move(card));
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

void Game::moveOffScreen(std::unique_ptr<Card>& card, float time, std::function<void()> callback)
{
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2f cardSize = card->getGlobalBounds().getSize();
	animations.add(*card, { float(windowSize.x), -cardSize.y }, time, callback);
}

void Game::moveToCenter(std::unique_ptr<Card>& card, std::function<void()> callback)
{
	sf::Vector2f cardSize = card->getGlobalBounds().getSize();
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2f animationTarget(windowSize.x / 2.0f - cardSize.x / 2.0f, windowSize.y / 2.0f - cardSize.y / 2.0f);
	animations.add(*card, animationTarget, 0.5f, callback);
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

void Game::displayCard(std::unique_ptr<Card>&& card, std::function<void()> callback)
{
	//placeAtOrigin(card);
	moveToCenter(card, callback);
	cardDisplay.pushCard(std::move(card));
}

void Game::displayCards(std::vector<std::unique_ptr<Card>>&& cards, std::function<void()> callback)
{
	std::vector<sf::Vector2f> layout = cardDisplay.getLayout(cards);
	for (int i = 0; i < cards.size(); ++i)
	{
		placeAtOrigin(cards[i]);
		animations.add(*cards[i], layout[i], float(i + 1) / float(cards.size()), i == cards.size() - 1 ? callback : []() { });
		cardDisplay.pushCard(std::move(cards[i]));
	}
}

void Game::panToNextFreeTower()
{
	sf::Vector2f boardTarget(0.0f, 0.0f);
	sf::Vector2u windowSize = window.getSize();
	if (players.size() == 0)
	{
		if (random.nextBool())
		{
			boardTarget.y = -float(windowSize.y);
		}
		if (random.nextBool())
		{
			boardTarget.x = -float(windowSize.x);
		}
	}
	else
	{
		Board::Site lastSite = players.back().boardSite;
		int oppositeRow = lastSite.row == 0 ? Board::rowCount - 1 : 0;
		int oppositeColumn = lastSite.column == 0 ? Board::columnCount - 1 : 0;
		std::vector<Board::Site> candidates = {
			{ oppositeRow, oppositeColumn },
			{ lastSite.row, oppositeColumn },
			{ oppositeRow, lastSite.column }
		};
		//random.shuffle(candidates);
		for (Board::Site candidate : candidates)
		{
			bool goodCandidate = true;
			for (int i = 0; i < int(players.size()) - 1; ++i)
			{
				if (players[i].boardSite == candidate)
				{
					goodCandidate = false;
					break;
				}
			}
			if (goodCandidate)
			{
				if (candidate.row > 0)
				{
					boardTarget.y = -float(windowSize.y);
				}
				if (candidate.column > 0)
				{
					boardTarget.x = -float(windowSize.x);
				}
				break;
			}
		}
	}
	animations.add(board, correctBoardPosition(boardTarget), 0.75f, []() { });
}

void Game::createPlayer(int heroIndex)
{
	std::unique_ptr<Card> pickedCard = cardDisplay.pullCard(heroIndex);
	moveToCenter(pickedCard, []() { });
	players.push_back({ std::move(idleHeroes[heroIndex]), Board::invalidSite, idleHeroes[heroIndex].getMaxLife() });
	++activePlayerIndex;
	idleHeroes.erase(idleHeroes.begin() + heroIndex);
	for (int i = int(idleHeroes.size()) - 1; i >= 0; --i)
	{
		std::unique_ptr<Card> card = cardDisplay.pullCard(i);
		moveOffScreen(card, float(i + 1) / float(idleHeroes.size()), []() { });
		idleHeroes[i].placeStatsCard(std::move(card));
	}
	cardDisplay.pushCard(std::move(pickedCard));
}

void Game::placeNewPlayer(Board::Site site, std::function<void()> callback)
{
	if (players.size() == 0)
	{
		THROW;
	}
	int index = int(players.size()) - 1;
	Player& player = players[index];
	player.boardSite = site;
	player.avatarIndex = index;
	if (cardDisplay.cardCount() != 1)
	{
		THROW;
	}
	std::unique_ptr<Card> card = cardDisplay.pullCard(0);
	std::function<void()> cardAnimationCallback = [this, &player, site, callback]() {
		board.addPlayer("Media/hjaltekort/" + player.hero.getId() + "gubbe.png", player.avatarIndex);
		board.setPlayerSite(player.avatarIndex, { site, Direction::Invalid }, callback);
		placeAtOrigin(player.hero.getStatsCard());
	};
	sf::Vector2f target;
	sf::Vector2f cardSize = card->getGlobalBounds().getSize();
	sf::Vector2u windowSize = window.getSize();
	if (site.column == 0)
	{
		target.x = -cardSize.x;
	}
	else if (site.column == board.columnCount - 1)
	{
		target.x = float(windowSize.x);
	}
	if (site.row == 0)
	{
		target.y = -cardSize.y;
	}
	else if (site.row == board.rowCount - 1)
	{
		target.y = float(windowSize.y);
	}
	animations.add(*card, target, 0.5f, cardAnimationCallback);
	player.hero.placeStatsCard(std::move(card));
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
		};
		displayCards(getHeroCards(), cardsDisplayedCallback);
	}
}

void Game::onPlayerMoved()
{
	startPlayerRound();
}

void Game::startNewGame()
{
	board.clearMoveSites();
	random.shuffle(players);
	activePlayerIndex = 0;
	startPlayerRound();
}

void Game::startPlayerRound()
{
	activePlayerIndex = (activePlayerIndex + 1) % int(players.size());
	Player& player = players[activePlayerIndex];
	sf::Vector2f size(270.0f, 100.0f);
	sf::Vector2f position(0.0f, 370.0f);
	std::wstringstream ss;
	ss << player.hero.getName() << std::endl;
	ss << L"Kroppspoäng: " << player.life << " / " << player.hero.getMaxLife();
	buttons.emplace_back(std::make_unique<Button>(ss.str(), size, position, 30));
	board.setPlayerMoveSites(player.boardSite);
	board.showMoveSites(true);
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2f avatarCenter = board.getAvatarCenter(player.avatarIndex);
	animations.add(board, correctBoardPosition({ windowSize.x / 2.0f - avatarCenter.x, windowSize.y / 2.0f - avatarCenter.y }), 0.75f, []() {});
}

#define STATE(state, event) stateLogicMap[State::state] = [this]() { return on##event##<Game::State::##state##>(State::state); }

void Game::createStateLogicMap()
{
	STATE(PickHero, LeftMouseClick);
	STATE(PickStartTower, LeftMouseClick);
	STATE(PlayerMove, LeftMouseClick);
	STATE(ViewStatsCard, LeftMouseClick);
}
