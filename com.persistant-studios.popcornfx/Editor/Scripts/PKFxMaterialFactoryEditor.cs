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
	[CustomEditor(typeof(PKFxMaterialFactory), true)]
	public class PKFxMaterialFactoryEditor : Editor
	{
        private static bool m_ShowCustomMaterials = false;
        private PKFxMaterialFactory m_MaterialFactory;

        void OnEnable()
        {
            if (targets.Length > 0)
                m_MaterialFactory = target as PKFxMaterialFactory;
            else
                m_MaterialFactory = null;
        }

        void OnDisable()
        {
        }

        public override void OnInspectorGUI()
        {
            m_ShowCustomMaterials = EditorGUILayout.BeginFoldoutHeaderGroup(m_ShowCustomMaterials, "Custom Materials");
            if (m_ShowCustomMaterials)
			{
                GUILayout.BeginVertical();
                if (m_MaterialFactory != null)
                {
                    if (GUILayout.Button("Reset Custom Materials"))
                    {
                        m_MaterialFactory.ResetAllCustomMaterials();
                    }
                    m_MaterialFactory.DrawEditorCustomMaterialList();
                }
                GUILayout.EndVertical();
            }
            EditorGUILayout.EndFoldoutHeaderGroup();
            GUILayout.Space(15);
            Rect rect = EditorGUILayout.GetControlRect(false, 1);
            rect.height = 1;
            EditorGUI.DrawRect(rect, new Color(0.1f, 0.1f, 0.1f, 1.0f));
            GUILayout.Space(15);
            DrawDefaultInspector();
        }
    }
}
