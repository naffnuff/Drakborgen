#include "CardDisplay.h"

CardDisplay::CardDisplay(sf::Window& window)
	: window(window)
{
}

int CardDisplay::hitTest(sf::Vector2i mousePosition) const
{
	for (int i = 0; i < cards.size(); ++i)
	{
		if (cards[i]->getBounds().contains(float(mousePosition.x), float(mousePosition.y)))
		{
			return i;
		}
	}
	return -1;
}

void CardDisplay::pushCard(std::unique_ptr<Card> card)
{
	cards.push_back(std::move(card));
	placeCards();
}

std::unique_ptr<Card> CardDisplay::pullCard(int index)
{
	std::unique_ptr<Card> card = std::move(cards[index]);
	cards.erase(cards.begin() + index);
	placeCards();
	return card;
}

void CardDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	for (const std::unique_ptr<Card>& card : cards)
	{
		target.draw(*card, states);
	}
}

void CardDisplay::placeCards()
{
	sf::Vector2u windowSize = window.getSize();
	if (cards.size() == 1)
	{
		cards.front().get()->centerAround({ windowSize.x / 2.0f, windowSize.y / 2.0f });
	}
	if (cards.size() == 4)
	{
		std::vector<std::pair<float, float>> quarters = { { 1.0f, 1.0f }, { 3.0f, 1.0f }, { 1.0f, 3.0f }, { 3.0f, 3.0f } };
		for(int i = 0; i < quarters.size(); ++i)
		{
			cards[i].get()->centerAround({ windowSize.x * quarters[i].first / 4.0f, windowSize.y * quarters[i].second / 4.0f });
		}
	}
}
