#pragma once

#include "Card.h"

class Hero
{
public:
	Hero(const std::string& id, const std::string& name, int maxLife);
	
	Hero(const Hero&) = delete;
	Hero& operator=(const Hero&) = delete;
	
	Hero(Hero&& other) noexcept;
	Hero& operator=(Hero&& other) noexcept;

	std::string getId() const { return id; }
	std::string getName() const { return name; }
	int getMaxLife() const { return maxLife; }

	std::unique_ptr<Card> pullStatsCard();
	void placeStatsCard(std::unique_ptr<Card> card);

	std::unique_ptr<Card>& getStatsCard() { return statsCard; }

private:
	std::string id;
	std::string name;
	int maxLife = 0;
	std::unique_ptr<Card> statsCard;
};

