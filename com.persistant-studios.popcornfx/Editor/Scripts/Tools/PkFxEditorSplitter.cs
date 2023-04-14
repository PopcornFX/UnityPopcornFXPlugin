//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

namespace PopcornFX
{
	/// <summary>
	/// Allow to draw horizontal lines in the Inspector
	/// </summary>
	public static class PkFxEditorSplitter
	{
		public static readonly GUIStyle splitter;

		private static readonly Color splitterColor = EditorGUIUtility.isProSkin ? new Color(0.157f, 0.157f, 0.157f) : new Color(0.5f, 0.5f, 0.5f);

		static PkFxEditorSplitter()
		{
			splitter = new GUIStyle();
			splitter.normal.background = EditorGUIUtility.whiteTexture;
			splitter.stretchWidth = true;
			splitter.margin = new RectOffset(0, 0, 7, 7);
		}

		public static void Splitter(int margins, float thickness = 1)
		{
			Vector2Int initialMargin = new Vector2Int(splitter.margin.top, splitter.margin.bottom);
			splitter.margin.top = splitter.margin.bottom = margins;

			Splitter(splitterColor, thickness);

			splitter.margin.top = initialMargin.x;
			splitter.margin.bottom = initialMargin.y;
		}

		public static void Splitter(Color rgb, float thickness = 1)
		{

			Rect position = GUILayoutUtility.GetRect(GUIContent.none, splitter, GUILayout.Height(thickness));

			if (Event.current.type == EventType.Repaint)
			{
				Color restoreColor = GUI.color;
				GUI.color = rgb;
				splitter.Draw(position, false, false, false, false);
				GUI.color = restoreColor;
			}
		}

		public static void Splitter(float thickness = 1)
		{
			Splitter(splitterColor, thickness);
		}
	}
}