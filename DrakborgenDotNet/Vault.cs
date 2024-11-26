
namespace Drakborgen
{
    internal class Vault : Tile
    {
        private List<List<Direction>> _exits;

        internal Vault(List<List<Direction>> exits)
            : base(null)
        {
            _exits = exits;
        }

        internal static Tile MakeVault(Direction section)
        {
            switch (section)
            {
                case Direction.North:
                    return new Vault(
                        [
                            [Direction.North],
                            [Direction.East],
                            [Direction.West],
                            [Direction.South, Direction.East],
                            [Direction.South, Direction.South],
                            [Direction.South, Direction.West]
                        ]
                    );
                case Direction.South:
                    return new Vault(
                        [
                            [Direction.South],
                            [Direction.East],
                            [Direction.West],
                            [Direction.North, Direction.East],
                            [Direction.North, Direction.North],
                            [Direction.North, Direction.West]
                        ]
                    );
                default:
                    throw new Exception();
            }
        }

        internal override List<List<Direction>> GetExits(Direction direction)
        {
            return _exits;
        }

        internal override State Enter()
        {
            Console.WriteLine(" -> turn end");
            return State.TurnEnd;
        }
    }
}