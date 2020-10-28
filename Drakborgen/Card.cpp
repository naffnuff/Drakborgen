#include "Card.h"

Card::Card(const std::string& imagePath)
{
	if (!texture.loadFromFile(imagePath))
	{
		throw std::runtime_error("No card image found: " + imagePath);
	}
	sprite.setTexture(texture);
}

void Card::centerAround(sf::Vector2f position)
{
	sf::Vector2u dimensions = texture.getSize();
	sprite.setPosition(position.x - float(dimensions.x) / 2.0f, position.y - float(dimensions.y) / 2.0f);
}

sf::FloatRect Card::getBounds()
{
	return sprite.getGlobalBounds();
}

void Card::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
}
