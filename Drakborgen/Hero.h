#pragma once

#include "Card.h"

class Hero
{
public:
	Hero(const std::string& id, const std::string& name, const std::string& imagePath);

	std::string getId() const { return id; }

	std::unique_ptr<Card> pullStatsCard();
	void placeStatsCard(std::unique_ptr<Card> card);

private:
	std::string id;
	std::string name;
	std::unique_ptr<Card> startsCard;
};

