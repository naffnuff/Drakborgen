#pragma once

enum class State
{
	NoState,
	SelectNetRole,
	SetupServer,
	AwaitClients,
	SetupClient,
	ConnectToServer,
	SetupGame,
	PickHero,
	PickStartTower,
	PlayerMove,
	ViewStatsCard,
	TurnContinue,
	TurnEnd,
	StateCount
};