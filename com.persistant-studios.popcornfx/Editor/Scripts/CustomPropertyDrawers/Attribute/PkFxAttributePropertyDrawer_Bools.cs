using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

namespace PopcornFX
{
	public partial class PKFxAttributePropertyDrawer : PropertyDrawer
	{
		private static void DrawBoolsAttribute(SerializedProperty attrDesc, ref float valueX)
		{
			SerializedProperty m_Type = attrDesc.FindPropertyRelative("m_Type");

			byte[] floatBytes = System.BitConverter.GetBytes(valueX);

			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc);
			PKFxEditorTools.BoolsCheckboxes(floatBytes, (EAttributeType)m_Type.intValue);
			EditorGUILayout.EndHorizontal();

			valueX = System.BitConverter.ToSingle(floatBytes, 0);
		}
	}
}