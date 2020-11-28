#pragma once

#include "TileLogic.h"
#include "UniqueSprite.h"

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile(std::unique_ptr<TileLogic> tileLogic);
	Tile(const std::string& imagePath);

	Tile(const Tile&) = delete;
	Tile& operator=(const Tile&) = delete;

	template<typename LogicType, typename... Args>
	static std::unique_ptr<Tile> create(Args... args)
	{
		std::unique_ptr<Tile> tile = std::make_unique<Tile>(std::make_unique<LogicType>(args...));
		return tile;
	}

	std::unique_ptr<TileLogic>& getLogic();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	UniqueSprite sprite;

	std::unique_ptr<TileLogic> tileLogic;
};

