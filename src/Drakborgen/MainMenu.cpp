#include "MainMenu.h"

#include "Engine.h"
#include "StateHandlerInitializer.h"

#include "SFML/Graphics.hpp"

MainMenu::MainMenu(Engine& engine, Random& random)
	: engine(engine)
	, random(random)
{
	createStateLogicMap();
}

void MainMenu::setState(State state)
{
	engine.setState(state);
}

template<>
void MainMenu::onBegin<State::SelectNetRole>()
{
	sf::Vector2f buttonSize(600.0f, 160.0f);
	{
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 1.0f / 4.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Snåla", buttonSize, buttonPosition, 60, [this]() { setState(State::SetupGame); });
	}
	{
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 2.0f / 4.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Håll gästabud", buttonSize, buttonPosition, 60, [this]() { setState(State::SetupServer); });
	}
	{
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 3.0f / 4.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Snylta", buttonSize, buttonPosition, 60, [this]() { setState(State::SetupClient); });
	}
}

template<>
void MainMenu::onEnd<State::SelectNetRole>()
{
	engine.clearInternalButtons();
}

// Server setup

template<>
void MainMenu::onBegin<State::SetupServer>()
{
	sf::Vector2f buttonSize(600.0f, 160.0f);
	{
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 1.0f / 4.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Bjud in en gäst", buttonSize, buttonPosition, 60, [this]() { startServer(1); });
	}
	{
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 2.0f / 4.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Bjud in två gäster", buttonSize, buttonPosition, 60, [this]() { startServer(2); });
	}
	{
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 3.0f / 4.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Bjud in tre gäster", buttonSize, buttonPosition, 60, [this]() { startServer(3); });
	}
}

template<>
void MainMenu::onEnd<State::SetupServer>()
{
	engine.clearInternalButtons();
}

// Client setup

template<>
void MainMenu::onBegin<State::SetupClient>()
{
	{
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 5.0f / 6.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Låt färden gå!", buttonSize, buttonPosition, 60, [this]() { startClient(serverAddress); });
	}
	{
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 2.0f / 5.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Rasmus", buttonSize, buttonPosition, 60, [this]() {});
	}
	{
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 4.0f / 5.0f - buttonSize.y / 2.0f);
		engine.createInternalButton(serverAddress, buttonSize, buttonPosition, 60, [this]() {});
	}
	{
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 1.0f / 8.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Om vem skall legenden vittna?", buttonSize, buttonPosition, 60, [this]() {});
	}
	{
		sf::Vector2f buttonSize(600.0f, 120.0f);
		sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 2.0f / 8.0f - buttonSize.y / 2.0f);
		engine.createInternalButton("Varthän går färden?", buttonSize, buttonPosition, 60, [this]() {});
	}
}

template<>
void MainMenu::onEnd<State::SetupClient>()
{
	engine.clearInternalButtons();
}

template<>
void MainMenu::onBegin<State::AwaitingConnection>()
{
	sf::Vector2f buttonSize(600.0f, 160.0f);
	sf::Vector2f buttonPosition(engine.getWindowSize().x / 2.0f - buttonSize.x / 2.0f, engine.getWindowSize().y * 2.0f / 4.0f - buttonSize.y / 2.0f);
	engine.createInternalButton("Sällskapet samlas...", buttonSize, buttonPosition, 60, [this]() {});
}

template<>
void MainMenu::onTick<State::AwaitingConnection>()
{
	if (engine.getNetwork().isConnected())
	{
		setState(State::NoState);
	}
}

template<>
void MainMenu::onEnd<State::AwaitingConnection>()
{
	engine.clearInternalButtons();
}

// Helpers

void MainMenu::startServer(int clientCount)
{
	engine.getNetwork().startServer(clientCount, random);
	setState(State::NoState);
}

void MainMenu::startClient(const std::string& serverAddress)
{
	engine.getNetwork().startClient(serverAddress);
	setState(State::NoState);
}

// Setup

void MainMenu::createStateLogicMap()
{
	StateHandlerInitializer<State::NoState, State::SetupGame, MainMenu>(engine, *this);
}
