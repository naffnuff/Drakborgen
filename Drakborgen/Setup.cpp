#include "Setup.h"

void Setup::setUpGame(Game& game)
{
	Deck<Tile>& tiles = game.getTiles();
	createTiles(tiles);
}

void Setup::createTiles( Deck<Tile>& tiles )
{
	createTile(tiles, "000");
	createTile(tiles, "001");
	createTile(tiles, "002");
	createTile(tiles, "003");
	createTile(tiles, "004");
	createTile(tiles, "005");
	createTile(tiles, "006");
	createTile(tiles, "007");
	createTile(tiles, "008");
	createTile(tiles, "009");
	createTile(tiles, "010");
	createTile(tiles, "011");
	createTile(tiles, "012");
	createTile(tiles, "013");
	createTile(tiles, "014");
	createTile(tiles, "015");
	createTile(tiles, "016");
	createTile(tiles, "017");
	createTile(tiles, "018");
	createTile(tiles, "019");
	createTile(tiles, "020");
	createTile(tiles, "021");
	createTile(tiles, "022");
	createTile(tiles, "023");
	createTile(tiles, "024");
	createTile(tiles, "025");
	createTile(tiles, "026");
	createTile(tiles, "027");
	createTile(tiles, "028");
	createTile(tiles, "029");
	createTile(tiles, "030");
	createTile(tiles, "031");
	createTile(tiles, "032");
	createTile(tiles, "033");
	createTile(tiles, "034");
	createTile(tiles, "035");
	createTile(tiles, "036");
	createTile(tiles, "037");
	createTile(tiles, "038");
	createTile(tiles, "039");
	createTile(tiles, "040");
	createTile(tiles, "041");
	createTile(tiles, "042");
	createTile(tiles, "043");
	createTile(tiles, "044");
	createTile(tiles, "045");
	createTile(tiles, "046");
	createTile(tiles, "047");
	createTile(tiles, "048");
	createTile(tiles, "049");
	createTile(tiles, "050");
	createTile(tiles, "051");
	createTile(tiles, "052");
	createTile(tiles, "053");
	createTile(tiles, "054");
	createTile(tiles, "055");
	createTile(tiles, "056");
	createTile(tiles, "057");
	createTile(tiles, "058");
	createTile(tiles, "059");
	createTile(tiles, "060");
	createTile(tiles, "061");
	createTile(tiles, "062");
	createTile(tiles, "063");
	createTile(tiles, "064");
	createTile(tiles, "065");
	createTile(tiles, "066");
	createTile(tiles, "067");
	createTile(tiles, "068");
	createTile(tiles, "069");
	createTile(tiles, "070");
	createTile(tiles, "071");
	createTile(tiles, "072");
	createTile(tiles, "073");
	createTile(tiles, "074");
	createTile(tiles, "075");
	createTile(tiles, "076");
	createTile(tiles, "077");
	createTile(tiles, "078");
	createTile(tiles, "079");
	createTile(tiles, "080");
	createTile(tiles, "081");
	createTile(tiles, "082");
	createTile(tiles, "083");
	createTile(tiles, "084");
	createTile(tiles, "085");
	createTile(tiles, "086");
	createTile(tiles, "087");
	createTile(tiles, "088");
	createTile(tiles, "089");
	createTile(tiles, "090");
	createTile(tiles, "091");
	createTile(tiles, "092");
	createTile(tiles, "093");
	createTile(tiles, "094");
	createTile(tiles, "095");
	createTile(tiles, "096");
	createTile(tiles, "097");
	createTile(tiles, "098");
	createTile(tiles, "099");
	createTile(tiles, "100");
	createTile(tiles, "101");
	createTile(tiles, "102");
	createTile(tiles, "103");
	createTile(tiles, "104");
	createTile(tiles, "105");
	createTile(tiles, "106");
	createTile(tiles, "107");
	createTile(tiles, "108");
	createTile(tiles, "109");
	createTile(tiles, "110");
	createTile(tiles, "111");
	createTile(tiles, "112");
	createTile(tiles, "113");
	createTile(tiles, "114");
}

void Setup::createTile(Deck<Tile>& tiles, const std::string& number)
{
	tiles.createItem("Media/rumsbrickor/rumsbricka0" + number + ".png");
}
