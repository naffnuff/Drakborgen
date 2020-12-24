#include "Board.h"

#include <iostream>
#include <vector>

#include "System.h"
#include "TileLogic.h"
#include "Tower.h"

Board::Board()
	: clickOverlay(sf::Vector2f(tileSize, tileSize))
	, boardSprite("Media/spelplan.jpg")
	, vaultSprite( "Media/skattkammaren.png" )
{
	vaultSprite.get().setPosition(getSitePosition({ 4, 6 }));
	vaultSprite.get().move(0.0f, 5.0f);
	//players.reserve(4);

	tileGrid[0][0] = Tile::create<Tower>(TileLogic::Exit::East, TileLogic::Exit::South);
	tileGrid[0][columnCount - 1] = Tile::create<Tower>(TileLogic::Exit::West, TileLogic::Exit::South);
	tileGrid[rowCount - 1][0] = Tile::create<Tower>(TileLogic::Exit::East, TileLogic::Exit::North);
	tileGrid[rowCount - 1][columnCount - 1] = Tile::create<Tower>(TileLogic::Exit::West, TileLogic::Exit::North);
	tileGrid[4][6] = Tile::create<Tower>();
	tileGrid[5][6] = Tile::create<Tower>();
}

void Board::update(float elapsedTime, float /*timeDelta*/)
{
	// one cycle per second
	if (clickSiteAnimationStartTime == 0.0f)
	{
		clickSiteAnimationStartTime = elapsedTime;
	}
	const int wholeSeconds = int(elapsedTime - clickSiteAnimationStartTime);
	float fraction = elapsedTime - clickSiteAnimationStartTime - wholeSeconds;
	if (wholeSeconds % 2 == 0)
	{
		fraction = 1.0f - fraction;
	}
	clickOverlay.setFillColor(sf::Color(uint8_t(0x80 + 0x7f * fraction), uint8_t(0x80 + 0x7f * fraction), uint8_t(0x80 + 0x7f * fraction), uint8_t(0xff * fraction)));
}

void Board::placeTile(std::unique_ptr<Tile> tile, Site site)
{
	if (hasTile(site) || !withinBounds(site))
	{
		THROW;
	}
	tile->setPosition(getSitePosition(site));
	tileGrid[site.row][site.column] = std::move(tile);
}

sf::Vector2f Board::getSitePosition(Site site) const
{
	return sf::Vector2f(gridOriginX + tileSize * site.column, gridOriginY + tileSize * site.row);
}

Board::Site Board::getSite(sf::Vector2f position) const
{
	return { int((position.y - gridOriginY) / tileSize), int((position.x - gridOriginX) / tileSize) };
}

sf::Vector2f Board::getSize() const
{
	return boardSprite.get().getLocalBounds().getSize();
}

void Board::setGameStartClickSites()
{
	clickSites = { { 0, 0 }, { 0, columnCount - 1 }, { rowCount - 1, 0 }, { rowCount - 1, columnCount - 1 } };
	clickSiteAnimationStartTime = 0.0f;
}

void Board::setPlayerMoveClickSites(Site playerSite)
{
	for (TileLogic::Exit exit : getTile(playerSite)->getLogic()->getExits())
	{
		clickSites.push_back(step(playerSite, exit));
	}
	clickSiteAnimationStartTime = 0.0f;
}

bool Board::testClickSites(sf::Vector2f position) const
{
	if (!clickSitesShown)
	{
		THROW;
	}
	for (Site clickSite : clickSites)
	{
		Site site = getSite(position);
		if (site == clickSite)
		{
			return true;
		}
	}
	return false;
}

void Board::removeClickSite( Site site )
{
	for (std::vector<Site>::iterator it = clickSites.begin(); it < clickSites.end(); ++it)
	{
		if (*it == site)
		{
			clickSites.erase(it);
			break;
		}
	}
}

void Board::clearClickSites()
{
	clickSites.clear();
}

void Board::showClickSites(bool show)
{
	clickSitesShown = show;
	if (show)
	{
		clickSiteAnimationStartTime = 0.0f;
	}
}

void Board::addPlayer(const std::string& imagePath, int index)
{
	if (players.size() != index)
	{
		THROW;
	}
	players.push_back({ Card(imagePath), invalidSite });
	Player& player = players.back();
	player.avatar.setScale(1.0f / 3.0f, 1.0f / 3.0f);
}

void Board::setPlayerSite(int index, Site site)
{
	if (players.size() <= index)
	{
		THROW;
	}
	players[index].site = site;
	placePlayer(index);
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(boardSprite.get(), states);
	target.draw(vaultSprite.get(), states);
	for (const std::array<std::unique_ptr<Tile>, 13>& row : tileGrid)
	{
		for (const std::unique_ptr<Tile>& tile : row)
		{
			if (tile)
			{
				target.draw(*tile, states);
			}
		}
	}
	if (clickSitesShown)
	{
		for(Site site : clickSites)
		{
			sf::Transform transform = states.transform;
			sf::Vector2f offset = getSitePosition(site);
			states.transform.translate(offset.x, offset.y);
			target.draw(clickOverlay, states);
			states.transform = transform;
		}
	}
	for (const Player& player : players)
	{
		target.draw(player.avatar, states);
	}
}

bool Board::hasTile(Site site) const
{
	return tileGrid[site.row][site.column] != nullptr;
}

bool Board::withinBounds(Site site) const
{
	return site.row >= 0 && site.row < rowCount && site.column >= 0 && site.column < columnCount;
}

void Board::placePlayer(int index)
{
	Player& player = players[index];
	sf::Vector2f sitePosition = getSitePosition(player.site);
	player.avatar.centerAround({ sitePosition.x + tileSize / 2.0f, sitePosition.y + tileSize / 2.0f });
}

std::unique_ptr<Tile>& Board::getTile(Site site)
{
	if (!hasTile(site))
	{
		THROW;
	}
	return tileGrid[site.row][site.column];
}

Board::Site Board::step(Site site, TileLogic::Exit exit)
{
	switch (exit)
	{
	case TileLogic::Exit::North:
		--site.row;
		break;
	case TileLogic::Exit::South:
		++site.row;
		break;
	case TileLogic::Exit::West:
		--site.column;
		break;
	case TileLogic::Exit::East:
		++site.column;
		break;
	default:
		THROW;
		break;
	}
	return site;
}
