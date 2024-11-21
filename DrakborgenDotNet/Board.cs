using SFML.Graphics;
using SFML.System;
using System.Numerics;

namespace Drakborgen
{
    internal class Board : Transformable, Drawable
    {
        internal Vector2f Size
        {
            get
            {
                return _boardSprite.GetLocalBounds().Size;
            }
        }

        private const float GridOriginX = 418.0f;
        private const float GridOriginY = 119.0f;
        private const float TileSize = 178.0f;

        internal readonly record struct Site(int Row, int Column);

        internal readonly record struct MoveSite(Site Site, Direction Direction = Direction.Invalid)
        {
	internal MoveSite(int row, int column, Direction direction = Direction.Invalid)
	    : this(new Site(row, column), direction)
	{
	}
        }

        private record Player(Card Avatar)
        {
	internal Site Site { get; set; } = InvalidSite;
        }

        internal static readonly Site InvalidSite = new Site(-1, -1);
        internal static readonly MoveSite InvalidMoveSite = new MoveSite(InvalidSite);

        internal const int RowCount = 10;
        internal const int ColumnCount = 13;

        private Sprite _boardSprite = new Sprite(new Texture(Setup.MediaPath + "spelplan.jpg"));
        private Sprite _vaultSprite = new Sprite(new Texture(Setup.MediaPath + "skattkammaren.png"));
        private RectangleShape _clickOverlay = new RectangleShape(new Vector2f(TileSize, TileSize));
        private List<Player> _players = new List<Player>(4);

        private Tile[,] _tileGrid = new Tile[RowCount, ColumnCount];
        private List<MoveSite> _moveSites = new List<MoveSite>();
        private bool _moveSitesShown = false;
        private float _moveSiteAnimationStartTime = 0.0f;

        private AnimationManager _animations;

        internal Board(AnimationManager animations)
        {
	_animations = animations;
	_vaultSprite.Position = GetSitePosition(new Site(4, 6));
	_vaultSprite.Position += new Vector2f(0.0f, 10.0f);
	//players.reserve(4);

	_tileGrid[0, 0] = new Tower(new List<Direction>() { Direction.East, Direction.South });
	_tileGrid[0, ColumnCount - 1] = new Tower(new List<Direction>() { Direction.West, Direction.South });
	_tileGrid[RowCount - 1, 0] = new Tower(new List<Direction>() { Direction.East, Direction.North });
	_tileGrid[RowCount - 1, ColumnCount - 1] = new Tower(new List<Direction>() { Direction.West, Direction.North });
	_tileGrid[4, 6] = Vault.MakeVault(Direction.North);
	_tileGrid[5, 6] = Vault.MakeVault(Direction.South);
        }



        internal void Update(float elapsedTime, float timeDelta)
        {
	// one cycle per second
	if (_moveSiteAnimationStartTime == 0.0f)
	{
	    _moveSiteAnimationStartTime = elapsedTime;
	}
	int wholeSeconds = (int)(elapsedTime - _moveSiteAnimationStartTime);
	float fraction = elapsedTime - _moveSiteAnimationStartTime - wholeSeconds;
	if (wholeSeconds % 2 != 0)
	{
	    fraction = 1.0f - fraction;
	}
	_clickOverlay.FillColor = new Color((byte)(0x80 + 0x7f * fraction), (byte)(0x80 + 0x7f * fraction), (byte)(0x80 + 0x7f * fraction), (byte)(0xff * fraction));
        }

        private void PlaceTile(Tile tile, MoveSite moveSite)
        {
	if (HasTile(moveSite.Site) || !WithinBounds(moveSite.Site))
	{
	    throw new Exception();
	}
	tile.Position = GetSitePosition(moveSite.Site);
	tile.SetOrientation(moveSite.Direction);
	_tileGrid[moveSite.Site.Row, moveSite.Site.Column] = tile;
        }

        private Vector2f GetSitePosition(Site site)
        {
	return new Vector2f(GridOriginX + TileSize * site.Column, GridOriginY + TileSize * site.Row);
        }

        internal Site GetSite(Vector2f position)
        {
	return new Site((int)((position.Y - GridOriginY) / TileSize), (int)((position.X - GridOriginX) / TileSize));
        }

        internal Vector2f GetAvatarCenter(int index)
        {
	FloatRect bounds = _players[index].Avatar.GetGlobalBounds();
	return new Vector2f(bounds.Left + bounds.Width / 2.0f, bounds.Top + bounds.Height / 2.0f);
        }

        internal void SetGameStartMoveSites()
        {
	_moveSites = new List<MoveSite>() { new MoveSite(0, 0), new MoveSite(0, ColumnCount - 1), new MoveSite(RowCount - 1, 0), new MoveSite(RowCount - 1, ColumnCount - 1) };
	_moveSiteAnimationStartTime = 0.0f;
        }

        internal void SetPlayerMoveSites(MoveSite playerSite)
        {
	foreach (List<Direction> exits in GetTile(playerSite.Site).GetExits(playerSite.Direction))
	{
	    MoveSite moveSite = CreateMoveSite(playerSite.Site, exits);
	    if (WithinBounds(moveSite.Site))
	    {
	        _moveSites.Add(moveSite);
	    }
	}
	_moveSiteAnimationStartTime = 0.0f;
        }

        internal bool TestMoveSites(Vector2f position)
        {
	if (!_moveSitesShown)
	{
	    throw new Exception();
	}
	Site site = GetSite(position);
	foreach (MoveSite moveSite in _moveSites)
	{
	    if (site == moveSite.Site)
	    {
	        return true;
	    }
	}
	return false;
        }

        internal MoveSite GetMoveSite(Vector2f position)
        {
	if (!_moveSitesShown)
	{
	    throw new Exception();
	}
	Site site = GetSite(position);
	foreach (MoveSite moveSite in _moveSites)
	{
	    if (site == moveSite.Site)
	    {
	        return moveSite;
	    }
	}
	throw new Exception();
        }

        internal void RemoveMoveSite(Site site)
        {
	for (int i = 0; i < _moveSites.Count; i++)
	{
	    if (_moveSites[i].Site == site)
	    {
	        _moveSites.RemoveAt(i);
	        break;
	    }
	}
        }

        internal void ClearMoveSites()
        {
	_moveSites.Clear();
        }

        internal void ShowMoveSites(bool show)
        {
	_moveSitesShown = show;
	if (show)
	{
	    _moveSiteAnimationStartTime = 0.0f;
	}
        }

        internal void AddPlayer(string imagePath, int index)
        {
	if (_players.Count != index)
	{
	    throw new Exception();
	}
	_players.Add(new Player(new Card(imagePath)));
	Player player = _players.Last();
	player.Avatar.Scale = new Vector2f(1.0f / 3.0f, 1.0f / 3.0f);
        }

        internal void SetPlayerSite(int index, MoveSite moveSite, Action animationCallback)
        {
	if (_players.Count <= index)
	{
	    throw new Exception();
	}
	_players[index].Site = moveSite.Site;
	PlacePlayer(index, moveSite.Direction, animationCallback);
        }

        void Drawable.Draw(RenderTarget target, RenderStates states)
        {
	states.Transform *= Transform;
	target.Draw(_boardSprite, states);
	target.Draw(_vaultSprite, states);
	foreach (Tile tile in _tileGrid)
	{
	    if (tile.CanDraw())
	    {
	        target.Draw(tile, states);
	    }
	}
	if (_moveSitesShown)
	{
	    foreach (MoveSite moveSite in _moveSites)
	    {
	        Transform transform = states.Transform;
	        Vector2f offset = GetSitePosition(moveSite.Site);
	        states.Transform.Translate(offset);
	        target.Draw(_clickOverlay, states);
	        states.Transform = transform;
	    }
	}
	foreach (Player player in _players)
	{
	    target.Draw(player.Avatar, states);
	}
        }

        internal bool HasTile(Site site)
        {
	return _tileGrid[site.Row, site.Column] != null;
        }

        internal Tile GetTile(Site site)
        {
	if (!HasTile(site))
	{
	    throw new Exception();
	}
	return _tileGrid[site.Row, site.Column];
        }

        private bool WithinBounds(Site site)
        {
	return site.Row >= 0 && site.Row < RowCount && site.Column >= 0 && site.Column < ColumnCount;
        }

        private void PlacePlayer(int index, Direction direction, Action animationCallback)
{
	Player player = _players[index];
	Vector2f sitePosition = GetSitePosition(player.Site);
	Vector2f avatarSize = player.Avatar.GetGlobalBounds().Size;
	Vector2f animationTarget = new Vector2f(sitePosition.X + TileSize / 2.0f - avatarSize.X / 2.0f, sitePosition.Y + TileSize / 2.0f - avatarSize.Y / 2.0f);
	if (direction == Direction.Invalid)
	{
	    float avatarStartX = animationTarget.X + (player.Site.Column == 0 ? -TileSize : TileSize) * 2.0f;
	    float avatarStartY = animationTarget.Y + (player.Site.Row == 0 ? -TileSize : TileSize);
	    player.Avatar.Position = new Vector2f(avatarStartX, avatarStartY);
	}
	_animations.Add(player.Avatar, animationTarget, 0.5f, animationCallback);
}

        private MoveSite CreateMoveSite(Site site, List<Direction> exits)
        {
	int row = site.Row;
	int column = site.Column;

	foreach (Direction exit in exits)
	{
	    switch (exit)
	    {
	        case Direction.North:
		--row;
		break;
	        case Direction.South:
		++row;
		break;
	        case Direction.West:
		--column;
		break;
	        case Direction.East:
		++column;
		break;
	        default:
		throw new Exception();
	    }
	}

	return new MoveSite(row, column, exits.Last());
        }
    }
}