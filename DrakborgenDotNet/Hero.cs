using System.Xml.Linq;

namespace Drakborgen
{
    internal class Hero
    {
        internal Card? StatsCard { get; private set; }
        internal int MaxLife { get; init; }
        internal string Id { get; init; }
        internal string Name { get; init; }

        internal Hero(string id, string name, int maxLife)
        {
            Id = id;
	Name = name;

            MaxLife = maxLife;
            StatsCard = new Card(Setup.MediaPath + "hjaltekort/" + id + ".png");
        }

        internal Card PullStatsCard()
        {
	if (StatsCard == null)
	{
	    throw new Exception();
            }
            Card result = StatsCard;
            StatsCard = null;
	return result;
        }

        internal void PlaceStatsCard(Card card)
        {
            if (StatsCard != null)
            {
                throw new Exception();
            }
            StatsCard = card;
        }
    }
}