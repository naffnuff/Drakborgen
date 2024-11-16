using SFML.Graphics;
using SFML.System;

namespace Drakborgen
{
    internal class Card : Transformable, Drawable
    {
        private Sprite _sprite;

        internal Card(string imagePath)
        {
            _sprite = new Sprite(new Texture(imagePath));
        }

        private void CenterAround(Vector2f position)
        {
            FloatRect bounds = GetGlobalBounds();
            Position = new Vector2f(position.X - (float)(bounds.Width) / 2.0f, position.Y - (float)(bounds.Height) / 2.0f);
        }

        internal FloatRect GetGlobalBounds()
        {
            return Transform.TransformRect(_sprite.GetLocalBounds());
        }

        void Drawable.Draw(RenderTarget target, RenderStates states)
        {
            states.Transform *= Transform;
            target.Draw(_sprite, states);
        }
    }
}