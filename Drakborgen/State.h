#pragma once

enum class State
{
	NoState,
	SelectNetRole,
	SetupServer,
	SetupClient,
	ConnectToServer,
	AwaitClients,
	SetupGame,
	PickHero,
	PickStartTower,
	PlayerMove,
	ViewStatsCard,
	TurnContinue,
	TurnEnd,
	StateCount
};