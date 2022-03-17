//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using UnityEditor;

namespace PopcornFX
{
	/// <summary>
	/// Allow to create a category in the editor.
	/// A category is just a box that regroup the controllers that will be inside.
	/// Equivalent of group box in .Net
	/// Use :
	/// using (var category = new PKFxEditorCategory(anyProperty, "MyCategory"))
	/// {
	///   if (category.IsExpended())
	///   { ... draw controls... }
	/// }
	/// </summary>
	public class PKFxEditorCategory : IDisposable
	{
		private SerializedProperty m_FoldoutProperty;
		private bool m_IsExpanded;

		public Rect RectArea { get; private set; }

		public PKFxEditorCategory(SerializedProperty foldoutProperty, string categoryName)
		{
			m_FoldoutProperty = foldoutProperty;
			StartIndentation(categoryName);
		}

		public PKFxEditorCategory(bool isExpended, string categoryName)
		{
			m_IsExpanded = isExpended;
			StartIndentation(categoryName);
		}

		public PKFxEditorCategory(Func<bool> customHeaderDrawer)
		{
			RectArea = EditorGUILayout.BeginVertical(EditorStyles.helpBox);
			++EditorGUI.indentLevel;

			SetPropertyExpanded(customHeaderDrawer.Invoke());
			if (IsExpanded())
			{
				--EditorGUI.indentLevel;
				PkFxEditorSplitter.Splitter();
			}
		}

		private void StartIndentation(string categoryName)
		{
			RectArea = EditorGUILayout.BeginVertical(EditorStyles.helpBox);
			++EditorGUI.indentLevel;

			SetPropertyExpanded(EditorGUILayout.Foldout(IsExpanded(), categoryName, true));
			if (IsExpanded())
			{
				--EditorGUI.indentLevel;
				PkFxEditorSplitter.Splitter();
			}
		}

		public bool IsExpanded()
		{
			return (m_FoldoutProperty != null ? m_FoldoutProperty.isExpanded : m_IsExpanded);
		}

		public void Dispose()
		{
			if (IsExpanded())
			{
				++EditorGUI.indentLevel;
			}

			--EditorGUI.indentLevel;
			EditorGUILayout.EndVertical();
		}

		private void SetPropertyExpanded(bool isExpanded)
		{
			if (m_FoldoutProperty != null)
			{
				m_FoldoutProperty.isExpanded = isExpanded;
			}
			else
			{
				m_IsExpanded = isExpanded;
			}
		}
	}
}