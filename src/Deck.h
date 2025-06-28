#pragma once

#include <memory>
#include <vector>

#include "System.h"
#include "Random.h"

template<typename T, int Capacity>
class Deck
{
public:
	Deck(Random& random)
		: random(random)
	{
		items.reserve(Capacity);
	}
	
	Deck(const Card&) = delete;
	Deck& operator=(const Deck&) = delete;

	void shuffle()
	{
		random.shuffle(items);
	}

	template<typename Subtype, typename... Args>
	void createItem(Args&&... args)
	{
		items.push_back(std::make_unique<Subtype>(std::forward<Args>(args)...));
		if (items.size() > Capacity)
		{
			THROW;
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
			THROW;
		}
		std::unique_ptr<T> item = std::move(items.front());
		items.erase(items.begin());
		//std::unique_ptr<T> item = std::move(items.back());
		//items.pop_back();
		return item;
	}

private:
	Random& random;
	std::vector<std::unique_ptr<T>> items;
};

