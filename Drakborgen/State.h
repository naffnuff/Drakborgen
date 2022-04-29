#pragma once

enum class State
{
	NoState,
	SelectNetRole,
	SetupServer,
	SetupClient,
	AwaitingConnections,
	Connect,
	SetupGame,
	PickHero,
	PickStartTower,
	PlayerMove,
	ViewStatsCard,
	TurnContinue,
	TurnEnd,
	StateCount
};