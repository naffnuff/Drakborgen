#pragma once

#include <memory>

#include <SFML/Graphics/Sprite.hpp>
#include "SFML/Graphics.hpp"

class UniqueSprite
{
public:
	UniqueSprite() = default;

	UniqueSprite(const std::string& imagePath);

	UniqueSprite(const UniqueSprite&) = delete;
	UniqueSprite& operator=(const UniqueSprite&) = delete;

	UniqueSprite(UniqueSprite&& other) noexcept;
	
	UniqueSprite& operator=(UniqueSprite&& other) noexcept;

	operator bool() const { return valid; }

	sf::Sprite& get() { return *sprite; }
	const sf::Sprite& get() const { return *sprite; }

private:
	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::Sprite> sprite;
	bool valid = false;
};

