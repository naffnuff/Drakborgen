using Drakborgen;
using static SFML.Window.Joystick;

namespace Drakborgen
{
    internal class Room : Tile
    {
        private List<List<Direction>> _exits;

        public Room(string imagePaths, List<Direction> exits)
            : base(imagePaths)
        {
            _exits = new List<List<Direction>>(exits.Count);
            foreach (Direction exit in exits)
            {
                _exits.Add([exit]);
            }
        }

        internal override List<List<Direction>> GetExits(Direction entrance)
        {
            if (entrance == Direction.Invalid || entrance == Direction.Count)
            {
                throw new Exception();
            }
            List<List<Direction>> result = _exits.Select(innerList => innerList.ToList()).ToList();
            TransposeExits(result);
            return result;
        }

        internal override State Enter()
        {

            Console.WriteLine(" -> turn end");
            return State.TurnEnd;
        }
    }
}