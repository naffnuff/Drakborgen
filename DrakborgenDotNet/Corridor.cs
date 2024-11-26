namespace Drakborgen
{
    internal class Corridor : Room
    {
        public Corridor(string imagePaths, List<Direction> exits)
            : base(imagePaths, exits)
        {
        }

        internal override State Enter()
        {
            Console.WriteLine(" -> turn continue");
            return State.TurnContinue;
        }
    }
}