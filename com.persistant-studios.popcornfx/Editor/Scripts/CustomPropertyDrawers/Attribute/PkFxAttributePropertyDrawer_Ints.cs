//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

namespace PopcornFX
{
	public partial class PKFxAttributePropertyDrawer : PropertyDrawer
	{
		private static void DrawIntAttribute(SerializedProperty attrDesc, ref int valueX)
		{
			//GUIContent content = CreateGUIContentFromAttribute(attrDesc);

			SerializedProperty m_MinMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
			SerializedProperty m_MinValue = attrDesc.FindPropertyRelative("m_MinValue");
			SerializedProperty m_MaxValue = attrDesc.FindPropertyRelative("m_MaxValue");


			//EditorGUILayout.BeginHorizontal();
			//if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue) && PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
			//	valueX = PKFxEditorTools.IntSlider(valueX, m_MinValue.vector4Value.x, m_MaxValue.vector4Value.x, content);
			//else if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
			//	valueX = PKFxEditorTools.MinIntField(valueX, m_MinValue.vector4Value.x, content);
			//else if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
			//	valueX = PKFxEditorTools.MaxIntField(valueX, m_MaxValue.vector4Value.x, content);
			//else
			//	valueX = PKFxManagerImpl.Int2Float(EditorGUILayout.IntField(content, PKFxManagerImpl.Float2Int(valueX)));
			//EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc);

			float labelwidth = EditorGUIUtility.labelWidth;
			EditorGUIUtility.labelWidth = 28;
			if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue) && PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
			{
				valueX = PKFxEditorTools.IntSlider(valueX, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "x"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "x"), new GUIContent("X"));
			}
			else
			{
				if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
					valueX = PKFxEditorTools.MinIntField(valueX, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "x"), new GUIContent("X"));
				else if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
					valueX = PKFxEditorTools.MaxIntField(valueX, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "x"), new GUIContent("X"));
				else
					valueX = EditorGUILayout.IntField("X", valueX);
			}
			EditorGUIUtility.labelWidth = labelwidth;
			EditorGUILayout.EndHorizontal();
		}

		//----------------------------------------------------------------------------

		private static void DrawInt2Attribute(SerializedProperty attrDesc, ref int valueX, ref int valueY)
		{
			SerializedProperty m_MinMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
			SerializedProperty m_MinValue = attrDesc.FindPropertyRelative("m_MinValue");
			SerializedProperty m_MaxValue = attrDesc.FindPropertyRelative("m_MaxValue");

			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc);
			if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue) && PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
			{
				EditorGUILayout.BeginVertical();
				valueX = PKFxEditorTools.IntSlider(valueX, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "x"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "x"), null);
				valueY = PKFxEditorTools.IntSlider(valueY, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "y"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "y"), null);
				EditorGUILayout.EndVertical();
			}
			else
			{
				Vector2Int tmp2 = new Vector2Int(valueX, valueY);
				tmp2 = EditorGUILayout.Vector2IntField(GUIContent.none, tmp2);

				if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
					tmp2 = PKFxEditorTools.Max(tmp2, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "x"),
													 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "y"));
				if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
					tmp2 = PKFxEditorTools.Min(tmp2, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "x"),
													 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "y"));

				valueX = tmp2.x;
				valueY = tmp2.y;
			}
			EditorGUILayout.EndHorizontal();
		}

		//----------------------------------------------------------------------------

		private static void DrawInt3Attribute(SerializedProperty attrDesc, ref int valueX, ref int valueY, ref int valueZ)
		{
			SerializedProperty m_MinMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
			SerializedProperty m_MinValue = attrDesc.FindPropertyRelative("m_MinValue");
			SerializedProperty m_MaxValue = attrDesc.FindPropertyRelative("m_MaxValue");

			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc);
			if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue) && PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
			{
				EditorGUILayout.BeginVertical();
				valueX = PKFxEditorTools.IntSlider(valueX, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "x"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "x"), null);
				valueY = PKFxEditorTools.IntSlider(valueY, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "y"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "y"), null);
				valueZ = PKFxEditorTools.IntSlider(valueZ, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "z"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "z"), null);
				EditorGUILayout.EndVertical();
			}
			else
			{
				Vector3Int tmp3 = new Vector3Int(valueX, valueY, valueZ);
				tmp3 = EditorGUILayout.Vector3IntField(GUIContent.none, tmp3);
				{
					if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
						tmp3 = PKFxEditorTools.Max(tmp3, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "x"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "y"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "z"));

					if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
						tmp3 = PKFxEditorTools.Min(tmp3, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "x"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "y"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "z"));

					valueX = tmp3.x;
					valueY = tmp3.y;
					valueZ = tmp3.z;
				}
			}
			EditorGUILayout.EndHorizontal();
		}

		//----------------------------------------------------------------------------

		private static void DrawInt4Attribute(SerializedProperty attrDesc, ref int valueX, ref int valueY, ref int valueZ, ref int valueW)
		{
			SerializedProperty m_MinMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
			SerializedProperty m_MinValue = attrDesc.FindPropertyRelative("m_MinValue");
			SerializedProperty m_MaxValue = attrDesc.FindPropertyRelative("m_MaxValue");

			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc, false);
			if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue) && PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
			{
				EditorGUILayout.BeginVertical();
				valueX = PKFxEditorTools.IntSlider(valueX, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "x"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "x"), null);
				valueY = PKFxEditorTools.IntSlider(valueY, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "y"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "y"), null);
				valueZ = PKFxEditorTools.IntSlider(valueZ, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "z"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "z"), null);
				valueW = PKFxEditorTools.IntSlider(valueW, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "w"), PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "w"), null);
				EditorGUILayout.EndVertical();
			}
			else
			{
				float labelwidth = EditorGUIUtility.labelWidth;
				EditorGUIUtility.labelWidth = 28;
				Int4 tmp4 = new Int4(valueX, valueY, valueZ, valueW);
				tmp4.x = EditorGUILayout.IntField("X", tmp4.x, GUILayout.MaxWidth(100));
				tmp4.y = EditorGUILayout.IntField("Y", tmp4.y, GUILayout.MaxWidth(100));
				tmp4.z = EditorGUILayout.IntField("Z", tmp4.z, GUILayout.MaxWidth(100));
				tmp4.w = EditorGUILayout.IntField("W", tmp4.w, GUILayout.MaxWidth(100));
				{
					if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
						tmp4 = PKFxEditorTools.Max(tmp4, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "x"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "y"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "z"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MinValue, "w"));
					if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
						tmp4 = PKFxEditorTools.Min(tmp4, PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "x"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "y"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "z"),
														 PKFxAssetEditor.AttrBoundsGetIntValueSerialized(m_MaxValue, "w"));

					valueX = tmp4.x;
					valueY = tmp4.y;
					valueZ = tmp4.z;
					valueW = tmp4.w;
				}
				EditorGUIUtility.labelWidth = labelwidth;
			}
			EditorGUILayout.EndHorizontal();
		}
	}
}