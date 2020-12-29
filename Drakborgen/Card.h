#pragma once

#include "UniqueSprite.h"

class Card : public sf::Drawable, public sf::Transformable
{
public:
	Card(const std::string& imagePath);

	Card(const Card&) = delete;
	Card& operator=(const Card&) = delete;
	
	//Card(Card&& other) noexcept;
	//Card& operator=(Card&& other) noexcept;

	void centerAround(sf::Vector2f position);
	sf::FloatRect getGlobalBounds();

	std::string imagePath;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	UniqueSprite sprite;
};

