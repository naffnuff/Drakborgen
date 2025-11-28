#include "Game.h"

#include <iostream>
#include <sstream>
#include <memory>

#include "Tile.h"
#include "Setup.h"

Game::Game()
#if BLESS_THIS_MESS
	: window(sf::VideoMode::getDesktopMode(), "Drakborgen")
	//: window(sf::VideoMode(1024, 768), "Drakborgen")
#else
	: window(sf::VideoMode::getDesktopMode(), "Drakborgen", sf::Style::Fullscreen)
#endif // BLESS_THIS_MESS
	, board(animations)
	, cardDisplay(window)
	, tiles(random)
{
	window.setVerticalSyncEnabled(true);

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

void Game::processSystemEvents()
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
					invokeEventHandler(onLeftMouseClickTable);
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
}

void Game::invokeEventHandler(const EventTable& eventTable)
{
	if (std::function<void(Game&)> function = eventTable[int(state)])
	{
		function(*this);
	}
	else
	{
		THROW;
	}
}

void Game::setState(State newState)
{
	if (newState != state)
	{
		invokeEventHandler(onEndTable);
		state = newState;
		invokeEventHandler(onBeginTable);
	}
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
void Game::onBegin<State::SelectNetRole>()
{
	sf::Vector2f buttonSize(600.0f, 160.0f);
	{
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 1.0f / 4.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Snåla", buttonSize, buttonPosition, 60));
	}
	{
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 2.0f / 4.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Håll gästabud", buttonSize, buttonPosition, 60));
	}
	{
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 3.0f / 4.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Snylta", buttonSize, buttonPosition, 60));
	}
}

template<>
void Game::onLeftMouseClick<State::SelectNetRole>()
{
	if (capturedItemIndex == 0)
	{
		setState(State::SetupGame);
	}
	else if (capturedItemIndex == 1)
	{
		setState(State::SetupServer);
	}
	else if (capturedItemIndex == 2)
	{
		setState(State::SetupClient);
	}
}

template<>
void Game::onEnd<State::SelectNetRole>()
{
	buttons.clear();
}

// Server setup

template<>
void Game::onBegin<State::SetupServer>()
{
	sf::Vector2f buttonSize(600.0f, 160.0f);
	{
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 1.0f / 4.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Bjud in en gäst", buttonSize, buttonPosition, 60));
	}
	{
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 2.0f / 4.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Bjud in två gäster", buttonSize, buttonPosition, 60));
	}
	{
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 3.0f / 4.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Bjud in tre gäster", buttonSize, buttonPosition, 60));
	}
}

template<>
void Game::onEnd<State::SetupServer>()
{
	buttons.clear();
}

template<>
void Game::onLeftMouseClick<State::SetupServer>()
{
	if (capturedItemIndex > -1)
	{
		network.startServer(capturedItemIndex + 1, random);
		setState(State::NoState);
	}
}

// Client setup

template<>
void Game::onBegin<State::SetupClient>()
{
	{
		// Needs to be at index 0
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 5.0f / 6.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Låt färden gå!", buttonSize, buttonPosition, 60));
	}
	{
		// Needs to be at index 1
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 2.0f / 5.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Rasmus", buttonSize, buttonPosition, 60));
	}
	{
		// Needs to be at index 2
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 4.0f / 5.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("192.168.1.121", buttonSize, buttonPosition, 60));
	}
	{
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 1.0f / 8.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Om vem skall legenden vittna?", buttonSize, buttonPosition, 60));
	}
	{
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 2.0f / 8.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Varthän går färden?", buttonSize, buttonPosition, 60));
	}
}

template<>
void Game::onLeftMouseClick<State::SetupClient>()
{
	if (capturedItemIndex == 0)
	{
		network.startClient(/*buttons[1]->getText(), */buttons[2]->getText());
		setState(State::NoState);
	}
}

template<>
void Game::onEnd<State::SetupClient>()
{
	buttons.clear();
}

template<>
void Game::onBegin<State::AwaitingConnection>()
{
	sf::Vector2f buttonSize(600.0f, 160.0f);
	sf::Vector2f buttonPosition(window.getSize().x / 2.0f - buttonSize.x / 2.0f, window.getSize().y * 2.0f / 4.0f - buttonSize.y / 2.0f);
	buttons.push_back(std::make_unique<Button>("Sällskapet samlas...", buttonSize, buttonPosition, 60));
}

template<>
void Game::onTick<State::AwaitingConnection>()
{
	if (network.isConnected())
	{
		setState(State::NoState);
	}
}

template<>
void Game::onEnd<State::AwaitingConnection>()
{
	buttons.clear();
}

template<>
void Game::onBegin<State::SetupGame>()
{
	if (idleHeroes.size() == 0)
	{
		idleHeroes.reserve(4);
		idleHeroes.push_back(Hero("rohan", "Riddar Rohan", 17));
		idleHeroes.push_back(Hero("sigeir", "Sigeir Skarpyxe", 16));
		idleHeroes.push_back(Hero("aelfric", "Aelfric Brunkåpa", 15));
		idleHeroes.push_back(Hero("bardhor", "Bardhor Bågman", 11));
	}

	board.setGameStartMoveSites();

	displayCards(getHeroCards(), []() {});

	setState(State::PickHero);
}

template<>
void Game::onLeftMouseClick<State::PickHero>()
{
	if (capturedItemIndex >= 0)
	{
		buttons.clear();
		if (capturedItemIndex < cardDisplay.cardCount()) // hero card clicked
		{
			setState(State::PickStartTower);
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
			setState(State::PlayerMove);
		}
	}
}

template<>
void Game::onBegin<State::PickStartTower>()
{
	panToNextFreeTower();
	createPlayer(capturedItemIndex);
	board.showMoveSites(true);
}

template<>
void Game::onLeftMouseClick<State::PickStartTower>()
{
	sf::Vector2f mouseBoardPosition = getMouseBoardPosition();
	if (board.testMoveSites(mouseBoardPosition))
	{
		board.showMoveSites(false);
		Board::Site site = board.getSite(mouseBoardPosition);
		board.removeMoveSite(site);
		placeNewPlayer(site, [this]() { onNewPlayerPlaced(); });
		if (idleHeroes.empty())
		{
			setState(State::PlayerMove);
		}
		else
		{
			setState(State::PickHero);
		}
	}
}

template<>
void Game::onLeftMouseClick<State::PlayerMove>()
{
	if (capturedItemIndex >= cardDisplay.cardCount())
	{
		setState(State::ViewStatsCard);
	}
	else
	{
		sf::Vector2f mouseBoardPosition = getMouseBoardPosition();
		if (board.testMoveSites(mouseBoardPosition))
		{
			Board::MoveSite moveSite = board.getMoveSite(mouseBoardPosition);
			if (!board.hasTile(moveSite.site))
			{
				std::unique_ptr<Tile> tile = tiles.pullNextItem();
				board.placeTile(std::move(tile), moveSite);
			}
			Tile& tile = *board.getTile(moveSite.site);
			movePlayer(activePlayerIndex, moveSite, [this, &tile]() { onPlayerMoved(tile); });
			board.clearMoveSites();
			// setting new state async
		}
	}
}

template<>
void Game::onBegin<State::ViewStatsCard>()
{
	//board.showMoveSites(false);
	std::unique_ptr<Card> card = players[activePlayerIndex].hero.pullStatsCard();
	if (!card)
	{
		THROW;
	}
	displayCard(std::move(card), []() {});
}

template<>
void Game::onLeftMouseClick<State::ViewStatsCard>()
{
	if (capturedItemIndex > -1)
	{
		setState(State::PlayerMove);
	}
}

template<>
void Game::onEnd<State::ViewStatsCard>()
{
	std::unique_ptr<Card> card = cardDisplay.pullCard();
	moveOffScreen(card, 0.5f, [this]() { placeAtOrigin(players[activePlayerIndex].hero.getStatsCard()); });
	players[activePlayerIndex].hero.placeStatsCard(std::move(card));
	//board.showMoveSites(true);
}

template<>
void Game::onBegin<State::TurnContinue>()
{
	startPlayerRound();
	setState(State::PlayerMove);
}

template<>
void Game::onBegin<State::TurnEnd>()
{
	activePlayerIndex = (activePlayerIndex + 1) % int(players.size());
	onBegin<State::TurnContinue>();
}

// Helpers

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
		Board::Site lastSite = players.back().boardSite.site;
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
				if (players[i].boardSite.site == candidate)
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
	players.push_back({ std::move(idleHeroes[heroIndex]), Board::invalidMoveSite, idleHeroes[heroIndex].getMaxLife() });
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
	player.boardSite = { site };
	player.avatarIndex = index;
	if (cardDisplay.cardCount() != 1)
	{
		THROW;
	}
	std::unique_ptr<Card> card = cardDisplay.pullCard(0);
	std::function<void()> cardAnimationCallback = [this, &player, site, callback]() {
		board.addPlayer(Setup::getMediaPath() + "hjaltekort/" + player.hero.getId() + "gubbe.png", player.avatarIndex);
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
	else if (site.column == Board::columnCount - 1)
	{
		target.x = float(windowSize.x);
	}
	if (site.row == 0)
	{
		target.y = -cardSize.y;
	}
	else if (site.row == Board::rowCount - 1)
	{
		target.y = float(windowSize.y);
	}
	animations.add(*card, target, 0.5f, cardAnimationCallback);
	player.hero.placeStatsCard(std::move(card));
}

void Game::movePlayer(int index, Board::MoveSite moveSite, std::function<void()> callback)
{
	Player& player = players[index];
	player.boardSite = moveSite;
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
		sf::Vector2f buttonSize(500.0f, 200.0f);
		sf::Vector2f buttonPosition(window.getSize().x * 3.0f / 4.0f - buttonSize.x / 2.0f, window.getSize().y * 3.0f / 4.0f - buttonSize.y / 2.0f);
		buttons.push_back(std::make_unique<Button>("Låt äventyret börja!", buttonSize, buttonPosition, 60));
		std::function<void()> cardsDisplayedCallback =
			[this]()
		{
		};
		displayCards(getHeroCards(), cardsDisplayedCallback);
	}
}

void Game::onPlayerMoved(Tile& tile)
{
	setState(tile.enter());
}

void Game::startNewGame()
{
	board.clearMoveSites();
	//random.shuffle(players);
	activePlayerIndex = 0;
	startPlayerRound();
}

void Game::startPlayerRound()
{
	Player& player = players[activePlayerIndex];
	sf::Vector2f size(270.0f, 100.0f);
	sf::Vector2f position(0.0f, 370.0f);
	std::stringstream ss;
	ss << player.hero.getName() << std::endl;
	ss << "Kroppspoäng: " << player.life << " / " << player.hero.getMaxLife();
	buttons.emplace_back(std::make_unique<Button>(ss.str(), size, position, 30));
	board.setPlayerMoveSites(player.boardSite);
	board.showMoveSites(true);
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2f avatarCenter = board.getAvatarCenter(player.avatarIndex);
	animations.add(board, correctBoardPosition({ windowSize.x / 2.0f - avatarCenter.x, windowSize.y / 2.0f - avatarCenter.y }), 0.75f, []() {});
}

// Setup

template<State state>
struct StateHandlerInitializer
{
	StateHandlerInitializer(Game& game)
		: next(game)
	{
		game.onBeginTable[int(state)] = &Game::onBegin<state>;
		game.onTickTable[int(state)] = &Game::onTick<state>;
		game.onLeftMouseClickTable[int(state)] = &Game::onLeftMouseClick<state>;
		game.onEndTable[int(state)] = &Game::onEnd<state>;
	}

	StateHandlerInitializer<State(int(state) + 1)> next;
};

template<>
struct StateHandlerInitializer<State::Count>
{
	StateHandlerInitializer(Game&)
	{
	}
};

void Game::createStateLogicMap()
{
	StateHandlerInitializer<State::NoState>(*this);
}
