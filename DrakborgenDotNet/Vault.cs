
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
                        new List<List<Direction>>()
                        {
                            new List<Direction>() { Direction.North },
                            new List<Direction>() { Direction.East },
                            new List<Direction>() { Direction.West },
                            new List<Direction>() { Direction.South, Direction.East },
                            new List<Direction>() { Direction.South, Direction.South },
                            new List<Direction>() { Direction.South, Direction.West }
                        }
                    );
                case Direction.South:
                    return new Vault(
                        new List<List<Direction>>()
                        {
                            new List<Direction>() { Direction.South },
                            new List<Direction>() { Direction.East },
                            new List<Direction>() { Direction.West },
                            new List<Direction>() { Direction.North, Direction.East },
                            new List<Direction>() { Direction.North, Direction.North },
                            new List<Direction>() { Direction.North, Direction.West }
                        }
                    );
                default:
                    throw new Exception();
            }
        }
    }
}