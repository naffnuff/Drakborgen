#pragma once

#include "UniqueSprite.h"

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	Tile();
	Tile(const std::string& imagePath);

	Tile(Tile&) = delete;
	Tile& operator=(Tile&) = delete;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	UniqueSprite sprite;
};

