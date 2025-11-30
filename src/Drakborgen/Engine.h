#pragma once

#include "State.h"
#include "Network.h"

#include "SFML/Graphics.hpp"

#include <functional>
#include <array>
#include <set>

#define BLESS_THIS_MESS 1

class AnimationManager;
class Board;
class TextBox;
class Button;
class Visual;

class Engine
{
public:
	Engine(AnimationManager& animations, Board& board);

	void run();

	sf::Vector2u getWindowSize();

	void setState(State newState);

	void animate(sf::Transformable& transformable, sf::Vector2f target, float time, std::function<void()> doneCallback);

	void addExternalButton(Button* button);
	void removeExternalButton(Button* button);

	void addExternalVisual(Visual* visual);
	void removeExternalVisual(Visual* visual);

	sf::Vector2f getMouseBoardPosition() const;
	sf::Vector2f correctBoardPosition(sf::Vector2f boardPosition);

	Network& getNetwork();

	void createInternalButton(const std::string& message, sf::Vector2f size, sf::Vector2f position, int textSize, std::function<void()> callback);
	void clearInternalButtons();

private:
	void processSystemEvents();

	using EventTable = std::array<std::function<void()>, (size_t)State::Count>;
	void invokeEventHandler(const EventTable& eventTable);

	Button* getHitButton(sf::Vector2f position) const;

private:
	sf::RenderWindow window;

	Network network;

	State state = State::NoState;

	EventTable onBeginTable;
	EventTable onTickTable;
	EventTable onLeftMouseClickTable;
	EventTable onEndTable;

	AnimationManager& animations;

	Board& board;

	bool xCenteredBoard = false;
	bool yCenteredBoard = false;

	bool leftMouseButtonDown = false;
	sf::Vector2f buttonPressedBoardPosition;
	sf::Vector2f buttonPressedMousePosition;
	sf::Vector2f buttonReleasedMousePosition;
	Button* capturedButton = nullptr;

	std::set<Button*> buttons;
	std::set<Visual*> visuals;

	std::vector<std::unique_ptr<TextBox>> textBoxes;
	std::vector<std::unique_ptr<Button>> internalButtons;

	template<State state, State endState, typename Handler>
	friend struct StateHandlerInitializer;
};
