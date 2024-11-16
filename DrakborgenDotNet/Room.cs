namespace Drakborgen
{
    internal class Room : Tile
    {
        private List<List<Direction>> _exits;

        internal Room(string imagePaths, List<Direction> exits)
            : base(imagePaths)
        {
            _exits = new List<List<Direction>>(exits.Count);
            foreach (Direction exit in exits)
            {
                _exits.Add(new List<Direction>() { exit });
            }
        }
    }
}