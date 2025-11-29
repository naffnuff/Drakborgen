#pragma once

#include "State.h"

#include <string>

class Engine;
class Random;

class MainMenu
{
public:
	MainMenu(Engine& engine, Random& random);

private:
	void setState(State state);

	template<State>
	void onBegin() {}

	template<State>
	void onTick() {}

	template<State>
	void onLeftMouseClick() {}

	template<State>
	void onEnd() {}

	void startServer(int clientCount);
	void startClient(const std::string& serverAddress);

	void createStateLogicMap();

private:
	static constexpr const char* serverAddress = "192.168.1.121";

	Engine& engine;

	Random& random;

	template<State state, State endState, typename Handler>
	friend struct StateHandlerInitializer;
};

