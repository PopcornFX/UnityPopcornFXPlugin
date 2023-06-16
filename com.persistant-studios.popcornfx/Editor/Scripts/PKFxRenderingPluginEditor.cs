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
		bool		m_SceneMeshCategory;
		bool		m_PreloadEffectsCategory;

		GUIContent timeMultiplierLabel = new GUIContent(" Time scale for the particle simulation");
		GUIContent cameraNumberLabel = new GUIContent(" Max number of camera supported");

		SerializedProperty m_TimeMultiplier;
		SerializedProperty m_ShowAdvanced;
		SerializedProperty m_CameraLayers;
		SerializedProperty m_UseSceneMesh;
		SerializedProperty m_SceneMesh;
		SerializedProperty m_PreloadEffect;

		SerializedProperty m_OutputPkmmPath;
		SerializedProperty m_GameObjectsToSearch;
		SerializedProperty m_MeshGameObjects;

		//----------------------------------------------------------------------------

		void OnEnable()
		{
			m_TimeMultiplier = serializedObject.FindProperty("m_TimeMultiplier");
			m_CameraLayers = serializedObject.FindProperty("m_CameraLayers");
			m_PreloadEffect = serializedObject.FindProperty("m_PreloadEffect");
			m_UseSceneMesh = serializedObject.FindProperty("m_UseSceneMesh");
			m_SceneMesh = serializedObject.FindProperty("m_SceneMesh");
			m_OutputPkmmPath = serializedObject.FindProperty("m_OutputPkmmPath");
			m_GameObjectsToSearch = serializedObject.FindProperty("m_GameObjectsToSearch");
			m_MeshGameObjects = serializedObject.FindProperty("m_MeshGameObjects");
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
			PKFxRenderingPlugin renderingPlugin = target as PKFxRenderingPlugin;

			EditorGUILayout.BeginHorizontal();
			EditorGUI.BeginChangeCheck();
			float timeMulValue = EditorGUILayout.Slider(timeMultiplierLabel, m_TimeMultiplier.floatValue, 0.0f, 10.0f);
			if (EditorGUI.EndChangeCheck())
			{
				m_TimeMultiplier.floatValue = timeMulValue;
				PKFxManager.TimeMultiplier = timeMulValue;
			}
			EditorGUILayout.EndHorizontal();

			if (renderingPlugin.MaxCameraSupport() != PKFxSettings.MaxCameraSupport)
			{
				int[] prevCameraLayers = renderingPlugin.CameraLayers;
				int[] newCameraLayers = new int[PKFxSettings.MaxCameraSupport];
				m_CameraLayers.arraySize = PKFxSettings.MaxCameraSupport;

				for (int i = 0; i < PKFxSettings.MaxCameraSupport; i++)
				{
					int cameraLayerID = i < prevCameraLayers.Length ? prevCameraLayers[i] : PKFxSettings.Instance.GetCameraLayer(i);
					m_CameraLayers.GetArrayElementAtIndex(i).intValue = cameraLayerID;
					newCameraLayers[i] = cameraLayerID;
				}
				renderingPlugin.CameraLayers = newCameraLayers;
				EditorUtility.SetDirty(renderingPlugin);
			}
			for (int i = 0; i < renderingPlugin.CameraLayers.Length; i++)
			{
				EditorGUI.BeginChangeCheck();
				int cameraLayerID = EditorGUILayout.LayerField(new GUIContent(" Camera " + i + " layer"), renderingPlugin.CameraLayers[i]);
				if (EditorGUI.EndChangeCheck())
				{
					m_CameraLayers.GetArrayElementAtIndex(i).intValue = cameraLayerID;
					renderingPlugin.CameraLayers[i] = cameraLayerID;
					renderingPlugin.UpdateLayerMask();
				}
			}

			using (var category = new PKFxEditorCategory(() => EditorGUILayout.Foldout(m_PreloadEffectsCategory, "Preload effects setup")))
			{
				m_PreloadEffectsCategory = category.IsExpanded();
				if (category.IsExpanded())
				{
					if (GUILayout.Button("Add all PKFxAssets to preload list"))
					{
						m_PreloadEffect.ClearArray();
						List<PKFxEffectAsset> PKFXList = FindAssetsByType<PKFxEffectAsset>();
						Undo.RecordObject(renderingPlugin, "Add all PKFxAssets to preload list");
						renderingPlugin.m_PreloadEffect = PKFXList;
					}

					EditorGUI.indentLevel += 1;
					EditorGUILayout.PropertyField(m_PreloadEffect);
					EditorGUI.indentLevel -= 1;
				}
			}

			EditorGUI.BeginDisabledGroup(Application.isPlaying);
			using (var category = new PKFxEditorCategory(() => EditorGUILayout.Foldout(m_SceneMeshCategory, "Scene mesh setup")))
			{
				m_SceneMeshCategory = category.IsExpanded();
				if (category.IsExpanded())
				{
					HandleSceneMesh();

					EditorGUILayout.PropertyField(m_OutputPkmmPath);

					EditorGUI.indentLevel += 1;
					EditorGUILayout.PropertyField(m_GameObjectsToSearch);
					EditorGUI.indentLevel -= 1;
					if (GUILayout.Button("Build meshes"))
					{
						FindMeshes();
						BuildMeshes();
					}
				}
			}
			EditorGUI.EndDisabledGroup();

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
					PKFxRaycasts.Clear();
					// Then clear the native side:
					PKFxManager.ClearRenderers();
					PKFxManager.UnregisterSceneAllEvents();
					PKFxManager.ResetAndUnloadAllEffects();

					// We also save the configuration file:
					if (PKFxSettings.AutomaticMeshResizing)
						EditorUtility.SetDirty(PKFxSettings.Instance);
					PKFxManager.StartupPopcorn(true);
					PKFxManager.SetQualityLevelSettings();
					PKFxManager.RestartPackWatcher();
				}
				else if (state == PlayModeStateChange.EnteredPlayMode)
				{
					if (!PKFxSettings.EnableHotreloadInPlayMode)
						PKFxManager.PausePackWatcher();
				}
			}
		}

		//----------------------------------------------------------------------------

		private void FindMeshes()
		{
			this.m_MeshGameObjects.ClearArray();
			for (int i = 0; i < m_GameObjectsToSearch.arraySize; i++)
			{
				FillMeshesWithChildren(m_GameObjectsToSearch.GetArrayElementAtIndex(i).objectReferenceValue as GameObject);
			}
		}

		//----------------------------------------------------------------------------

		private void FillMeshesWithChildren(GameObject o)
		{
			foreach (Transform t in o.transform)
			{
				FillMeshesWithChildren(t.gameObject);
			}

			if (o.GetComponent<MeshFilter>() != null)
			{
				// add gameObject to meshGameObject
				this.m_MeshGameObjects.InsertArrayElementAtIndex(this.m_MeshGameObjects.arraySize);
				this.m_MeshGameObjects.GetArrayElementAtIndex(this.m_MeshGameObjects.arraySize - 1).objectReferenceValue = o as Object;
			}
		}

		//----------------------------------------------------------------------------

		private void BuildMeshes()
		{
			string outputPkmm = m_OutputPkmmPath.stringValue;
			if (outputPkmm.Length <= 0)
			{
				Debug.LogError("[PopcornFX] SceneMeshBuilder: invalid mesh path", this);
				return;
			}
			PKFxManager.StartupPopcorn(false);
			PKFxManager.SetQualityLevelSettings();
			PKFxManager.SetSceneMesh(null);

			// We just need the positions and the normal for the scene mesh:
			int sceneMeshVertexAttributes = (1 << (int)EMeshVertexAttributes.Attrib_Normal);

			for (int meshi = 0; meshi < m_MeshGameObjects.arraySize; meshi++)
			{
				GameObject obj = m_MeshGameObjects.GetArrayElementAtIndex(meshi).objectReferenceValue as GameObject;
				Mesh mesh = obj.GetComponent<MeshFilter>().sharedMesh;

				if (!PKFxManager.AppendMeshToBakeList(mesh, obj.transform, "Default", sceneMeshVertexAttributes, meshi == 0))
				{
					Debug.LogError("[PopcornFX] SceneMeshBuilder: failed to load mesh " + obj.name + "", this);
				}
			}
			// Disabled for the moment as we do not have a PackFx folder in the streaming assets
			bool success = PKFxManager.BakeMeshList(outputPkmm);
			if (!success)
			{
				Debug.LogError("[PopcornFX] SceneMeshBuilder: failed to save scene mesh " + this.name + "", this);
			}
			else
			{
				Debug.Log("[PopcornFX] SceneMeshBuilder: mesh ok " + this.name);
			}
		}
	}
}