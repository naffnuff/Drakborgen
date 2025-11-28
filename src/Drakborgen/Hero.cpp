#include "Hero.h"
#include "Setup.h"

Hero::Hero(const std::string& id, const std::string& name, int maxLife)
	: id(id)
	, name(name)
	, statsCard(std::make_unique<Card>(Setup::getMediaPath() + "hjaltekort/" + id + ".png"))
	, maxLife(maxLife)
{
}

Hero::Hero(Hero&& other) noexcept
{
	*this = std::move(other);
}

Hero& Hero::operator=(Hero&& other) noexcept
{
	id = std::move(other.id);
	name = std::move(other.name);
	statsCard = std::move(other.statsCard);
	maxLife = other.maxLife;
	return *this;
}

std::unique_ptr<Card> Hero::pullStatsCard()
{
	return std::move(statsCard);
}

void Hero::placeStatsCard(std::unique_ptr<Card> card)
{
	statsCard = std::move(card);
}
