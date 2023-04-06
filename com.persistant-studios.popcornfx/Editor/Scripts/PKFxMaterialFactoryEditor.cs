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
			DrawDefaultInspector();
		}
	}
}
