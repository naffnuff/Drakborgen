#pragma once

#include "UniqueSprite.h"

class Card : public sf::Drawable, public sf::Transformable
{
public:
	Card(const std::string& imagePath);

	Card(Card&) = delete;
	void operator=(Card&) = delete;
	
	Card(Card&& other) noexcept;
	void operator=(Card&& other) noexcept;

	void centerAround(sf::Vector2f position);
	sf::FloatRect getBounds();
	void setScale(float factorX, float factorY);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	UniqueSprite sprite;
};

