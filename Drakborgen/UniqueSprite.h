#pragma once

#include <memory>

#include "SFML/Graphics.hpp"

class UniqueSprite
{
public:
	UniqueSprite() = default;

	UniqueSprite(const std::string& imagePath);

	UniqueSprite(UniqueSprite&) = delete;
	void operator=(UniqueSprite&) = delete;

	UniqueSprite(UniqueSprite&& other) noexcept;
	
	void operator=(UniqueSprite&& other) noexcept;

	sf::Sprite& get() { return *sprite; }
	const sf::Sprite& get() const { return *sprite; }

private:
	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::Sprite> sprite;
};

