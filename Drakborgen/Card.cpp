#include "Card.h"

Card::Card(const std::string& imagePath)
	: sprite(imagePath)
{
}

Card::Card(Card&& other) noexcept
{
	*this = std::move(other);
}

Card& Card::operator=(Card&& other) noexcept
{
	sprite = std::move(other.sprite);
	return *this;
}

void Card::centerAround(sf::Vector2f position)
{
	sf::FloatRect bounds = getBounds();
	sprite.get().setPosition(position.x - float(bounds.width) / 2.0f, position.y - float(bounds.height) / 2.0f);
}

sf::FloatRect Card::getBounds()
{
	return sprite.get().getGlobalBounds();
}

void Card::setScale( float factorX, float factorY )
{
	sprite.get().setScale(factorX, factorY);
}

void Card::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite.get(), states);
}
