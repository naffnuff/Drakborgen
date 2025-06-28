#pragma once

enum class State
{
	NoState,
	SelectNetRole,
	SetupServer,
	SetupClient,
	AwaitingConnection,
	SetupGame,
	PickHero,
	PickStartTower,
	PlayerMove,
	ViewStatsCard,
	TurnContinue,
	TurnEnd,
	StateCount
};