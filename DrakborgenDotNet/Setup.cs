﻿using System;
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
            CreateTile<Room>(tiles, "000", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "001", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "002", [Direction.North, Direction.South]);
            CreateTile<Corridor>(tiles, "003", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Corridor>(tiles, "004", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "005", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "006", [Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "007", [Direction.South, Direction.West]);
            CreateTile<Corridor>(tiles, "008", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "009", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "010", [Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "011", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "012", [Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "013", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "014", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "015", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "016", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "017", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "018", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "019", [Direction.South]);
            CreateTile<Room>(tiles, "020", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Corridor>(tiles, "021", [Direction.South, Direction.East]);
            CreateTile<Room>(tiles, "022", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "023", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "024", [Direction.South]);
            CreateTile<Room>(tiles, "025", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "026", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "027", [Direction.North, Direction.South]);
            CreateTile<Corridor>(tiles, "028", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "029", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "030", [Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "031", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "032", [Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "033", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "034", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "035", [Direction.East, Direction.South]);
            CreateTile<Corridor>(tiles, "036", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "037", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "038", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "039", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Corridor>(tiles, "040", [Direction.East, Direction.South]);
            CreateTile<SplitRoom>(tiles, "041");
            CreateTile<Room>(tiles, "042", [Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "043", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "044", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "045", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "046", [Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "047", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "048", [Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "049", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "050", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Corridor>(tiles, "051", [Direction.South, Direction.West]);
            CreateTile<Corridor>(tiles, "052", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "053", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "054", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "055", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "056", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "057", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "058", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "059", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "060", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Corridor>(tiles, "061", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "062", [Direction.South]);
            CreateTile<Room>(tiles, "063", [Direction.North]);
            CreateTile<Room>(tiles, "064", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "065", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "066", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "067", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "068", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "069", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "070", [Direction.North]);
            CreateTile<Room>(tiles, "071", [Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "072", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "073", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "074", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Corridor>(tiles, "075", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Corridor>(tiles, "076", [Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "077", [Direction.South]);
            CreateTile<Room>(tiles, "078", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "079", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "080", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "081", [Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "082", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "083", [Direction.North]);
            CreateTile<Room>(tiles, "084", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "085", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "086", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "087", [Direction.North]);
            CreateTile<Room>(tiles, "088", [Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "089", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "090", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "091", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "092", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "093", [Direction.East, Direction.South]);
            CreateTile<Corridor>(tiles, "094", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "095", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "096", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "097", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "098", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "099", [Direction.North, Direction.South]);
            CreateTile<Corridor>(tiles, "100", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "101", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "102", [Direction.North]);
            CreateTile<Room>(tiles, "103", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "104", [Direction.North, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "105", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "106", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "107", [Direction.North, Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "108", [Direction.North, Direction.East, Direction.South]);
            CreateTile<Room>(tiles, "109", [Direction.North, Direction.South]);
            CreateTile<Room>(tiles, "110", [Direction.South]);
            CreateTile<Room>(tiles, "111", [Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "112", [Direction.East, Direction.South, Direction.West]);
            CreateTile<Room>(tiles, "113", [Direction.East, Direction.South]);
            CreateTile<SplitRoom>(tiles, "114");

            if (!tiles.IsFull())
            {
                throw new Exception("Tile deck is not full after creation");
            }
        }
    }
}
