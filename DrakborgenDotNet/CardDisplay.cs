using Drakborgen;
using SFML.Graphics;
using SFML.Window;
using System.Collections.Generic;
using System;
using SFML.System;

namespace Drakborgen
{
    internal class CardDisplay : Transformable, Drawable
    {
        internal int CardCount { get { return _cards.Count; }}

        private Window _window;
        private List<Card> _cards;

        internal CardDisplay(Window window)
        {
	_window = window;
            _cards = new List<Card>();
        }

        internal int HitTest(Vector2f mousePosition)
        {
	for (int i = 0; i < _cards.Count; ++i)
	{
	    if (_cards[i].GetGlobalBounds().Contains(mousePosition))
	    {
	        return i;
	    }
	}
	return -1;
        }

        internal void PushCard(Card card)
        {
	_cards.Add(card);
        }

        internal Card PullCard(int index = 0)
        {
	Card card = _cards[index];
	_cards.RemoveAt(index);
	return card;
        }

        void Drawable.Draw(RenderTarget target, RenderStates states)
        {
	states.Transform *= Transform;
	foreach (Card card in _cards)
	{
	    target.Draw(card, states);
	}
        }

        private readonly record struct Size(float Width, float Height);

        internal Vector2f[] GetLayout(List<Card> laidOutCards)
        {
	Vector2f[] layout = new Vector2f[laidOutCards.Count];
	Vector2u windowSize = _window.Size;
	Size[]? quarters = null;
	if (layout.Length == 1)
	{
	    quarters = [new Size(1.0f, 1.0f)];
	}
	else if (layout.Length == 2)
	{
	    quarters = [new Size(1.0f, 1.0f), new Size(3.0f, 1.0f)];
	}
	else if (layout.Length == 3)
	{
	    quarters = [new Size(1.0f, 1.0f), new Size(3.0f, 1.0f), new Size(1.0f, 3.0f)];
	}
	else if (layout.Length == 4)
	{
	    quarters = [new Size(1.0f, 1.0f), new Size(3.0f, 1.0f), new Size(1.0f, 3.0f), new Size(3.0f, 3.0f)];
	}
	else
	{
	    throw new Exception();
	}
	for (int i = 0; i < quarters.Length; ++i)
	{
	    Vector2f newElement = new Vector2f(windowSize.X * quarters[i].Width / 4.0f, windowSize.Y * quarters[i].Height / 4.0f);
                newElement.X -= laidOutCards[i].GetGlobalBounds().Width / 2.0f;
                newElement.Y -= laidOutCards[i].GetGlobalBounds().Height / 2.0f;
	    layout[i] = newElement;
            }
	return layout;
        }


    }
}