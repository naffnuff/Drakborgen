#include "Setup.h"

#include "System.h"
#include "Room.h"
#include "Corridor.h"
#include "SplitRoom.h"

std::optional<std::filesystem::path> Setup::mediaPath;


std::string Setup::getMediaPath()
{
	if (!mediaPath.has_value())
	{
		std::filesystem::path currentPath = std::filesystem::current_path();

		while (!currentPath.empty())
		{
			std::filesystem::path assetsPath = currentPath / "DrakborgenAssets/";
			if (std::filesystem::exists(assetsPath) && std::filesystem::is_directory(assetsPath))
			{
				mediaPath = assetsPath;
				break;
			}

			std::filesystem::path parent = currentPath.parent_path();
			if (parent == currentPath)
			{
				break;
			}

			currentPath = parent;
		}

		if (!mediaPath.has_value())
		{
			THROW;
		}
	}

	return mediaPath->string();
}

void Setup::setUpGame(Game& game)
{
	TileDeck& tiles = game.getTiles();
	createTiles(tiles);
}

void Setup::createTiles(TileDeck& tiles)
{
	createTile<Room>(tiles, "000", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "001", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "002", Direction::North, Direction::South);
	createTile<Corridor>(tiles, "003", Direction::East, Direction::South, Direction::West);
	createTile<Corridor>(tiles, "004", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "005", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "006", Direction::East, Direction::South);
	createTile<Room>(tiles, "007", Direction::South, Direction::West);
	createTile<Corridor>(tiles, "008", Direction::North, Direction::South);
	createTile<Room>(tiles, "009", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "010", Direction::East, Direction::South);
	createTile<Room>(tiles, "011", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "012", Direction::South, Direction::West);
	createTile<Room>(tiles, "013", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "014", Direction::North, Direction::South);
	createTile<Room>(tiles, "015", Direction::North, Direction::South);
	createTile<Room>(tiles, "016", Direction::North, Direction::South);
	createTile<Room>(tiles, "017", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "018", Direction::North, Direction::South);
	createTile<Room>(tiles, "019", Direction::South);
	createTile<Room>(tiles, "020", Direction::North, Direction::South, Direction::West);
	createTile<Corridor>(tiles, "021", Direction::South, Direction::East);
	createTile<Room>(tiles, "022", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "023", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "024", Direction::South);
	createTile<Room>(tiles, "025", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "026", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "027", Direction::North, Direction::South);
	createTile<Corridor>(tiles, "028", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "029", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "030", Direction::East, Direction::South);
	createTile<Room>(tiles, "031", Direction::North, Direction::South);
	createTile<Room>(tiles, "032", Direction::South, Direction::West);
	createTile<Room>(tiles, "033", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "034", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "035", Direction::East, Direction::South);
	createTile<Corridor>(tiles, "036", Direction::North, Direction::South);
	createTile<Room>(tiles, "037", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "038", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "039", Direction::East, Direction::South, Direction::West);
	createTile<Corridor>(tiles, "040", Direction::East, Direction::South);
	createTile<SplitRoom>(tiles, "041");
	createTile<Room>(tiles, "042", Direction::South, Direction::West);
	createTile<Room>(tiles, "043", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "044", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "045", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "046", Direction::South, Direction::West);
	createTile<Room>(tiles, "047", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "048", Direction::East, Direction::South);
	createTile<Room>(tiles, "049", Direction::North, Direction::South);
	createTile<Room>(tiles, "050", Direction::North, Direction::East, Direction::South);
	createTile<Corridor>(tiles, "051", Direction::South, Direction::West);
	createTile<Corridor>(tiles, "052", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "053", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "054", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "055", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "056", Direction::North, Direction::South);
	createTile<Room>(tiles, "057", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "058", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "059", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "060", Direction::North, Direction::East, Direction::South);
	createTile<Corridor>(tiles, "061", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "062", Direction::South);
	createTile<Room>(tiles, "063", Direction::North);
	createTile<Room>(tiles, "064", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "065", Direction::North, Direction::South);
	createTile<Room>(tiles, "066", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "067", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "068", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "069", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "070", Direction::North);
	createTile<Room>(tiles, "071", Direction::South, Direction::West);
	createTile<Room>(tiles, "072", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "073", Direction::North, Direction::South);
	createTile<Room>(tiles, "074", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Corridor>(tiles, "075", Direction::North, Direction::South, Direction::West);
	createTile<Corridor>(tiles, "076", Direction::South, Direction::West);
	createTile<Room>(tiles, "077", Direction::South);
	createTile<Room>(tiles, "078", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "079", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "080", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "081", Direction::South, Direction::West);
	createTile<Room>(tiles, "082", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "083", Direction::North);
	createTile<Room>(tiles, "084", Direction::North, Direction::South);
	createTile<Room>(tiles, "085", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "086", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "087", Direction::North);
	createTile<Room>(tiles, "088", Direction::South, Direction::West);
	createTile<Room>(tiles, "089", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "090", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "091", Direction::North, Direction::South);
	createTile<Room>(tiles, "092", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "093", Direction::East, Direction::South);
	createTile<Corridor>(tiles, "094", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "095", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "096", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "097", Direction::North, Direction::South);
	createTile<Room>(tiles, "098", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "099", Direction::North, Direction::South);
	createTile<Corridor>(tiles, "100", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "101", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "102", Direction::North);
	createTile<Room>(tiles, "103", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "104", Direction::North, Direction::South, Direction::West);
	createTile<Room>(tiles, "105", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "106", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "107", Direction::North, Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "108", Direction::North, Direction::East, Direction::South);
	createTile<Room>(tiles, "109", Direction::North, Direction::South);
	createTile<Room>(tiles, "110", Direction::South);
	createTile<Room>(tiles, "111", Direction::South, Direction::West);
	createTile<Room>(tiles, "112", Direction::East, Direction::South, Direction::West);
	createTile<Room>(tiles, "113", Direction::East, Direction::South);
	createTile<SplitRoom>(tiles, "114");
	if (!tiles.isFull())
	{
		THROW;
	}
}
