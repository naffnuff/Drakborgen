#pragma once

#include "SFML/Graphics.hpp"

class Visual : public sf::Drawable
{
public:
	virtual sf::FloatRect getBounds() const = 0;
};

