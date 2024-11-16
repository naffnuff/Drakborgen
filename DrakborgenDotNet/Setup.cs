using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Drakborgen
{
    internal class Setup
    {
        static private string? _mediaPath;

        internal static string MediaPath
        {
            get
            {
                if (_mediaPath == null)
                {
                    _mediaPath = Environment.CurrentDirectory;
                    while (_mediaPath != null && !Directory.Exists(Path.Combine(_mediaPath, "Media")))
                    {
                        _mediaPath = Directory.GetParent(_mediaPath)?.FullName;
                        Console.WriteLine(_mediaPath);
                    }
                    if (_mediaPath == null)
                    {
                        throw new Exception("No Media path found");
                    }
                    else
                    {
                        _mediaPath = Path.Combine(_mediaPath, "Media/");
                    }
                    Console.WriteLine(_mediaPath);
                }
                return _mediaPath;
            }
        }

        internal static void SetUpGame(Game game)
        {
            CreateTiles(game.Tiles);
        }

        static void CreateTile<TileType>(Deck<Tile> tiles, string number, List<Direction>? exits = null) where TileType : Tile
        {
            tiles.CreateItem<TileType>(Path.Combine(MediaPath, "rumsbrickor/rumsbricka0" + number + ".png"), exits);
        }

        internal static void CreateTiles(Deck<Tile> tiles)
        {
            CreateTile<Room>(tiles, "000", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "001", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "002", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Corridor>(tiles, "003", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Corridor>(tiles, "004", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "005", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "006", new List<Direction> { Direction.East, Direction.South });
            CreateTile<Room>(tiles, "007", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Corridor>(tiles, "008", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "009", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "010", new List<Direction> { Direction.East, Direction.South });
            CreateTile<Room>(tiles, "011", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "012", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Room>(tiles, "013", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "014", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "015", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "016", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "017", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "018", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "019", new List<Direction> { Direction.South });
            CreateTile<Room>(tiles, "020", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Corridor>(tiles, "021", new List<Direction> { Direction.South, Direction.East });
            CreateTile<Room>(tiles, "022", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "023", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "024", new List<Direction> { Direction.South });
            CreateTile<Room>(tiles, "025", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "026", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "027", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Corridor>(tiles, "028", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "029", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "030", new List<Direction> { Direction.East, Direction.South });
            CreateTile<Room>(tiles, "031", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "032", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Room>(tiles, "033", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "034", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "035", new List<Direction> { Direction.East, Direction.South });
            CreateTile<Corridor>(tiles, "036", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "037", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "038", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "039", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Corridor>(tiles, "040", new List<Direction> { Direction.East, Direction.South });
            CreateTile<SplitRoom>(tiles, "041");
            CreateTile<Room>(tiles, "042", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Room>(tiles, "043", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "044", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "045", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "046", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Room>(tiles, "047", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "048", new List<Direction> { Direction.East, Direction.South });
            CreateTile<Room>(tiles, "049", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "050", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Corridor>(tiles, "051", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Corridor>(tiles, "052", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "053", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "054", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "055", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "056", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "057", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "058", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "059", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "060", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Corridor>(tiles, "061", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "062", new List<Direction> { Direction.South });
            CreateTile<Room>(tiles, "063", new List<Direction> { Direction.North });
            CreateTile<Room>(tiles, "064", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "065", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "066", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "067", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "068", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "069", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "070", new List<Direction> { Direction.North });
            CreateTile<Room>(tiles, "071", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Room>(tiles, "072", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "073", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "074", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Corridor>(tiles, "075", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Corridor>(tiles, "076", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Room>(tiles, "077", new List<Direction> { Direction.South });
            CreateTile<Room>(tiles, "078", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "079", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "080", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "081", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Room>(tiles, "082", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "083", new List<Direction> { Direction.North });
            CreateTile<Room>(tiles, "084", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "085", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "086", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "087", new List<Direction> { Direction.North });
            CreateTile<Room>(tiles, "088", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Room>(tiles, "089", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "090", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "091", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "092", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "093", new List<Direction> { Direction.East, Direction.South });
            CreateTile<Corridor>(tiles, "094", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "095", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "096", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "097", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "098", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "099", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Corridor>(tiles, "100", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "101", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "102", new List<Direction> { Direction.North });
            CreateTile<Room>(tiles, "103", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "104", new List<Direction> { Direction.North, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "105", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "106", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "107", new List<Direction> { Direction.North, Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "108", new List<Direction> { Direction.North, Direction.East, Direction.South });
            CreateTile<Room>(tiles, "109", new List<Direction> { Direction.North, Direction.South });
            CreateTile<Room>(tiles, "110", new List<Direction> { Direction.South });
            CreateTile<Room>(tiles, "111", new List<Direction> { Direction.South, Direction.West });
            CreateTile<Room>(tiles, "112", new List<Direction> { Direction.East, Direction.South, Direction.West });
            CreateTile<Room>(tiles, "113", new List<Direction> { Direction.East, Direction.South });
            CreateTile<SplitRoom>(tiles, "114");

            if (!tiles.IsFull())
            {
                throw new Exception("Tile deck is not full after creation");
            }
        }
    }
}
