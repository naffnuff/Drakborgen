#include "SplitRoom.h"

std::vector<std::vector<Direction>> SplitRoom::getExits(Direction entrance) const
{
	Direction relativeEntrance = applyOrientation(entrance, true);
	std::vector<std::vector<Direction>> result;
	if (relativeEntrance == Direction::North || relativeEntrance == Direction::East)
	{
		result = southSection;
	}
	else
	{
		result = northSection;
	}
	transposeExits(result);
	return result;
}
