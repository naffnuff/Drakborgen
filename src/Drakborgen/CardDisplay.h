#pragma once

#include "Button.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <functional>

class Card;
class Engine;

class CardDisplay : public sf::Drawable, public sf::Transformable
{
public:
	CardDisplay(Engine& engine);
	
	CardDisplay(const CardDisplay&) = delete;
	CardDisplay& operator=(const CardDisplay&) = delete;
	
	int cardCount() const { return int(cards.size()); }

	void pushCard(std::unique_ptr<Card> card, std::function<void()> buttonCallback);
	std::unique_ptr<Card> pullCard(int index = 0);

	std::vector<sf::Vector2f> getLayout(const std::vector<std::unique_ptr<Card>>& cards);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Engine& engine;
	std::vector<std::unique_ptr<Card>> cards;
	std::vector<std::unique_ptr<Button>> cardButtons;
};

