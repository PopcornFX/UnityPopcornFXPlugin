//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

namespace PopcornFX
{
	public partial class PKFxAttributePropertyDrawer
	{
		public const int m_IconSize = 13;

		public static bool DrawAttributeInt(SerializedProperty attrDesc, SerializedProperty propX, SerializedProperty propY, SerializedProperty propZ, SerializedProperty propW)
		{
			Int4 value = new Int4(PKFxUtils.Float2Int(propX.floatValue),
								  PKFxUtils.Float2Int(propY.floatValue),
								  PKFxUtils.Float2Int(propZ.floatValue),
								  PKFxUtils.Float2Int(propW.floatValue));
			Int4 Oldvalue = value;
			SerializedProperty m_Type = attrDesc.FindPropertyRelative("m_Type");

			++EditorGUI.indentLevel;
			switch ((EAttributeType)m_Type.intValue)
			{
				case EAttributeType.Int:
					DrawIntAttribute(attrDesc, ref value.x);
					break;
				case EAttributeType.Int2:
					DrawInt2Attribute(attrDesc, ref value.x, ref value.y);
					break;
				case EAttributeType.Int3:
					DrawInt3Attribute(attrDesc, ref value.x, ref value.y, ref value.z);
					break;
				case EAttributeType.Int4:
					DrawInt4Attribute(attrDesc, ref value.x, ref value.y, ref value.z, ref value.w);
					break;
			}
			--EditorGUI.indentLevel;

			if (!(value == Oldvalue))
			{
				propX.floatValue = PKFxUtils.Int2Float(value.x);
				propY.floatValue = PKFxUtils.Int2Float(value.y);
				propZ.floatValue = PKFxUtils.Int2Float(value.z);
				propW.floatValue = PKFxUtils.Int2Float(value.w);
				return true;
			}
			return false;
		}

		public static bool DrawAttributeFloat(SerializedProperty attrDesc, SerializedProperty propX, SerializedProperty propY, SerializedProperty propZ, SerializedProperty propW)
		{
			Vector4 value = new Vector4(propX.floatValue, propY.floatValue, propZ.floatValue, propW.floatValue);
			Vector4 Oldvalue = value;
			SerializedProperty m_Type = attrDesc.FindPropertyRelative("m_Type");

			++EditorGUI.indentLevel;
			switch ((EAttributeType)m_Type.intValue)
			{
				case EAttributeType.Float:
					DrawFloatAttribute(attrDesc, ref value.x);
					break;
				case EAttributeType.Float2:
					DrawFloat2Attribute(attrDesc, ref value.x, ref value.y);
					break;
				case EAttributeType.Float3:
					DrawFloat3Attribute(attrDesc, ref value.x, ref value.y, ref value.z);
					break;
				case EAttributeType.Float4:
					DrawFloat4Attribute(attrDesc, ref value.x, ref value.y, ref value.z, ref value.w);
					break;
			}
			--EditorGUI.indentLevel;

			if (Oldvalue != value)
			{
				propX.floatValue = value.x;
				propY.floatValue = value.y;
				propZ.floatValue = value.z;
				propW.floatValue = value.w;
				return true;
			}
			return false;
		}

		public static bool DrawAttributeBool(SerializedProperty attrDesc, SerializedProperty floatValue)
		{
			float value = floatValue.floatValue;
			float oldValue = value;
			SerializedProperty m_Type = attrDesc.FindPropertyRelative("m_Type");

			++EditorGUI.indentLevel;
			switch ((EAttributeType)m_Type.intValue)
			{
				case EAttributeType.Bool:
				case EAttributeType.Bool2:
				case EAttributeType.Bool3:
				case EAttributeType.Bool4:
					DrawBoolsAttribute(attrDesc, ref value);
					break;
			}
			--EditorGUI.indentLevel;

			if (value != oldValue)
			{
				floatValue.floatValue = value;
				return true;
			}
			return false;
		}

		/// <summary>
		/// Draw attribute name with icon and tooltip
		/// </summary>
		/// <returns>If not folding out, the position returned by PrefixLabel</returns>
		public static void DrawAttributeName(SerializedProperty attrDesc, bool canFoldOut = false)
		{
			GUIContent content = CreateGUIContentFromAttribute(attrDesc);

			if (canFoldOut)
			{
				Rect iconRect = new Rect(0, 0, m_IconSize, m_IconSize);

				GUI.DrawTexture(iconRect, content.image, ScaleMode.ScaleToFit);
				content.image = null;
				attrDesc.isExpanded = EditorGUILayout.Foldout(attrDesc.isExpanded, content);
			}
			else
			{
				EditorGUILayout.PrefixLabel(content);
			}
		}

		public static GUIContent CreateGUIContentFromAttribute(SerializedProperty attrDesc)
		{
			SerializedProperty nameProp = attrDesc.FindPropertyRelative("m_Name");
			SerializedProperty descriptionProp = attrDesc.FindPropertyRelative("m_Description");
			SerializedProperty typeProp = attrDesc.FindPropertyRelative("m_Type");
			Texture2D icon = LoadTextureByAttributeType((EAttributeType)typeProp.intValue);
			string description = descriptionProp.stringValue.Replace("\\n", "\n");

			return new GUIContent(nameProp.stringValue, icon, description);
		}

		//----------------------------------------------------------------------------

		private static Texture2D LoadTextureByAttributeType(EAttributeType AttributeType)
		{
			string iconPath;
			switch (AttributeType)
			{
				case EAttributeType.Int:
					iconPath = "AttributeI1";
					break;
				case EAttributeType.Int2:
					iconPath = "AttributeI2";
					break;
				case EAttributeType.Int3:
					iconPath = "AttributeI3";
					break;
				case EAttributeType.Int4:
					iconPath = "AttributeI4";
					break;
				case EAttributeType.Float:
					iconPath = "AttributeF1";
					break;
				case EAttributeType.Float2:
					iconPath = "AttributeF2";
					break;
				case EAttributeType.Float3:
					iconPath = "AttributeF3";
					break;
				case EAttributeType.Float4:
					iconPath = "AttributeF4";
					break;
				case EAttributeType.Bool:
					iconPath = "AttributeB1";
					break;
				case EAttributeType.Bool2:
					iconPath = "AttributeB2";
					break;
				case EAttributeType.Bool3:
					iconPath = "AttributeB3";
					break;
				case EAttributeType.Bool4:
					iconPath = "AttributeB4";
					break;
				default:
					return (null);
			}
			return (Resources.Load<Texture2D>(PKFxEditorTools.IconsRootDirectory + "/" + iconPath));
		}

		private float GetHeightForControls(int numControls)
		{
			return (EditorGUIUtility.singleLineHeight * numControls + EditorGUIUtility.standardVerticalSpacing * (numControls - 1));
		}
	}
}