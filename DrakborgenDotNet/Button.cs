using SFML.Graphics;
using SFML.System;
using static System.Net.Mime.MediaTypeNames;
using System.Drawing;
using Font = SFML.Graphics.Font;
using Text = SFML.Graphics.Text;
using Color = SFML.Graphics.Color;
using System.Text;

namespace Drakborgen
{
    internal class Button : Drawable
    {
        private string ButtonText
        {
	get
	{
	    return _text.DisplayedString;
	}
	set
	{
	    _text.DisplayedString = value;
	}
        }

        private Text _text;
        private Font _font;
        private RectangleShape _background;

        Button(string message, Vector2f size, Vector2f position, uint textSize)
        {
	_font = new Font(Setup.MediaPath + "ITCBLKAD.TTF");

	_background.FillColor = Color.Black;
	_background.Position = position;
	_background.Size = size;

	// select the font
	_text.Font = _font; // font is a sf::Font

	// set the string to display
	ButtonText = message;

	// set the character size
	_text.CharacterSize = textSize; // in pixels, not points!

	// set the color
	_text.FillColor = Color.Red;

	// set the text style
	_text.Style = Text.Styles.Bold;

	Vector2f globalTextSize = _text.GetGlobalBounds().Size;
	_text.Position = new Vector2f(position.X + size.X / 2.0f - globalTextSize.X / 2.0f, position.Y + size.Y / 2.0f - 45.0f);
        }

        bool HitTest(Vector2f point)
        {
	FloatRect buttonRectangle = _background.GetGlobalBounds();
	return buttonRectangle.Contains(point);
        }

        void Drawable.Draw(RenderTarget target, RenderStates states)
        {
	target.Draw(_background, states);
	target.Draw(_text, states);
        }
    }
}