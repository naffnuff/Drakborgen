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
        private Window _window;
        private List<Card> _cards;

        internal CardDisplay(Window window)
        {
	_window = window;
            _cards = new List<Card>();
        }

        int HitTest(Vector2f mousePosition)
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

        void PushCard(Card card)
        {
	_cards.Add(card);
        }

        Card PullCard(int index)
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
        private record struct Size(float Width, float Height);

        List<Vector2f> GetLayout(List<Card> laidOutCards)
        {
	List<Vector2f> layout = new List<Vector2f>(laidOutCards.Count);
	Vector2u windowSize = _window.Size;
	List<Size>? quarters = null;
	if (layout.Count == 1)
	{
	    quarters = new List<Size>() { new Size(1.0f, 1.0f) };
	}
	else if (layout.Count == 2)
	{
	    quarters = new List<Size>() { new Size(1.0f, 1.0f), new Size(3.0f, 1.0f) };
	}
	else if (layout.Count == 3)
	{
	    quarters = new List<Size>() { new Size(1.0f, 1.0f), new Size(3.0f, 1.0f), new Size(1.0f, 3.0f) };
	}
	else if (layout.Count == 4)
	{
	    quarters = new List<Size>() { new Size(1.0f, 1.0f), new Size(3.0f, 1.0f), new Size(1.0f, 3.0f), new Size(3.0f, 3.0f) };
	}
	if (quarters == null || quarters.Count != layout.Count)
	{
	    throw new Exception();
	}
	for (int i = 0; i < quarters.Count; ++i)
	{
	    Vector2f newElement = new Vector2f(windowSize.X * quarters[i].Width / 4.0f, windowSize.Y * quarters[i].Height / 4.0f);
                layout.Add(newElement);
                newElement.X -= laidOutCards[i].GetGlobalBounds().Width / 2.0f;
                newElement.Y -= laidOutCards[i].GetGlobalBounds().Height / 2.0f;
	}
	return layout;
        }


    }
}