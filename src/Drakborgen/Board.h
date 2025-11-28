#pragma once

#include <memory>
#include <array>

#include "UniqueSprite.h"

#include "Tile.h"
#include "Card.h"
#include "Animation.h"

class Board : public sf::Drawable, public sf::Transformable
{
public:
	static constexpr float gridOriginX = 418.f;
	static constexpr float gridOriginY = 119.f;
	static constexpr float tileSize = 178.f;

	struct Site
	{
		int row = 0;
		int column = 0;
	};

	struct MoveSite
	{
		Site site;
		Direction direction = Direction::Invalid;
	};

	static constexpr Site invalidSite = { -1, -1 };
	static constexpr MoveSite invalidMoveSite = { invalidSite, Direction::Invalid };

private:
	struct Player
	{
		std::unique_ptr<Card> avatar;
		Site site;
	};

public:
	static constexpr int rowCount = 10;
	static constexpr int columnCount = 13;

	Board(AnimationManager& animations);

	Board(const Board&) = delete;
	Board& operator=(const Board&) = delete;

	void update(float elapsedTime, float timeDelta);
	bool hasTile(Site site) const;
	std::unique_ptr<Tile>& getTile(Site site);
	void placeTile(std::unique_ptr<Tile> tile, MoveSite moveSite);
	sf::Vector2f getSitePosition(Site site) const;
	Site getSite(sf::Vector2f position) const;
	sf::Vector2f getAvatarCenter(int index) const;

	sf::Vector2f getSize() const;

	void setGameStartMoveSites();
	void setPlayerMoveSites(MoveSite playerSite);
	bool testMoveSites(sf::Vector2f position) const;
	MoveSite getMoveSite(sf::Vector2f position) const;
	void removeMoveSite(Site site);
	void clearMoveSites();
	void showMoveSites(bool show);

	void addPlayer(const std::string& imagePath, int index);
	void setPlayerSite(int index, MoveSite moveSite, std::function<void()> callback);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool withinBounds(Site site) const;
	void placePlayer(int index, Direction direction, std::function<void()> callback);

	MoveSite createMoveSite(Site site, const std::vector<Direction>& exits);

private:
	UniqueSprite boardSprite;
	UniqueSprite vaultSprite;
	sf::RectangleShape clickOverlay;
	std::vector<Player> players;

	std::array<std::array<std::unique_ptr<Tile>, columnCount>, rowCount> tileGrid;
	std::vector<MoveSite> moveSites;
	bool moveSitesShown = false;
	float moveSiteAnimationStartTime = 0.0f;

	AnimationManager& animations;
};

inline bool operator==(Board::Site siteOne, Board::Site siteTwo)
{
	return siteOne.row == siteTwo.row && siteOne.column == siteTwo.column;
}
