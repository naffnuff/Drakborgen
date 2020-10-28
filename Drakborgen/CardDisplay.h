#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Card.h"

class CardDisplay : public sf::Drawable, public sf::Transformable
{
public:
	CardDisplay(sf::Window& window);
	
	bool empty() const { return cards.empty(); }

	int hitTest(sf::Vector2i mousePosition) const;

	void pushCard(std::unique_ptr<Card> card);
	std::unique_ptr<Card> pullCard(int index = 0);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void placeCards();

private:
	sf::Window& window;
	std::vector<std::unique_ptr<Card>> cards;
};

