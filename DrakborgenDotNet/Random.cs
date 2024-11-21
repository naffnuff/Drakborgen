using System.Collections.Generic;

namespace Drakborgen
{
    internal class Random
    {
        private System.Random _random;

        internal Random()
        {
            _random = new System.Random();
        }

        internal bool NextBool()
        {
            return _random.Next() % 2 == 0;
        }

        internal void Shuffle<T>(List<T> list)
        {
            for (int i = list.Count - 1; i > 0; i--)
            {
                int j = _random.Next(i + 1);
                // Swap elements i and j
                //(list[i], list[j]) = (list[j], list[i]);
            }
        }
    }
}