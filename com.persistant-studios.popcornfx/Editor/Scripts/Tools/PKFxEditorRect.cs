//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

namespace PopcornFX
{
	public struct PKFxEditorRect
	{
		private Rect m_Position;

		public PKFxEditorRect(float x, float y, float width, float height)
		{
			m_Position = new Rect(x, y, width, height);
		}

		public PKFxEditorRect(Rect position)
		{
			m_Position = position;
		}

		public void Shift(Vector2 offset)
		{
			m_Position.x += offset.x;
			m_Position.y += offset.y;
		}

		public void ShiftHorizontally(float x)
		{
			m_Position.x += x;
		}

		public void ShiftVertically(float y)
		{
			m_Position.y += y;
		}

		public void NextLine()
		{
			m_Position.y += EditorGUIUtility.singleLineHeight + EditorGUIUtility.standardVerticalSpacing;
		}

		public void SetHeightForOneLine()
		{
			m_Position.height = EditorGUIUtility.singleLineHeight;
		}

		public void AlignToRight(float width)
		{
			m_Position.x = m_Position.x + m_Position.width - width;
			m_Position.width = width;
		}

		public void AddPadding(float x, float y)
		{
			m_Position.x += x;
			m_Position.width -= x * 2;

			m_Position.y += y;
			m_Position.height -= y * 2;
		}

		public void IncreaseWidth(float amount)
		{
			m_Position.width += amount;
		}

		public void DecreaseWidth(float amount)
		{
			m_Position.width -= amount;
		}

		public PKFxEditorRect[] SplitHorizontally(int numItems, float space = 1)
		{
			PKFxEditorRect[] rects = new PKFxEditorRect[numItems];

			float sliceWidth = m_Position.width / numItems - space * (numItems - 1);
			for (int i = 0; i < numItems; ++i)
			{
				rects[i] = new PKFxEditorRect(m_Position.x + (sliceWidth + space) * i, m_Position.y, sliceWidth, m_Position.height);
			}
			return (rects);
		}

		public static implicit operator Rect(PKFxEditorRect r)
		{
			return (r.m_Position);
		}

		public static implicit operator PKFxEditorRect(Rect r)
		{
			return (new PKFxEditorRect(r));
		}
	}

}