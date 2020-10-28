#include "Hero.h"

Hero::Hero(const std::string& id, const std::string& name, const std::string& imagePath)
	: id(id)
	, name(name)
	, startsCard(std::make_unique<Card>(imagePath))
{
}

std::unique_ptr<Card> Hero::pullStatsCard()
{
	return std::move(startsCard);
}

void Hero::placeStatsCard(std::unique_ptr<Card> card)
{
	startsCard = std::move(card);
}
