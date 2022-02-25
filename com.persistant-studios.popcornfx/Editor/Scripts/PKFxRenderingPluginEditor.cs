//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEditor;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace PopcornFX
{
	[CustomEditor(typeof(PKFxRenderingPlugin))]
	[CanEditMultipleObjects]
	public class PKFxRenderingPluginEditor : Editor
	{
		GUIContent timeMultiplierLabel = new GUIContent(" Time scale for the particle simulation");
		GUIContent cameraNumberLabel = new GUIContent(" Max number of cameras");

		SerializedProperty m_TimeMultiplier;
		SerializedProperty m_ShowAdvanced;
		SerializedProperty m_MaxCameraSupport;
		SerializedProperty m_EnableDistortion;
		SerializedProperty m_EnableBlur;
		SerializedProperty m_BlurFactor;
		SerializedProperty m_UseSceneMesh;
		SerializedProperty m_SceneMesh;
		SerializedProperty m_PreloadEffect;

		//----------------------------------------------------------------------------

		void OnEnable()
		{
			m_TimeMultiplier = serializedObject.FindProperty("m_TimeMultiplier");
			m_MaxCameraSupport = serializedObject.FindProperty("m_MaxCameraSupport");
			m_EnableDistortion = serializedObject.FindProperty("m_EnableDistortion");
			m_EnableBlur = serializedObject.FindProperty("m_EnableBlur");
			m_PreloadEffect = serializedObject.FindProperty("m_PreloadEffect");
			m_BlurFactor = serializedObject.FindProperty("m_BlurFactor");
			m_UseSceneMesh = serializedObject.FindProperty("m_UseSceneMesh");
			m_SceneMesh = serializedObject.FindProperty("m_SceneMesh");
		}

		//----------------------------------------------------------------------------
		public static List<T> FindAssetsByType<T>() where T : UnityEngine.Object
		{
			List<T> assets = new List<T>();
			string[] guids = AssetDatabase.FindAssets(string.Format("t:{0}", typeof(T)));
			for (int i = 0; i < guids.Length; i++)
			{
				string assetPath = AssetDatabase.GUIDToAssetPath(guids[i]);
				T asset = AssetDatabase.LoadAssetAtPath<T>(assetPath);
				if (asset != null)
				{
					assets.Add(asset);
				}
			}
			return assets;
		}

		public override void OnInspectorGUI()
		{
			EditorGUILayout.LabelField("PopcornFX plugin "
									 + PKFxManagerImpl.m_PluginVersion
									 + " (Build "
									 + PKFxManagerImpl.m_CurrentVersionString + ")");

			DrawDefaultInspector();

			EditorGUILayout.BeginHorizontal();
			EditorGUI.BeginChangeCheck();
			float timeMulValue = EditorGUILayout.Slider(timeMultiplierLabel, m_TimeMultiplier.floatValue, 0.0f, 10.0f);
			if (EditorGUI.EndChangeCheck())
			{
				m_TimeMultiplier.floatValue = timeMulValue;
				PKFxManager.TimeMultiplier = timeMulValue;
			}
			EditorGUILayout.EndHorizontal();

			EditorGUI.BeginChangeCheck();
			int maxCamValue = EditorGUILayout.IntSlider(cameraNumberLabel, m_MaxCameraSupport.intValue, 1, 4);
			if (EditorGUI.EndChangeCheck())
			{
				m_MaxCameraSupport.intValue = maxCamValue;
				PKFxManager.SetMaxCameraCount(maxCamValue);
			}

			EditorGUILayout.PropertyField(m_EnableDistortion);
			EditorGUILayout.PropertyField(m_EnableBlur);
			if (m_EnableBlur.boolValue)
			{
				EditorGUI.indentLevel++;
				EditorGUILayout.PropertyField(m_BlurFactor);
				m_BlurFactor.floatValue = Mathf.Clamp(m_BlurFactor.floatValue, 0.0f, 1.0f);
				EditorGUI.indentLevel--;
			}
			HandleSceneMesh();

			PKFxRenderingPlugin renderingPlugin = target as PKFxRenderingPlugin;
			if (GUILayout.Button("Add all PKFxAssets to preload list"))
			{
				m_PreloadEffect.ClearArray();
				List<PKFxEffectAsset> PKFXList = FindAssetsByType<PKFxEffectAsset>();

				Undo.RecordObject(renderingPlugin, "Add all PKFxAssets to preload list");
				renderingPlugin.m_PreloadEffect = PKFXList;
			}
			EditorGUILayout.PropertyField(m_PreloadEffect);

			serializedObject.ApplyModifiedProperties();
		}

		//----------------------------------------------------------------------------

		private void HandleSceneMesh()
		{
			EditorGUILayout.PropertyField(m_UseSceneMesh);
			if (m_UseSceneMesh.boolValue)
			{
				EditorGUI.indentLevel++;

				if (PKFxSettings.EnableRaycastForCollisions)
					EditorGUILayout.HelpBox("Can't enable Scene Mesh if raycast for collisions is enabled in settings.", MessageType.Warning, true);
				EditorGUILayout.PropertyField(m_SceneMesh, new GUIContent("Mesh Asset"));
				EditorGUI.indentLevel--;
			}
		}

		//----------------------------------------------------------------------------

		[InitializeOnLoad]
		public class PlayModeChangeWatcher
		{
			static PlayModeChangeWatcher()
			{
				if (EditorApplication.isPlayingOrWillChangePlaymode)
				{
					PKFxManager.SetSceneMesh(null);
					PKFxManager.UnregisterSceneAllEvents();
					PKFxManager.ResetAndUnloadAllEffects();
				}
				EditorApplication.playModeStateChanged += PlaymodeStateChanged;
			}

			private static void PlaymodeStateChanged(PlayModeStateChange state)
			{
				if (state == PlayModeStateChange.EnteredEditMode)
				{
					Dictionary<int, PKFxEmitter> fxs = new Dictionary<int, PKFxEmitter>(PKFxEmitter.g_ListEffects);
					// Start by clearing the FXs in the C# so they don't get updated
					foreach (KeyValuePair<int, PKFxEmitter> effect in fxs)
					{
						if (effect.Value.Alive)
						{
							effect.Value.KillEffect();
						}
					}

					// Then clear the native side:
					PKFxManager.ClearRenderers();
					PKFxManager.UnregisterSceneAllEvents();
					PKFxManager.ResetAndUnloadAllEffects();

					// We also save the configuration file:
					if (PKFxSettings.AutomaticMeshResizing)
						EditorUtility.SetDirty(PKFxSettings.Instance);
					PKFxManager.StartupPopcorn(true);
					PKFxManager.RestartPackWatcher();
				}
				else if (state == PlayModeStateChange.EnteredPlayMode)
				{
					if (!PKFxSettings.EnableHotreloadInPlayMode)
						PKFxManager.PausePackWatcher();
				}
			}
		}
	}
}