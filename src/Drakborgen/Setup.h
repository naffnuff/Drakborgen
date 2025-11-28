#pragma once

#include <optional>
#include <filesystem>

#include "Game.h"

class Setup
{
public:
	Setup() = delete;
	Setup(const Setup&) = delete;
	Setup& operator=(const Setup&) = delete;

	static std::string getMediaPath();

	static void setUpGame(Game& game);

private:
	static void createTiles(TileDeck& tiles);

	template<typename RoomType, typename... Args>
	static void createTile(TileDeck& tiles, const std::string& number, Args... args)
	{
		tiles.createItem<RoomType>(getMediaPath() + "rumsbrickor/rumsbricka0" + number + ".png", args...);
	}

private:
	static std::optional<std::filesystem::path> mediaPath;
};

