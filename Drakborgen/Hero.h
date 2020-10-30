#pragma once

#include "Card.h"

class Hero
{
public:
	Hero(const std::string& id, const std::string& name, int maxLife);
	
	Hero(Hero&) = delete;
	Hero& operator=(Hero&) = delete;
	
	Hero(Hero&& other) noexcept;
	Hero& operator=(Hero&& other) noexcept;

	std::string getId() const { return id; }
	int getMaxLife() const { return maxLife; }

	std::unique_ptr<Card> pullStatsCard();
	void placeStatsCard(std::unique_ptr<Card> card);

private:
	std::string id;
	std::string name;
	int maxLife = 0;
	std::unique_ptr<Card> statCard;
};

