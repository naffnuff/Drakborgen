#include "Game.h"

#include "Tile.h"
#include "Setup.h"
#include "Engine.h"
#include "StateHandlerInitializer.h"

#include "SFML/Graphics.hpp"

#include <iostream>
#include <sstream>
#include <memory>

Game::Game(Engine& engine, Random& random, Board& board)
	: engine(engine)
	, random(random)
	, board(board)
	, cardDisplay(engine)
	, tiles(random)
{
	createStateLogicMap();
}

void Game::setState(State state)
{
	engine.setState(state);
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

	displayCards(getHeroCards(), [this](int heroIndex) { pickHero(heroIndex); });

	setState(State::PickHero);
}

template<>
void Game::onBegin<State::PickStartTower>()
{
	board.showMoveSites(true);
}

template<>
void Game::onLeftMouseClick<State::PickStartTower>()
{
	sf::Vector2f mouseBoardPosition = engine.getMouseBoardPosition();
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
	sf::Vector2f mouseBoardPosition = engine.getMouseBoardPosition();
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

template<>
void Game::onBegin<State::ViewStatsCard>()
{
	//board.showMoveSites(false);
	std::unique_ptr<Card> card = players[activePlayerIndex].hero.pullStatsCard();
	if (!card)
	{
		THROW;
	}
	displayCard(std::move(card), [this]() { setState(State::PlayerMove); });
}

template<>
void Game::onEnd<State::ViewStatsCard>()
{
	std::unique_ptr<Card> card = cardDisplay.pullCard();
	moveOffScreen(card.get(), 0.5f, [this]() { placeAtOrigin(players[activePlayerIndex].hero.getStatsCard()); });
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

void Game::pickHero(int heroIndex)
{
	engine.clearInternalButtons();
	panToNextFreeTower();
	createPlayer(heroIndex);
	setState(State::PickStartTower);
}

void Game::requestGameStart()
{
	engine.clearInternalButtons();
	for (int i = int(idleHeroes.size()) - 1; i >= 0; --i)
	{
		std::unique_ptr<Card> card = cardDisplay.pullCard(i);
		moveOffScreen(card.get(), float(i + 1) / float(idleHeroes.size()), []() {});
		idleHeroes[i].placeStatsCard(std::move(card));
	}
	startNewGame();
	setState(State::PlayerMove);
}

void Game::placeAtOrigin(Card* card) const
{
	sf::Vector2u windowSize = engine.getWindowSize();
	sf::Vector2f cardSize = card->getBounds().getSize();
	card->setPosition({ -cardSize.x, float(windowSize.y) });
}

void Game::moveOffScreen(Card* card, float time, std::function<void()> callback)
{
	sf::Vector2u windowSize = engine.getWindowSize();
	sf::Vector2f cardSize = card->getBounds().getSize();
	engine.addExternalVisual(card);
	engine.animate(*card, { float(windowSize.x), -cardSize.y }, time, [this, card, callback]() { engine.removeExternalVisual(card); callback(); });
}

void Game::moveToCenter(Card* card, std::function<void()> callback)
{
	sf::Vector2f cardSize = card->getBounds().getSize();
	sf::Vector2u windowSize = engine.getWindowSize();
	sf::Vector2f animationTarget(windowSize.x / 2.0f - cardSize.x / 2.0f, windowSize.y / 2.0f - cardSize.y / 2.0f);
	engine.animate(*card, animationTarget, 0.5f, callback);
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
	moveToCenter(card.get(), []() {});
	cardDisplay.pushCard(std::move(card), callback);
}

void Game::displayCards(std::vector<std::unique_ptr<Card>>&& cards, std::function<void(int)> callback)
{
	std::vector<sf::Vector2f> layout = cardDisplay.getLayout(cards);
	for (int i = 0; i < cards.size(); ++i)
	{
		placeAtOrigin(cards[i].get());
		engine.animate(*cards[i], layout[i], float(i + 1) / float(cards.size()), []() { });
		cardDisplay.pushCard(std::move(cards[i]), [callback, i]() { callback(i); });
	}
}

void Game::panToNextFreeTower()
{
	sf::Vector2f boardTarget(0.0f, 0.0f);
	sf::Vector2u windowSize = engine.getWindowSize();
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
	engine.animate(board, engine.correctBoardPosition(boardTarget), 0.75f, []() { });
}

void Game::createPlayer(int heroIndex)
{
	std::unique_ptr<Card> pickedCard = cardDisplay.pullCard(heroIndex);
	moveToCenter(pickedCard.get(), []() {});
	players.push_back({ std::move(idleHeroes[heroIndex]), Board::invalidMoveSite, idleHeroes[heroIndex].getMaxLife() });
	++activePlayerIndex;
	idleHeroes.erase(idleHeroes.begin() + heroIndex);
	for (int i = int(idleHeroes.size()) - 1; i >= 0; --i)
	{
		std::unique_ptr<Card> card = cardDisplay.pullCard(i);
		moveOffScreen(card.get(), float(i + 1) / float(idleHeroes.size()), []() {});
		idleHeroes[i].placeStatsCard(std::move(card));
	}
	cardDisplay.pushCard(std::move(pickedCard), []() {});
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
	Card* cardPtr = card.get();
	engine.addExternalVisual(cardPtr);
	std::function<void()> cardAnimationCallback = [this, &player, site, cardPtr, callback]() {
		engine.removeExternalVisual(cardPtr);
		board.addPlayer(Setup::getMediaPath() + "hjaltekort/" + player.hero.getId() + "gubbe.png", player.avatarIndex);
		board.setPlayerSite(player.avatarIndex, { site, Direction::Invalid }, callback);
		placeAtOrigin(player.hero.getStatsCard());
	};
	sf::Vector2f target;
	sf::Vector2f cardSize = card->getBounds().getSize();
	sf::Vector2u windowSize = engine.getWindowSize();
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
	engine.animate(*card, target, 0.5f, cardAnimationCallback);
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
		sf::Vector2f buttonPosition(engine.getWindowSize().x * 3.0f / 4.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 3.0f / 4.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Låt äventyret börja!", buttonSize, buttonPosition, 60, [this]() { requestGameStart(); });
		displayCards(getHeroCards(), [this](int heroIndex) { pickHero(heroIndex); });
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
	engine.createInternalButton(ss.str(), size, position, 30, [this]() { setState(State::ViewStatsCard); });
	board.setPlayerMoveSites(player.boardSite);
	board.showMoveSites(true);
	sf::Vector2u windowSize = engine.getWindowSize();
	sf::Vector2f avatarCenter = board.getAvatarCenter(player.avatarIndex);
	engine.animate(board, engine.correctBoardPosition({ windowSize.x / 2.0f - avatarCenter.x, windowSize.y / 2.0f - avatarCenter.y }), 0.75f, []() {});
}

// Setup

void Game::createStateLogicMap()
{
	StateHandlerInitializer<State::SetupGame, State::Count, Game>(engine, *this);
}
