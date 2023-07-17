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
		private static void DrawQuaternionAttribute(SerializedProperty attrDesc, ref float valueX, ref float valueY, ref float valueZ, ref float valueW)
		{
			EditorGUILayout.BeginHorizontal();
			DrawAttributeName(attrDesc, false);
			EditorGUILayout.BeginVertical();

			valueX = (float.IsNaN(valueX) ? 0f : valueX); // Checking for NaN
			valueY = (float.IsNaN(valueY) ? 0f : valueY);
			valueZ = (float.IsNaN(valueZ) ? 0f : valueZ);
			valueW = (float.IsNaN(valueW) ? 0f : valueW);

			Vector3 euler = new Quaternion(valueX, valueY, valueZ, valueW).eulerAngles;
			Vector3 newEuler = EditorGUILayout.Vector3Field(GUIContent.none, euler);
			if (euler != newEuler)
			{
				Quaternion newQuat = Quaternion.Euler(newEuler);
				valueX = newQuat.x;
				valueY = newQuat.y;
				valueZ = newQuat.z;
				valueW = newQuat.w;
			}

			EditorGUILayout.EndVertical();
			EditorGUILayout.EndHorizontal();
		}
	}
}