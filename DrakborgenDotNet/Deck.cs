using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Drakborgen
{
    class Deck<T>
    {
        private int _capacity;
        private List<T> _items;
        private Random _random;

        internal Deck(int capacity, System.Random random)
        {
            _capacity = capacity;
            _random = random;
            _items = new List<T>(capacity);
        }

        void shuffle()
        {
            _random.Shuffle(items);
        }

        internal void CreateItem<ItemType>(string imagePath, List<Direction>? exits) where ItemType : class, T
        {
            ItemType? item = Activator.CreateInstance(typeof(ItemType), imagePath, exits) as ItemType;
            if (item == null)
            {
                throw new Exception();
            }
            _items.Add(item);
            if (_items.Count > _capacity)
            {
                throw new Exception();
            }
        }

        bool isEmpty() const
	{
		return items.size() == 0;
	}

    internal bool IsFull()
        {
            return _items.Count == _capacity;
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
    }
}
