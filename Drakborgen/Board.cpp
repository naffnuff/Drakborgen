#include "Board.h"

#include <iostream>

Board::Board()
	: clickOverlay(sf::Vector2f(tileSize, tileSize))
{
	if (!boardTexture.loadFromFile("Media/spelplan.jpg"))
	{
		throw std::runtime_error("No board image found");
	}
	if (!vaultTexture.loadFromFile("Media/skattkammaren.png"))
	{
		throw std::runtime_error("No vault image found");
	}
	boardSprite.setTexture(boardTexture);
	vaultSprite.setTexture(vaultTexture);
	vaultSprite.setPosition(getSitePosition({ 4, 6 }));
	vaultSprite.move(0.0f, 5.0f);
	//players.reserve(4);

	tileGrid[0][0] = std::make_unique<Tile>();
	tileGrid[0][columnCount - 1] = std::make_unique<Tile>();
	tileGrid[rowCount - 1][0] = std::make_unique<Tile>();
	tileGrid[rowCount - 1][columnCount - 1] = std::make_unique<Tile>();
	tileGrid[4][6] = std::make_unique<Tile>();
	tileGrid[5][6] = std::make_unique<Tile>();
}

void Board::update(float elapsedTime, float timeDelta)
{
	// one cycle per second
	const int wholeSeconds = elapsedTime;
	float fraction = elapsedTime - wholeSeconds;
	if (wholeSeconds % 2 == 0)
	{
		fraction = 1.0f - fraction;
	}
	clickOverlay.setFillColor(sf::Color(0x80 + 0x7f * fraction, 0x80 + 0x7f * fraction, 0x80 + 0x7f * fraction, 0xff * fraction));
}

void Board::placeTile(std::unique_ptr<Tile> tile, Site site)
{
	if (hasTile(site) || !withinBounds(site))
	{
		throw std::runtime_error("Illegal tile placement");
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

void Board::setClickSites(const std::vector<Site>& sites)
{
	clickSites = sites;
	clickSiteAnimationStartTime = 0.0f;
}

bool Board::testClickSites(sf::Vector2f position) const
{
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

void Board::addPlayer(const std::string& imagePath, int index)
{
	if (players.size() != index)
	{
		throw std::runtime_error("Board: player index mismatch");
	}
	players.push_back({ Card(imagePath), invalidSite });
	Player& player = players.back();
	player.avatar.setScale(1.0f / 3.0f, 1.0f / 3.0f);
}

void Board::setPlayerSite(int index, Site site)
{
	if (players.size() <= index)
	{
		throw std::runtime_error("Board: player index out-of-bounds");
	}
	players[index].site = site;
	placePlayer(index);
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(boardSprite, states);
	target.draw(vaultSprite, states);
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
	for (Site site : clickSites)
	{
		sf::Transform transform = states.transform;
		sf::Vector2f offset = getSitePosition(site);
		states.transform.translate(offset.x, offset.y);
		target.draw(clickOverlay, states);
		states.transform = transform;
	}
	for (const Player& player : players)
	{
		target.draw(player.avatar, states);
	}
}

bool Board::hasTile(Site site) const
{
	return !!tileGrid[site.row][site.column];
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
