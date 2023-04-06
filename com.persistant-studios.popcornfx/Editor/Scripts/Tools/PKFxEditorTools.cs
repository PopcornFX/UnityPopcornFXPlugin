//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System;

namespace PopcornFX
{
	public static class PKFxEditorTools
	{
		public const string IconsRootDirectory = "Icons";

		public static readonly Color EditorBackgroundColor = EditorGUIUtility.isProSkin
			? new Color32(56, 56, 56, 255)
			: new Color32(194, 194, 194, 255);

		public static readonly Color EditorBoxBackgroundColor = EditorGUIUtility.isProSkin
			? new Color32(56, 56, 56, 255) // TODO : check the color on a ProSkin editor
			: new Color32(208, 208, 208, 255);

		public static bool ColorPicker(GUIContent content, ref float r, ref float g, ref float b, bool expand = false)
		{
			Color color = new Color(r, g, b, 1.0f);
			Color newColor;
			if (expand)
				newColor = EditorGUILayout.ColorField(content, color);
			else
				newColor = EditorGUILayout.ColorField(content, color, GUILayout.MaxWidth(EditorGUIUtility.singleLineHeight * 4));

			if (color != newColor)
			{
				r = newColor.r;
				g = newColor.g;
				b = newColor.b;
				return true;
			}
			return false;
		}

		public static bool ColorPicker(GUIContent content, ref float r, ref float g, ref float b, ref float a, bool expand = false)
		{
			Color color = new Color(r, g, b, a);
			Color newColor;
			if (expand)
				newColor = EditorGUILayout.ColorField(content, color);
			else
				newColor = EditorGUILayout.ColorField(content, color, GUILayout.MaxWidth(EditorGUIUtility.singleLineHeight * 4));

			if (color != newColor)
			{
				r = newColor.r;
				g = newColor.g;
				b = newColor.b;
				a = newColor.a;
				return true;
			}
			return false;
		}

		//----------------------------------------------------------------------------

		public static void BoolsCheckboxes(byte[] floatBytes, EAttributeType type, GUIContent content = null)
		{
			GUILayoutOption[] opt = { GUILayout.MaxWidth(70) };

			float labelwidth = EditorGUIUtility.labelWidth;
			EditorGUIUtility.labelWidth = 28;
			if (type == EAttributeType.Bool)
			{
				floatBytes[0] = (byte)(EditorGUILayout.Toggle("X", floatBytes[0] != 0, opt) == true ? 0xFF : 0);
				EditorGUILayout.Space();
				EditorGUILayout.Space();
				EditorGUILayout.Space();
			}
			else if (type == EAttributeType.Bool2)
			{
				floatBytes[0] = (byte)(EditorGUILayout.Toggle("X", floatBytes[0] != 0, opt) == true ? 0xFF : 0);
				floatBytes[1] = (byte)(EditorGUILayout.Toggle("Y", floatBytes[1] != 0, opt) == true ? 0xFF : 0);
				EditorGUILayout.Space();
				EditorGUILayout.Space();
			}
			else if (type == EAttributeType.Bool3)
			{
				floatBytes[0] = (byte)(EditorGUILayout.Toggle("X", floatBytes[0] != 0, opt) == true ? 0xFF : 0);
				floatBytes[1] = (byte)(EditorGUILayout.Toggle("Y", floatBytes[1] != 0, opt) == true ? 0xFF : 0);
				floatBytes[2] = (byte)(EditorGUILayout.Toggle("Z", floatBytes[2] != 0, opt) == true ? 0xFF : 0);
				EditorGUILayout.Space();
			}
			else if (type == EAttributeType.Bool4)
			{
				floatBytes[0] = (byte)(EditorGUILayout.Toggle("X", floatBytes[0] != 0, opt) == true ? 0xFF : 0);
				floatBytes[1] = (byte)(EditorGUILayout.Toggle("Y", floatBytes[1] != 0, opt) == true ? 0xFF : 0);
				floatBytes[2] = (byte)(EditorGUILayout.Toggle("Z", floatBytes[2] != 0, opt) == true ? 0xFF : 0);
				floatBytes[3] = (byte)(EditorGUILayout.Toggle("W", floatBytes[3] != 0, opt) == true ? 0xFF : 0);
			}
			EditorGUIUtility.labelWidth = labelwidth;
		}

		//----------------------------------------------------------------------------

		public static float FloatSlider(float value, float min, float max, GUIContent content = null)
		{
			if (content != null)
				return EditorGUILayout.Slider(content, value, min, max);
			else
				return EditorGUILayout.Slider(value, min, max);
		}

		//----------------------------------------------------------------------------

		public static int IntSlider(int value, int min, int max, GUIContent content = null)
		{
			if (content != null)
				return EditorGUILayout.IntSlider(content, value, min, max);
			else
				return EditorGUILayout.IntSlider(value, min, max);
		}

		//----------------------------------------------------------------------------

		public static float MinFloatField(float value, float min, GUIContent content = null)
		{
			if (content == null)
			{
				content = GUIContent.none;
			}
			return Mathf.Max(EditorGUILayout.FloatField(content, value), min);
		}

		public static float MaxFloatField(float value, float max, GUIContent content = null)
		{
			if (content == null)
			{
				content = GUIContent.none;
			}
			return Mathf.Min(EditorGUILayout.FloatField(content, value), max);
		}

		public static int MaxIntField(int value, int max, GUIContent content = null)
		{
			if (content == null)
			{
				content = GUIContent.none;
			}
			return Math.Min(EditorGUILayout.IntField(content, value), max);
		}

		public static int MinIntField(int value, int min, GUIContent content = null)
		{
			if (content == null)
			{
				content = GUIContent.none;
			}
			return Math.Max(EditorGUILayout.IntField(content, value), min);
		}

		//----------------------------------------------------------------------------

		public static int IntField(SerializedProperty v)
		{
			return EditorGUILayout.IntField(v.intValue);
		}

		//----------------------------------------------------------------------------

		public static bool HasMin(int flag)
		{
			return (flag & (int)PKFxEffectAsset.AttributeDesc.EAttrDescFlag.HasMin) != 0;
		}

		//----------------------------------------------------------------------------

		public static bool HasMax(int flag)
		{
			return (flag & (int)PKFxEffectAsset.AttributeDesc.EAttrDescFlag.HasMax) != 0;
		}

		//----------------------------------------------------------------------------

		public static Vector2 Minf(Vector2 v, float x, float y)
		{
			v.x = Mathf.Min(v.x, x);
			v.y = Mathf.Min(v.y, y);
			return v;
		}

		public static Vector3 Minf(Vector3 v, float x, float y, float z)
		{
			v.x = Mathf.Min(v.x, x);
			v.y = Mathf.Min(v.y, y);
			v.z = Mathf.Min(v.z, z);
			return v;
		}

		public static Vector4 Minf(Vector4 v, float x, float y, float z, float w)
		{
			v.x = Mathf.Min(v.x, x);
			v.y = Mathf.Min(v.y, y);
			v.z = Mathf.Min(v.z, z);
			v.w = Mathf.Min(v.w, w);
			return v;
		}

		public static Vector2Int Min(Vector2Int v, int x, int y)
		{
			v.x = Math.Min(v.x, x);
			v.y = Math.Min(v.y, y);
			return v;
		}

		public static Vector3Int Min(Vector3Int v, int x, int y, int z)
		{
			v.x = Math.Min(v.x, x);
			v.y = Math.Min(v.y, y);
			v.z = Math.Min(v.z, z);
			return v;
		}

		internal static Int4 Min(Int4 v, int x, int y, int z, int w)
		{
			v.x = Math.Min(v.x, x);
			v.y = Math.Min(v.y, y);
			v.z = Math.Min(v.z, z);
			v.w = Math.Min(v.w, w);
			return v;
		}

		//----------------------------------------------------------------------------

		public static Vector2 Maxf(Vector2 v, float x, float y)
		{
			v.x = Mathf.Max(v.x, x);
			v.y = Mathf.Max(v.y, y);
			return v;
		}

		public static Vector3 Maxf(Vector3 v, float x, float y, float z)
		{
			v.x = Mathf.Max(v.x, x);
			v.y = Mathf.Max(v.y, y);
			v.z = Mathf.Max(v.z, z);
			return v;
		}

		public static Vector4 Maxf(Vector4 v, float x, float y, float z, float w)
		{
			v.x = Mathf.Max(v.x, x);
			v.y = Mathf.Max(v.y, y);
			v.z = Mathf.Max(v.z, z);
			v.w = Mathf.Max(v.w, w);
			return v;
		}

		public static Vector2Int Max(Vector2Int v, int x, int y)
		{
			v.x = Math.Max(v.x, x);
			v.y = Math.Max(v.y, y);
			return v;
		}

		public static Vector3Int Max(Vector3Int v, int x, int y, int z)
		{
			v.x = Math.Max(v.x, x);
			v.y = Math.Max(v.y, y);
			v.z = Math.Max(v.z, z);
			return v;
		}

		internal static Int4 Max(Int4 v, int x, int y, int z, int w)
		{
			v.x = Mathf.Max(v.x, x);
			v.y = Mathf.Max(v.y, y);
			v.z = Mathf.Max(v.z, z);
			v.w = Mathf.Max(v.w, w);
			return v;
		}
	}
}