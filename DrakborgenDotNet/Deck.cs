using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Drakborgen
{
    class Deck<T>
    {
        private int _capacity;
        private List<T> _items;
        private Random _random;

        internal Deck(int capacity, Random random)
        {
            _capacity = capacity;
            _random = random;
            _items = new List<T>(capacity);
        }

        void shuffle()
        {
            _random.Shuffle(CollectionsMarshal.AsSpan(_items));
        }

        internal void CreateItem<ItemType>(string imagePath, List<Direction>? exits) where ItemType : class, T
        {
            if (Activator.CreateInstance(typeof(ItemType), [imagePath, exits]) is not ItemType item)
            {
                throw new Exception();
            }
            _items.Add(item);
            if (_items.Count > _capacity)
            {
                throw new Exception();
            }
        }

        private bool IsEmpty()
        {
            return _items.Count == 0;
        }

        internal bool IsFull()
        {
            return _items.Count == _capacity;
        }

        internal T PullNextItem()
        {
            if (IsEmpty())
            {
                throw new Exception();
            }
            T item = _items.First();
            _items.RemoveAt(0);
            //std::unique_ptr<T> item = std::move(items.back());
            //items.pop_back();
            return item;
        }
    }
}
