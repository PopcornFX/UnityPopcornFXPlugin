//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Reflection;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

namespace PopcornFX
{
	[CustomEditor(typeof(PKFxCustomMaterialInfo))]
	public class PKFxCustomMaterialInfoEditor : Editor
	{
		private PKFxCustomMaterialInfo m_Materialinfo;

		void OnEnable()
		{
			if (targets.Length > 0)
				m_Materialinfo = target as PKFxCustomMaterialInfo;
			else
				m_Materialinfo = null;
		}

		void OnDisable()
		{
		}

		public override void OnInspectorGUI()
		{
			PKFxCustomMaterialInfo  customMat = serializedObject.targetObject as PKFxCustomMaterialInfo;
			EditorGUILayout.LabelField(customMat.m_BatchDescName);
			EditorGUILayout.LabelField("Renderer UID: " + customMat.m_UID);
			PKFxEffectAsset.DrawEditorCustomMaterial(customMat, true);
		}
	}
}
