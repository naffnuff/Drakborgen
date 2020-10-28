#include "UniqueSprite.h"

UniqueSprite::UniqueSprite(const std::string& imagePath)
	: texture(std::make_unique<sf::Texture>())
	, sprite(std::make_unique<sf::Sprite>())
{
	if (!texture->loadFromFile(imagePath))
	{
		throw std::runtime_error("No image found: " + imagePath);
	}
	sprite->setTexture(*texture);
}

UniqueSprite::UniqueSprite(UniqueSprite&& other) noexcept
{
	*this = std::move(other);
}

void UniqueSprite::operator=(UniqueSprite&& other) noexcept
{
	sprite = std::move(other.sprite);
	texture = std::move(other.texture);
}
