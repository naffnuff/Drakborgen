namespace Drakborgen
{
    internal class Tower : Tile
    {
        private List<List<Direction>> _exits;

        internal Tower(List<Direction> exits)
            : base(null)
        {
            _exits = new List<List<Direction>>(exits.Count);
            foreach (Direction exit in exits)
            {
                _exits.Add([exit]);
            }
        }

        internal override List<List<Direction>> GetExits(Direction direction)
        {
            return _exits;
        }

        internal override State Enter()
        {
            Console.WriteLine(" -> turn continue");
            return State.TurnContinue;
        }
    }
}