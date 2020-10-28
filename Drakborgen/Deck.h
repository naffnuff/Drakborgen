#pragma once

#include <memory>
#include <vector>

#include "Random.h"

template<typename T>
class Deck
{
public:
	void shuffle()
	{
		random.shuffle(items);
	}

	template<typename... Args>
	void createItem(Args... args)
	{
		items.push_back(std::make_unique<T>(args...));
	}

	bool isEmpty() const
	{
		return items.size() == 0;
	}

	std::unique_ptr<T> pullNextItem()
	{
		if (items.size() == 0)
		{
			throw std::runtime_error("No items in deck");
		}
		std::unique_ptr<T> item = std::move(items.back());
		items.pop_back();
		return item;
	}

private:
	Random random;
	std::vector<std::unique_ptr<T>> items;
};

