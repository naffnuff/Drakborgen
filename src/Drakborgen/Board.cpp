#include "Board.h"

#include <iostream>
#include <vector>

#include "System.h"
#include "Tile.h"
#include "Tower.h"
#include "Vault.h"
#include "Setup.h"

Board::Board(AnimationManager& animations)
	: clickOverlay(sf::Vector2f(tileSize, tileSize))
	, boardSprite(Setup::getMediaPath() + "spelplan.jpg")
	, vaultSprite(Setup::getMediaPath() + "skattkammaren.png")
	, animations(animations)
{
	vaultSprite.get().setPosition(getSitePosition({ 4, 6 }));
	vaultSprite.get().move(sf::Vector2f(0.0f, 10.0f));
	//players.reserve(4);

	tileGrid[0][0] = std::make_unique<Tower>(Direction::East, Direction::South);
	tileGrid[0][columnCount - 1] = std::make_unique<Tower>(Direction::West, Direction::South);
	tileGrid[rowCount - 1][0] = std::make_unique<Tower>(Direction::East, Direction::North);
	tileGrid[rowCount - 1][columnCount - 1] = std::make_unique<Tower>(Direction::West, Direction::North);
	tileGrid[4][6] = Vault::makeVault(Direction::North);
	tileGrid[5][6] = Vault::makeVault(Direction::South);
}

void Board::update(float elapsedTime, float /*timeDelta*/)
{
	// one cycle per second
	if (moveSiteAnimationStartTime == 0.0f)
	{
		moveSiteAnimationStartTime = elapsedTime;
	}
	const int wholeSeconds = int(elapsedTime - moveSiteAnimationStartTime);
	float fraction = elapsedTime - moveSiteAnimationStartTime - wholeSeconds;
	if (wholeSeconds % 2 != 0)
	{
		fraction = 1.0f - fraction;
	}
	clickOverlay.setFillColor(sf::Color(uint8_t(0x80 + 0x7f * fraction), uint8_t(0x80 + 0x7f * fraction), uint8_t(0x80 + 0x7f * fraction), uint8_t(0xff * fraction)));
}

void Board::placeTile(std::unique_ptr<Tile> tile, MoveSite moveSite)
{
	if (hasTile(moveSite.site) || !withinBounds(moveSite.site))
	{
		THROW;
	}
	tile->setPosition(getSitePosition(moveSite.site));
	tile->setOrientation(moveSite.direction);
	tileGrid[moveSite.site.row][moveSite.site.column] = std::move(tile);
}

sf::Vector2f Board::getSitePosition(Site site) const
{
	return sf::Vector2f(gridOriginX + tileSize * site.column, gridOriginY + tileSize * site.row);
}

Board::Site Board::getSite(sf::Vector2f position) const
{
	return { int((position.y - gridOriginY) / tileSize), int((position.x - gridOriginX) / tileSize) };
}

sf::Vector2f Board::getAvatarCenter(int index) const
{
	sf::FloatRect bounds = players[index].avatar->getGlobalBounds();
	return { bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f };
}

sf::Vector2f Board::getSize() const
{
	return boardSprite.get().getLocalBounds().getSize();
}

void Board::setGameStartMoveSites()
{
	moveSites = { { 0, 0 }, { 0, columnCount - 1 }, { rowCount - 1, 0 }, { rowCount - 1, columnCount - 1 } };
	moveSiteAnimationStartTime = 0.0f;
}

void Board::setPlayerMoveSites(MoveSite playerSite)
{
	for (const std::vector<Direction>& exits : getTile(playerSite.site)->getExits(playerSite.direction))
	{
		MoveSite moveSite = createMoveSite(playerSite.site, exits);
		if (withinBounds(moveSite.site))
		{
			moveSites.push_back(moveSite);
		}
	}
	moveSiteAnimationStartTime = 0.0f;
}

bool Board::testMoveSites(sf::Vector2f position) const
{
	if (!moveSitesShown)
	{
		THROW;
	}
	Site site = getSite(position);
	for (MoveSite moveSite : moveSites)
	{
		if (site == moveSite.site)
		{
			return true;
		}
	}
	return false;
}

Board::MoveSite Board::getMoveSite(sf::Vector2f position) const
{
	if (!moveSitesShown)
	{
		THROW;
	}
	Site site = getSite(position);
	for (MoveSite moveSite : moveSites)
	{
		if (site == moveSite.site)
		{
			return moveSite;
		}
	}
	THROW;
}

void Board::removeMoveSite(Site site)
{
	for (std::vector<MoveSite>::iterator it = moveSites.begin(); it < moveSites.end(); ++it)
	{
		if (it->site == site)
		{
			moveSites.erase(it);
			break;
		}
	}
}

void Board::clearMoveSites()
{
	moveSites.clear();
}

void Board::showMoveSites(bool show)
{
	moveSitesShown = show;
	if (show)
	{
		moveSiteAnimationStartTime = 0.0f;
	}
}

void Board::addPlayer(const std::string& imagePath, int index)
{
	if (players.size() != index)
	{
		THROW;
	}
	players.push_back({ std::make_unique<Card>(imagePath), invalidSite });
	Player& player = players.back();
	player.avatar->setScale(sf::Vector2f(1.0f / 3.0f, 1.0f / 3.0f));
}

void Board::setPlayerSite(int index, MoveSite moveSite, std::function<void()> callback)
{
	if (players.size() <= index)
	{
		THROW;
	}
	players[index].site = moveSite.site;
	placePlayer(index, moveSite.direction, callback);
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
	if (moveSitesShown)
	{
		for(MoveSite moveSite : moveSites)
		{
			sf::Transform transform = states.transform;
			sf::Vector2f offset = getSitePosition(moveSite.site);
			states.transform.translate(offset);
			target.draw(clickOverlay, states);
			states.transform = transform;
		}
	}
	for (const Player& player : players)
	{
		target.draw(*player.avatar, states);
	}
}

bool Board::hasTile(Site site) const
{
	std::cout << site.row << ":" << site.column << std::endl;
	return tileGrid[site.row][site.column] != nullptr;
}

std::unique_ptr<Tile>& Board::getTile(Site site)
{
	if (!hasTile(site))
	{
		THROW;
	}
	return tileGrid[site.row][site.column];
}

bool Board::withinBounds(Site site) const
{
	return site.row >= 0 && site.row < rowCount && site.column >= 0 && site.column < columnCount;
}

void Board::placePlayer(int index, Direction direction, std::function<void()> callback)
{
	Player& player = players[index];
	sf::Vector2f sitePosition = getSitePosition(player.site);
	sf::Vector2f avatarSize = player.avatar->getGlobalBounds().getSize();
	sf::Vector2f animationTarget(sitePosition.x + tileSize / 2.0f - avatarSize.x / 2.0f, sitePosition.y + tileSize / 2.0f - avatarSize.y / 2.0f);
	if (direction == Direction::Invalid)
	{
		float avatarStartX = animationTarget.x + (player.site.column == 0 ? -tileSize : tileSize) * 2.0f;
		float avatarStartY = animationTarget.y + (player.site.row == 0 ? -tileSize : tileSize);
		player.avatar->setPosition(sf::Vector2f(avatarStartX, avatarStartY));
	}
	animations.add(*player.avatar, animationTarget, 0.5f, callback);
}

Board::MoveSite Board::createMoveSite(Site site, const std::vector<Direction>& exits)
{
	for (Direction exit : exits)
	{
		switch (exit)
		{
		case Direction::North:
			--site.row;
			break;
		case Direction::South:
			++site.row;
			break;
		case Direction::West:
			--site.column;
			break;
		case Direction::East:
			++site.column;
			break;
		default:
			THROW;
			break;
		}
	}
	return { site, exits.back() };
}
