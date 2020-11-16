#pragma once

#include "UniqueSprite.h"

class Tile : public sf::Drawable, public sf::Transformable
{
public:
	enum class Exit
	{
		North,
		South,
		West,
		East
	};

public:
	Tile() { }
	Tile(const std::string& imagePath);

	Tile(const Tile&) = delete;
	Tile& operator=(const Tile&) = delete;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	UniqueSprite sprite;

	std::vector<Exit> exits;
};

