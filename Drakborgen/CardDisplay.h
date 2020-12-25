#pragma once

#include <vector>

#include "UniqueSprite.h"

#include "Card.h"

class CardDisplay : public sf::Drawable, public sf::Transformable
{
public:
	CardDisplay(sf::Window& window);
	
	CardDisplay(const CardDisplay&) = delete;
	CardDisplay& operator=(const CardDisplay&) = delete;
	
	bool empty() const { return cards.empty(); }

	int hitTest(sf::Vector2i mousePosition) const;

	void pushCard(std::unique_ptr<Card> card);
	std::unique_ptr<Card> pullCard(int index = 0);

	std::vector<sf::Vector2f> getLayout(const std::vector<std::unique_ptr<Card>>& cards);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Window& window;
	std::vector<std::unique_ptr<Card>> cards;
};

