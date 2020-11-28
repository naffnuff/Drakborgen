#pragma once

#include <memory>
#include <array>

#include "UniqueSprite.h"

#include "Tile.h"
#include "Card.h"

class Board : public sf::Drawable, public sf::Transformable
{
public:
	static constexpr float gridOriginX = 412.f;
	static constexpr float gridOriginY = 119.f;
	static constexpr float tileSize = 177.f;

	struct Site
	{
		int row = 0;
		int column = 0;
	};

	static constexpr Site invalidSite = { -1, -1 };

private:
	struct Player
	{
		Card avatar;
		Site site;
	};

public:
	static constexpr int rowCount = 10;
	static constexpr int columnCount = 13;

	Board();

	Board(const Board&) = delete;
	Board& operator=(const Board&) = delete;

	void update(float elapsedTime, float timeDelta);
	bool hasTile(Site site) const;
	void placeTile(std::unique_ptr<Tile> tile, Site site);
	sf::Vector2f getSitePosition(Site site) const;
	Site getSite(sf::Vector2f position) const;

	void setGameStartClickSites();
	void setPlayerMoveClickSites(Site playerSite);
	bool testClickSites(sf::Vector2f position) const;
	void removeClickSite(Site site);
	void clearClickSites();
	void showClickSites(bool show);

	void addPlayer(const std::string& imagePath, int index);
	void setPlayerSite(int index, Site site);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool withinBounds(Site site) const;
	void placePlayer(int index);

	std::unique_ptr<Tile>& getTile(Site site);

	Site step(Site site, TileLogic::Exit exit);

private:
	UniqueSprite boardSprite;
	UniqueSprite vaultSprite;
	sf::RectangleShape clickOverlay;
	std::vector<Player> players;

	std::array<std::array<std::unique_ptr<Tile>, columnCount>, rowCount> tileGrid;
	std::vector<Site> clickSites;
	bool clickSitesShown = false;
	float clickSiteAnimationStartTime = 0.0f;
};

inline bool operator==(Board::Site siteOne, Board::Site siteTwo)
{
	return siteOne.row == siteTwo.row && siteOne.column == siteTwo.column;
}
