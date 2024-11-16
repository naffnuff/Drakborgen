using SFML.Graphics;
using SFML.System;
using static SFML.Window.Joystick;
using System;

namespace Drakborgen
{
    internal enum Direction
    {
        Invalid = -1,
        North = 0,
        East = 1,
        South = 2,
        West = 3,
        Count = 4
    }

    internal abstract class Tile : Transformable, Drawable
    {
        private string? _imagePath;
        private Sprite? _sprite;
        private Direction _orientation = Direction.North;

        internal Tile(string? imagePath)
        {
            if (imagePath != null)
            {
                _imagePath = imagePath;
                _sprite = new Sprite(new Texture(imagePath));
            }
        }

        internal abstract List<List<Direction>> GetExits(Direction entrance);

        internal abstract State Enter();

        internal bool CanDraw()
        {
            throw new NotImplementedException();
        }

        internal void SetOrientation(Direction direction)
        {
            _orientation = direction;
            if (_sprite != null)
            {
                Vector2f size = _sprite.GetGlobalBounds().Size;
                Rotation = (float)_orientation * 90.0f;
                switch (_orientation)
                {
                    case Direction.East:
                        Position += new Vector2f(size.X, 0.0f);
                        break;
                    case Direction.South:
                        Position += new Vector2f(size.X, size.Y);
                        break;
                    case Direction.West:
                        Position += new Vector2f(0.0f, size.Y);
                        break;
                    default:
                        break;
                }
            }
        }

        private Direction ApplyOrientation(Direction direction, bool invert)
        {
            return (Direction)(((int)direction + (invert ? -(int)_orientation : (int)_orientation)) % (int)Direction.Count);
        }

        private void TransposeExits(List<List<Direction>> exits)
        {
            foreach (List<Direction> exit in exits)
            {
                for (int i = 0; i < exit.Count; ++i)
                {
                    exit[i] = ApplyOrientation(exit[i], false);
                }
            }
            Console.WriteLine(_imagePath);
        }

        void Drawable.Draw(RenderTarget target, RenderStates states)
        {
            if (_sprite != null)
            {
                states.Transform *= Transform;
                target.Draw(_sprite, states);
            }
        }
    }
}