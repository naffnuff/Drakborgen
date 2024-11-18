using System.Xml.Linq;

namespace Drakborgen
{
    internal class Hero
    {
        internal Card? StatsCard { get; private set; }

        private string _id;
        private string _name;
        private int _maxLife = 0;

        Hero(string id, string name, int maxLife)
        {
            _id = id;
	_name = name;
	_maxLife = maxLife;

            StatsCard = new Card(Setup.MediaPath + "hjaltekort/" + id + ".png");
        }

        Card PullStatsCard()
        {
	if (StatsCard == null)
	{
	    throw new Exception();
            }
            Card result = StatsCard;
            StatsCard = null;
	return result;
        }

        void PlaceStatsCard(Card card)
        {
            if (StatsCard != null)
            {
                throw new Exception();
            }
            StatsCard = card;
        }
    }
}