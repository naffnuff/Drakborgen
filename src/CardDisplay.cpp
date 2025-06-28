#include "CardDisplay.h"

#include "System.h"

CardDisplay::CardDisplay(sf::Window& window)
	: window(window)
{
}

int CardDisplay::hitTest(sf::Vector2f mousePosition) const
{
	for (int i = 0; i < cards.size(); ++i)
	{
		if (cards[i]->getGlobalBounds().contains(mousePosition))
		{
			return i;
		}
	}
	return -1;
}

void CardDisplay::pushCard(std::unique_ptr<Card> card)
{
	cards.push_back(std::move(card));
}

std::unique_ptr<Card> CardDisplay::pullCard(int index)
{
	std::unique_ptr<Card> card = std::move(cards[index]);
	cards.erase(cards.begin() + index);
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

std::vector<sf::Vector2f> CardDisplay::getLayout(const std::vector<std::unique_ptr<Card>>& laidOutCards)
{
	std::vector<sf::Vector2f> layout(laidOutCards.size());
	sf::Vector2u windowSize = window.getSize();
	std::vector<std::pair<float, float>> quarters;
	if (layout.size() == 1)
	{
		quarters = { { 1.0f, 1.0f } };
	}
	else if (layout.size() == 2)
	{
		quarters = { { 1.0f, 1.0f }, { 3.0f, 1.0f } };
	}
	else if (layout.size() == 3)
	{
		quarters = { { 1.0f, 1.0f }, { 3.0f, 1.0f }, { 1.0f, 3.0f } };
	}
	else if (layout.size() == 4)
	{
		quarters = { { 1.0f, 1.0f }, { 3.0f, 1.0f }, { 1.0f, 3.0f }, { 3.0f, 3.0f } };
	}
	if (quarters.size() != layout.size())
	{
		THROW;
	}
	for (int i = 0; i < quarters.size(); ++i)
	{
		layout[i] = { windowSize.x * quarters[i].first / 4.0f, windowSize.y * quarters[i].second / 4.0f };
		layout[i].x -= laidOutCards[i]->getGlobalBounds().width / 2.0f;
		layout[i].y -= laidOutCards[i]->getGlobalBounds().height / 2.0f;
	}
	return layout;
}
