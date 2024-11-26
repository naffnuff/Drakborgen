namespace Drakborgen
{
    internal class SplitRoom : Room
    {
        private readonly List<List<Direction>> northSection = [[Direction.North], [Direction.East]];
        private readonly List<List<Direction>> southSection = [[Direction.South], [Direction.West]];

        public SplitRoom(string imagePaths)
	: base(imagePaths, new List<Direction>())
        {
        }

        public SplitRoom(string imagePaths, List<Direction> exits)
	: this(imagePaths)
        {
        }

        internal override List<List<Direction>> GetExits(Direction entrance)
        {
	Direction relativeEntrance = ApplyOrientation(entrance, true);
	List<List<Direction>> result;
	if (relativeEntrance == Direction.North || relativeEntrance == Direction.East)
	{
	    result = southSection;
	}
	else
	{
	    result = northSection;
	}
	TransposeExits(result);
	return result;
        }
    }
}