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
		private static void DrawFloatAttribute(SerializedProperty attrDesc, ref float valueX)
		{
			SerializedProperty m_MinMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
			SerializedProperty m_MinValue = attrDesc.FindPropertyRelative("m_MinValue");
			SerializedProperty m_MaxValue = attrDesc.FindPropertyRelative("m_MaxValue");

			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc);

			float labelwidth = EditorGUIUtility.labelWidth;
			EditorGUIUtility.labelWidth = 28;

			if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue) && PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
				valueX = PKFxEditorTools.FloatSlider(valueX, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "x"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "x"), new GUIContent("X"));
			else if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
				valueX = PKFxEditorTools.MinFloatField(valueX, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "x"), new GUIContent("X"));
			else if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
				valueX = PKFxEditorTools.MaxFloatField(valueX, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "x"), new GUIContent("X"));
			else
				valueX = EditorGUILayout.FloatField("X", valueX);

			EditorGUIUtility.labelWidth = labelwidth;
			EditorGUILayout.EndHorizontal();
		}

		//----------------------------------------------------------------------------

		private static void DrawFloat2Attribute(SerializedProperty attrDesc, ref float valueX, ref float valueY)
		{
			SerializedProperty m_MinMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
			SerializedProperty m_MinValue = attrDesc.FindPropertyRelative("m_MinValue");
			SerializedProperty m_MaxValue = attrDesc.FindPropertyRelative("m_MaxValue");

			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc, false);
			if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue) && PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
			{
				EditorGUILayout.BeginVertical();
				valueX = PKFxEditorTools.FloatSlider(valueX, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "x"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "x"), null);
				valueY = PKFxEditorTools.FloatSlider(valueY, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "y"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "y"), null);
				EditorGUILayout.EndVertical();
			}
			else
			{
				Vector2 tmp2 = new Vector2(valueX, valueY);
				tmp2 = EditorGUILayout.Vector2Field(GUIContent.none, tmp2);

				if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
					tmp2 = PKFxEditorTools.Maxf(tmp2,
												PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "x"),
												PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "y"));
				if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
					tmp2 = PKFxEditorTools.Minf(tmp2,
												PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "x"),
												PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "y"));

				valueX = tmp2.x;
				valueY = tmp2.y;
			}
			EditorGUILayout.EndHorizontal();
		}

		//----------------------------------------------------------------------------

		private static void DrawFloat3Attribute(SerializedProperty attrDesc, ref float valueX, ref float valueY, ref float valueZ)
		{
			SerializedProperty m_Semantic = attrDesc.FindPropertyRelative("m_Semantic");
			SerializedProperty m_MinMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
			SerializedProperty m_MinValue = attrDesc.FindPropertyRelative("m_MinValue");
			SerializedProperty m_MaxValue = attrDesc.FindPropertyRelative("m_MaxValue");

			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc, false);
			if ((EAttributeSemantic)m_Semantic.intValue == EAttributeSemantic.Color)
			{
				Vector3 tmp3 = new Vector3(PKFxUtils.Linear2sRGB(valueX), PKFxUtils.Linear2sRGB(valueY), PKFxUtils.Linear2sRGB(valueZ));

				EditorGUILayout.BeginVertical();
				tmp3 = EditorGUILayout.Vector3Field(GUIContent.none, tmp3);
				PKFxEditorTools.ColorPicker(GUIContent.none, ref tmp3.x, ref tmp3.y, ref tmp3.z, true);
				EditorGUILayout.EndVertical();

				valueX = PKFxUtils.sRGB2Linear(tmp3.x);
				valueY = PKFxUtils.sRGB2Linear(tmp3.y);
				valueZ = PKFxUtils.sRGB2Linear(tmp3.z);
			}
			else if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue) && PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
			{
				EditorGUILayout.BeginVertical();
				valueX = PKFxEditorTools.FloatSlider(valueX, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "x"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "x"), null);
				valueY = PKFxEditorTools.FloatSlider(valueY, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "y"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "y"), null);
				valueZ = PKFxEditorTools.FloatSlider(valueZ, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "z"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "z"), null);
				EditorGUILayout.EndVertical();
			}
			else
			{
				Vector3 tmp3 = new Vector3(valueX, valueY, valueZ);
				tmp3 = EditorGUILayout.Vector3Field(GUIContent.none, tmp3);
				{
					if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
						tmp3 = PKFxEditorTools.Maxf(tmp3,
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "x"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "y"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "z"));
					if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
						tmp3 = PKFxEditorTools.Minf(tmp3,
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "x"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "y"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "z"));

					valueX = tmp3.x;
					valueY = tmp3.y;
					valueZ = tmp3.z;
				}
			}
			EditorGUILayout.EndHorizontal();

			// Re-clamp between min and max at the end:
			/* Seem unecessary
			{
				Vector3		tmp3 = new Vector3(valueX, valueY, valueZ);

				if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
					tmp3 = PKFxEditorTools.Maxf(tmp3, m_MinValue.FindPropertyRelative("x").FindPropertyRelative("f1").floatValue, m_MinValue.FindPropertyRelative("y").FindPropertyRelative("f1").floatValue, m_MinValue.FindPropertyRelative("z").FindPropertyRelative("f1").floatValue);
				if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
					tmp3 = PKFxEditorTools.Minf(tmp3, m_MaxValue.FindPropertyRelative("x").FindPropertyRelative("f1").floatValue, m_MaxValue.FindPropertyRelative("y").FindPropertyRelative("f1").floatValue, m_MaxValue.FindPropertyRelative("z").FindPropertyRelative("f1").floatValue);

				valueX = tmp3.x;
				valueY = tmp3.y;
				valueZ = tmp3.z;
			}
			*/
		}

		//----------------------------------------------------------------------------

		private static void DrawFloat4Attribute(SerializedProperty attrDesc, ref float valueX, ref float valueY, ref float valueZ, ref float valueW)
		{
			SerializedProperty m_Semantic = attrDesc.FindPropertyRelative("m_Semantic");
			SerializedProperty m_MinMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
			SerializedProperty m_MinValue = attrDesc.FindPropertyRelative("m_MinValue");
			SerializedProperty m_MaxValue = attrDesc.FindPropertyRelative("m_MaxValue");

			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc, false);
			if ((EAttributeSemantic)m_Semantic.intValue == EAttributeSemantic.Color)
			{
				Vector4 tmp4 = new Vector4(PKFxUtils.Linear2sRGB(valueX), PKFxUtils.Linear2sRGB(valueY), PKFxUtils.Linear2sRGB(valueZ), valueW);

				EditorGUILayout.BeginVertical();
				tmp4 = EditorGUILayout.Vector4Field(GUIContent.none, tmp4);
				PKFxEditorTools.ColorPicker(GUIContent.none, ref tmp4.x, ref tmp4.y, ref tmp4.z, ref tmp4.w, true);
				EditorGUILayout.EndVertical();

				valueX = PKFxUtils.sRGB2Linear(tmp4.x);
				valueY = PKFxUtils.sRGB2Linear(tmp4.y);
				valueZ = PKFxUtils.sRGB2Linear(tmp4.z);
				valueW = tmp4.w;
			}
			else if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue) && PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
			{
				EditorGUILayout.BeginVertical();
				valueX = PKFxEditorTools.FloatSlider(valueX, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "x"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "x"));
				valueY = PKFxEditorTools.FloatSlider(valueY, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "y"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "y"));
				valueZ = PKFxEditorTools.FloatSlider(valueZ, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "z"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "z"));
				valueW = PKFxEditorTools.FloatSlider(valueW, PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "w"), PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "w"));
				EditorGUILayout.EndVertical();
			}
			else
			{
				EditorGUILayout.BeginVertical();
				Vector4 tmp4 = new Vector4(valueX, valueY, valueZ, valueW);
				tmp4 = EditorGUILayout.Vector4Field(GUIContent.none, tmp4);
				{
					if (PKFxEditorTools.HasMin(m_MinMaxFlag.intValue))
						tmp4 = PKFxEditorTools.Maxf(tmp4,
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "x"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "y"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "z"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MinValue, "w"));
					if (PKFxEditorTools.HasMax(m_MinMaxFlag.intValue))
						tmp4 = PKFxEditorTools.Minf(tmp4,
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "x"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "y"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "z"),
													PKFxAssetEditor.AttrBoundsGetFloatValueSerialized(m_MaxValue, "w"));

					valueX = tmp4.x;
					valueY = tmp4.y;
					valueZ = tmp4.z;
					valueW = tmp4.w;
				}
				EditorGUILayout.EndVertical();
			}
			EditorGUILayout.EndHorizontal();
		}

	}
}