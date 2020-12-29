#pragma once

#include "Card.h"

class Hero
{
public:
	Hero(const std::string& id, const std::wstring& name, int maxLife);
	
	Hero(const Hero&) = delete;
	Hero& operator=(const Hero&) = delete;
	
	Hero(Hero&& other) noexcept;
	Hero& operator=(Hero&& other) noexcept;

	std::string getId() const { return id; }
	std::wstring getName() const { return name; }
	int getMaxLife() const { return maxLife; }

	std::unique_ptr<Card> pullStatsCard();
	void placeStatsCard(std::unique_ptr<Card> card);

private:
	std::string id;
	std::wstring name;
	int maxLife = 0;
	std::unique_ptr<Card> statCard;
};

