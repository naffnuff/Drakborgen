#include "Hero.h"

Hero::Hero(const std::string& id, const std::string& name)
	: id(id)
	, name(name)
	, statCard(std::make_unique<Card>("Media/hjaltekort/" + id + ".png"))
{
}

Hero::Hero(Hero&& other) noexcept
{
	*this = std::move(other);
}

void Hero::operator=( Hero&& other ) noexcept
{
	id = std::move(other.id);
	name = std::move(other.name);
	statCard = std::move(other.statCard);
}

std::unique_ptr<Card> Hero::pullStatsCard()
{
	return std::move(statCard);
}

void Hero::placeStatsCard(std::unique_ptr<Card> card)
{
	statCard = std::move(card);
}
