#pragma once

#include "State.h"
#include "Engine.h"

template<State state, State endState, typename Handler>
struct StateHandlerInitializer
{
	StateHandlerInitializer(Engine& engine, Handler& handler)
		: next(engine, handler)
	{
		engine.onBeginTable[int(state)] = [&handler]() { handler.template onBegin<state>(); };
		engine.onTickTable[int(state)] = [&handler]() { handler.template onTick<state>(); };
		engine.onLeftMouseClickTable[int(state)] = [&handler]() { handler.template onLeftMouseClick<state>(); };
		engine.onEndTable[int(state)] = [&handler]() { handler.template onEnd<state>(); };
	}

	StateHandlerInitializer<State(int(state) + 1), endState, Handler> next;
};

template<State endState, typename Handler>
struct StateHandlerInitializer<endState, endState, Handler>
{
	StateHandlerInitializer(Engine&, Handler&)
	{
	}
};
