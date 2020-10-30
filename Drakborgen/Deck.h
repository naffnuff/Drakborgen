#pragma once

#include <memory>
#include <vector>

#include "Random.h"

template<typename T, int Capacity>
class Deck
{
public:
	Deck()
	{
		items.reserve(Capacity);
	}
	
	Deck(Card&) = delete;
	Deck& operator=(Deck&) = delete;

	void shuffle()
	{
		random.shuffle(items);
	}

	template<typename... Args>
	void createItem(Args... args)
	{
		items.push_back(std::make_unique<T>(args...));
		if (items.size() > Capacity)
		{
			throw std::logic_error("Deck size exceeds capacity");
		}
	}

	bool isEmpty() const
	{
		return items.size() == 0;
	}

	bool isFull() const
	{
		return items.size() == Capacity;
	}

	std::unique_ptr<T> pullNextItem()
	{
		if (items.size() == 0)
		{
			throw std::logic_error("No items in deck");
		}
		std::unique_ptr<T> item = std::move(items.back());
		items.pop_back();
		return item;
	}

private:
	Random random;
	std::vector<std::unique_ptr<T>> items;
};

