//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using UnityEditor;
using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

namespace PopcornFX
{
	[CustomEditor(typeof(PKFxSettings))]
	public class PKFxSettingsEditor : Editor
	{
		[MenuItem("Assets/Create/PopcornFX/PopcornFX Settings")]
		static void CreateMaterialFactoryDefault()
		{
			PKFxSettings settings = PKFxSettings.Instance;
			AssetDatabase.SaveAssets();

			EditorUtility.FocusProjectWindow();

			Selection.activeObject = settings;
		}

		GUIContent forceDeterminismLabel = new GUIContent(" Enable force determinism while baking asset");
		GUIContent hotreloadLabel = new GUIContent(" Enable hot reloading effects from source pack");
		GUIContent hotreloadInPlayLabel = new GUIContent(" Enable hot reloading effects while in play mode");
		GUIContent debugEffectsBoundingBoxes = new GUIContent(" Debug the effects bounding boxes");
		GUIContent debugEffectsRaycasts = new GUIContent(" Debug the effects raycasts");
		GUIContent enableSoftParticlesLabel = new GUIContent(" Enable soft particles");
		GUIContent enableUnityConsoleLog = new GUIContent(" Enable PopcornFX Unity console logs");
		GUIContent enableFileLog = new GUIContent(" Enable PopcornFX file logs");
		GUIContent enableRaycastForCollisionsLabel = new GUIContent(" Enable raycast for collisions");
		GUIContent splitDrawCallsOfSoubleSidedParticlesLabel = new GUIContent(" Split the draw calls of the particles that require disabling the back-face culling");
		GUIContent disableDynamicEffectBoundsLabel = new GUIContent(" Disable dynamic effect bounds");
		GUIContent materialFactoryLabel = new GUIContent(" Material Factory");
		GUIContent useHashAsMaterialNameLabel = new GUIContent(" Use hashes as material name");
		GUIContent useHashAsMaterialNameDialogMessageLabel = new GUIContent("Warning, this action will delete all existing generated materials and recreate them. \nContinue ?");

		GUIContent enableLocalizedPages = new GUIContent(" Enable Localized Pages");
		GUIContent enableLocalizedPagesByDefault = new GUIContent(" Enable Localized Pages By Default");

		GUIContent freeUnusedBactchesLabel = new GUIContent(" Free unused batches");
		GUIContent frameCountBeforeFreeUnusedBactchesLabel = new GUIContent(" Frame count before freeing unused batches");

#if UNITY_2017 || UNITY_2018
	GUIContent useGPUBillboarding = new GUIContent(" Enable GPU Billboarding -- Not compatible with your Unity version --");
#else
		GUIContent useGPUBillboarding = new GUIContent(" Enable GPU Billboarding (experimental)");
#endif

		GUIContent useMeshInstancingLabel = new GUIContent(" Enable Mesh Instancing (per instance color in not supported by URP/HDRP yet)");

		GUIContent singleThreadedExecLabel = new GUIContent(" Run PopcornFX on a single thread to avoid visual studio hangs");
		GUIContent splitUpdateInComponentsLabel = new GUIContent(" Splits the update in 3 components");
		GUIContent waitForUpdateOnRenderThreadLabel = new GUIContent(" Wait for the end of the particle update on the render thread");
		GUIContent overrideThreadPoolConfigLabel = new GUIContent(" Override the PopcornFX thread pool configuration");
		GUIContent automaticMeshResizingLabel = new GUIContent(" Automatic mesh resizing to avoid dynamic re-alloc during the next run");
		GUIContent vertexBufferSizeMultiplicatorLabel = new GUIContent(" Vertex buffer size multiplicator");
		GUIContent indexBufferSizeMultiplicatorLabel = new GUIContent(" Index buffer size multiplicator");

		bool	showBuffers = true;


		private static void _AddCameraLayersIFN()
		{
			UnityEngine.Object[] tagManager = AssetDatabase.LoadAllAssetsAtPath("ProjectSettings/TagManager.asset");

			if (tagManager.Length == 0)
				return;

			SerializedObject	tagsAndLayersManager = new SerializedObject(tagManager[0]);
			SerializedProperty	layersProp = tagsAndLayersManager.FindProperty("layers");

			List<string>		cameraLayerName = new List<string>(new string[] {"PopcornFX_0", "PopcornFX_1", "PopcornFX_2", "PopcornFX_3", "PopcornFX_Disto" });

			if (PKFxSettings.Instance.m_PopcornLayerName.Count() < cameraLayerName.Count())
				PKFxSettings.Instance.m_PopcornLayerName = new string[cameraLayerName.Count()];

			//serch if the sorting layer already exist
			for (int i = 0; i < layersProp.arraySize; ++i)
			{
				SerializedProperty layer = layersProp.GetArrayElementAtIndex(i);

				if (layer != null && layer.stringValue.Length != 0)
				{
					if (cameraLayerName.Contains(layer.stringValue))
					{
						int idx = cameraLayerName.IndexOf(layer.stringValue);

						PKFxSettings.Instance.m_PopcornLayerName[cameraLayerName.IndexOf(layer.stringValue)] = layer.stringValue;
						cameraLayerName[idx] = "";
					}
						
				}
			}
			cameraLayerName = cameraLayerName.Where(s => !string.IsNullOrWhiteSpace(s)).ToList();

			if (cameraLayerName.Count == 0)
				return;

			for (int i = layersProp.arraySize -1; i >= 0 ; --i)
			{
				SerializedProperty layer = layersProp.GetArrayElementAtIndex(i);

				if (layer != null && layer.stringValue.Length == 0)
				{
					layer.stringValue = cameraLayerName[0];

					PKFxSettings.Instance.m_PopcornLayerName[cameraLayerName.IndexOf(layer.stringValue)] = layer.stringValue;

					cameraLayerName.RemoveAt(0);
					if (cameraLayerName.Count == 0)
					{
						break;
					}
				}
			}
		
			tagsAndLayersManager.ApplyModifiedProperties();

		}

		private static void _AddSortingLayerIFN(string layerName, bool isFirst)
		{
			UnityEngine.Object[] tagManager = AssetDatabase.LoadAllAssetsAtPath("ProjectSettings/TagManager.asset");

			if (tagManager.Length == 0)
				return;

			SerializedObject tagsAndLayersManager = new SerializedObject(tagManager[0]);
			SerializedProperty sortingLayersProp = tagsAndLayersManager.FindProperty("m_SortingLayers");

			//serch if the sorting layer already exist
			for (int i = 0; i < sortingLayersProp.arraySize; ++i)
			{
				var layer = sortingLayersProp.GetArrayElementAtIndex(i);
				if (layer.FindPropertyRelative("name").stringValue == layerName)
					return;
			}

			//insert the sorting layer
			int index = 0;
			if (!isFirst)
				index = sortingLayersProp.arraySize;
			sortingLayersProp.InsertArrayElementAtIndex(index);
			var newlayer = sortingLayersProp.GetArrayElementAtIndex(index);
			newlayer.FindPropertyRelative("uniqueID").intValue = UnityEngine.Random.Range(int.MinValue, int.MaxValue);
			newlayer.FindPropertyRelative("name").stringValue = layerName;
			tagsAndLayersManager.ApplyModifiedProperties();
			string beforeOrAfter = isFirst ? "before" : "after";
			Debug.Log("[PopcornFX] Adding the sorting layer \"" + layerName + "\" " + beforeOrAfter + " \"Default\"");
		}

		private static void _CheckForMultipleSettingsObject(PKFxSettings instance)
		{
			string[] guids = AssetDatabase.FindAssets("t:PKFxSettings");
			if (guids.Length > 1)
			{
				string instancePath = AssetDatabase.GetAssetPath(instance);
				string objPaths = "";
				foreach (var guid in guids)
				{
					string path = AssetDatabase.GUIDToAssetPath(guid);
					if (path != instancePath)
						objPaths += " - " + path + "\n";
				}
				Debug.Assert(false, "[PopcornFX] Found more than one PKFxSettings object, loading " + instancePath + "\nThis objects will not be used: " + objPaths);
			}
		}

		public static PKFxSettings GetOrCreateSettingsAsset()
		{
			string settingsPath = "Assets/Resources/";

			PKFxSettings instance = Resources.Load<PKFxSettings>(PKFxSettings.kSettingsAssetName);

			_CheckForMultipleSettingsObject(instance);

			if (instance == null)
			{
				string fullPath = Path.Combine(settingsPath, PKFxSettings.kSettingsAssetName + PKFxSettings.kSettingsAssetExtension);

				instance = CreateInstance<PKFxSettings>();


				Directory.CreateDirectory(fullPath);
				AssetDatabase.CreateAsset(instance, fullPath);

				AssetDatabase.SaveAssets();
			}

			PKFxSettings.SetInstance(instance);
			PKFxMaterialFactory factory = PKFxSettings.MaterialFactory;
			if (factory == null)
			{
				Debug.Assert(false, "[PopcornFX] Unable to create or find the Material factory, use create menu and assign it the the PKFxSettings");
			}
			return instance;
		}

		void OnDisable()
		{
			// make sure the runtime code will load the Asset from Resources when it next tries to access this. 
			PKFxSettings.SetInstance(null);
		}

		[InitializeOnLoadMethod]
		private static void CreateSettingsAssetIFN()
		{
			// The assets are not imported yet here when opening the project for the first time
			// We instead initialize the settings on the first editor update to make sure we have access to all assets
			EditorApplication.update += OnFirstEditorUpdate;
		}

		static public void OnFirstEditorUpdate()
		{
			if (UnityEditorInternal.InternalEditorUtility.inBatchMode)
				return;

			GetOrCreateSettingsAsset();

			_AddCameraLayersIFN();
			_AddSortingLayerIFN("PopcornFX", true);
			_AddSortingLayerIFN("PopcornFXUI", false);

			EditorApplication.update -= OnFirstEditorUpdate;
		}

		public override void OnInspectorGUI()
		{
			PKFxSettings settings = (PKFxSettings)target;
			PKFxSettings.SetInstance(settings);

			using (var category = new PKFxEditorCategory(() => EditorGUILayout.Foldout(PKFxSettings.GeneralCategory, "General")))
			{
				PKFxSettings.GeneralCategory = category.IsExpanded();
				if (category.IsExpanded())
				{
					DisplayGeneralCategory(category);
				}
			}
			using (var category = new PKFxEditorCategory(() => EditorGUILayout.Foldout(PKFxSettings.RenderingCategory, "Rendering")))
			{
				PKFxSettings.RenderingCategory = category.IsExpanded();
				if (category.IsExpanded())
				{
					DisplayRenderingCategory(category);
				}
			}
			using (var category = new PKFxEditorCategory(() => EditorGUILayout.Foldout(PKFxSettings.ThreadingCategory, "Multithreading")))
			{
				PKFxSettings.ThreadingCategory = category.IsExpanded();
				if (category.IsExpanded())
				{
					DisplayThreadingCategory();
				}
			}

			using (var category = new PKFxEditorCategory(() => EditorGUILayout.Foldout(PKFxSettings.AssetCategory, "Assets")))
			{
				PKFxSettings.AssetCategory = category.IsExpanded();
				if (category.IsExpanded())
				{
					EditorGUILayout.BeginVertical();
					EditorGUILayout.BeginHorizontal();
					if (GUILayout.Button("Update Asset List"))
					{
						string[] assets = AssetDatabase.FindAssets("t:PKFxEffectAsset");
						foreach (string id in assets)
						{
							string path = AssetDatabase.GUIDToAssetPath(id);
							PKFxEffectAsset obj = AssetDatabase.LoadAssetAtPath(path, typeof(PKFxEffectAsset)) as PKFxEffectAsset;
							if (obj != null)
							{
								PKFxSettings.Instance.AddGUIDForAsset(obj.AssetFullPath + ".asset", id);
							}
						}
					}
					if (GUILayout.Button("Clear Asset List"))
					{
						PKFxSettings.AssetGUID.Clear();
					}
					var AssetGUID = PKFxSettings.AssetGUID;
					EditorGUILayout.EndHorizontal();

					GUIStyle boldStyle = new GUIStyle();
					boldStyle.fontStyle = FontStyle.Bold;
					boldStyle.normal.textColor = Color.white;
					boldStyle.hover.textColor = Color.white;

					GUIStyle greyedStyle = new GUIStyle();
					greyedStyle.fontStyle = FontStyle.Normal;
					greyedStyle.normal.textColor = Color.gray;
					greyedStyle.hover.textColor = Color.gray;

					foreach (var guid in AssetGUID)
					{
						EditorGUILayout.BeginHorizontal();
						EditorGUILayout.LabelField(guid.m_Path, boldStyle);
						EditorGUILayout.EndHorizontal();
						EditorGUI.indentLevel += 2;
						if (guid.m_New != guid.m_Old)
						{
							EditorGUILayout.BeginHorizontal();
							EditorGUILayout.LabelField("Previous GUID:\t" + guid.m_Old.ToString(), greyedStyle);
							EditorGUILayout.EndHorizontal();
						}
						EditorGUILayout.BeginHorizontal();
						EditorGUILayout.LabelField("Current GUID:\t" + guid.m_New.ToString());
						EditorGUILayout.EndHorizontal();
						EditorGUI.indentLevel -= 2;
					}
					EditorGUILayout.EndVertical();
				}
			}

			if (GUI.changed)
				EditorUtility.SetDirty(settings);
		}

		private void DisplayGeneralCategory(PKFxEditorCategory category)
		{
			GUIStyle boldStyleRed = new GUIStyle();
			boldStyleRed.fontStyle = FontStyle.Bold;
			boldStyleRed.normal.textColor = Color.red;
			boldStyleRed.hover.textColor = Color.red;

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.EnableRaycastForCollisions = EditorGUILayout.ToggleLeft(enableRaycastForCollisionsLabel, PKFxSettings.EnableRaycastForCollisions);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			bool hotreload = EditorGUILayout.ToggleLeft(hotreloadLabel, PKFxSettings.EnableEffectHotreload);
			if (hotreload != PKFxSettings.EnableEffectHotreload)
				PKFxSettings.EnableEffectHotreload = hotreload;
			EditorGUILayout.EndHorizontal();

			EditorGUI.BeginDisabledGroup(!PKFxSettings.EnableEffectHotreload);

			EditorGUILayout.BeginHorizontal();
			bool hotreloadInPlay = EditorGUILayout.ToggleLeft(hotreloadInPlayLabel, PKFxSettings.EnableHotreloadInPlayMode);
			if (hotreloadInPlay != PKFxSettings.EnableHotreloadInPlayMode)
				PKFxSettings.EnableHotreloadInPlayMode = hotreloadInPlay;
			EditorGUILayout.EndHorizontal();

			EditorGUI.EndDisabledGroup();

			EditorGUILayout.BeginHorizontal();
			{
				EditorGUILayout.BeginVertical();
				{
					if (!string.IsNullOrEmpty(PKFxSettings.PopcornPackFxPath))
					{
						EditorGUILayout.LabelField(PKFxSettings.PopcornPackFxPath);
					}
					else
					{
						EditorGUILayout.LabelField("<empty>", boldStyleRed);
					}

					if (!string.IsNullOrEmpty(PKFxSettings.UnityPackFxPath))
					{
						EditorGUILayout.LabelField(PKFxSettings.UnityPackFxPath);
					}
					else
					{
						EditorGUILayout.LabelField("<empty>", boldStyleRed);
					}
					EditorGUILayout.LabelField("Reimport Pack");
				}
				EditorGUILayout.EndVertical();

				EditorGUILayout.BeginVertical();
				{
					if (GUILayout.Button("Source Effects Pack"))
					{
						category.EndCb = () =>
						{
							string from = Directory.Exists(PKFxSettings.PopcornPackFxPath) ? PKFxSettings.PopcornPackFxPath : "";
							string path = EditorUtility.OpenFilePanelWithFilters("Choose PopcornFx Asset Folder", from, new[] { "PopcornFX Project", "pkproj" });
							if (File.Exists(path))
							{
								string folderPath = path.Remove(path.LastIndexOf('/'));
								Uri fileUri = new Uri(folderPath);
								Uri referenceUri = new Uri(Application.dataPath);
								Uri relative = referenceUri.MakeRelativeUri(fileUri);
								string Srcpath = referenceUri.MakeRelativeUri(fileUri).ToString();

								PKFxSettings.PopcornPackFxPath = Uri.UnescapeDataString(Srcpath);

							}
						};
					}

					if (GUILayout.Button("Baked Effects Path"))
					{
						category.EndCb = () =>
						{
							string path = EditorUtility.OpenFolderPanel("Choose Unity Fx Assets Folder", "Resources", "");
							if (Directory.Exists(path))
							{
								PKFxSettings.UnityPackFxPath = path.Substring(Application.dataPath.Length);
							}
						};
					}

					if (GUILayout.Button("Choose files"))
					{
						category.EndCb = () =>
						{
							if (PKFxSettings.UnityPackFxPath.Length != 0 && PKFxSettings.GetAllAssetPath())
							{
								ImportPKFxListEditor window = EditorWindow.GetWindow<ImportPKFxListEditor>();
								window.ShowUtility();
							}
						};
					}
					if (GUILayout.Button("All"))
					{
						category.EndCb = () =>
						{
							if (PKFxSettings.UnityPackFxPath.Length != 0 && PKFxSettings.GetAllAssetPath())
							{
								PKFxSettings.ReimportAssets(PKFxSettings.AssetPathList);
							}
						};
					}

				}
				EditorGUILayout.EndVertical();

			}
			EditorGUILayout.EndHorizontal();



#if UNITY_EDITOR_OSX
		EditorGUILayout.BeginHorizontal();
		EditorGUILayout.LabelField("Bake of PopcornFX Effects inside Unity MacOS is disabled.", boldStyleRed);
		EditorGUILayout.EndHorizontal();
#else


#endif

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.EnableForceDeterminism = EditorGUILayout.ToggleLeft(forceDeterminismLabel, PKFxSettings.EnableForceDeterminism);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.EnablePopcornFXLogs = EditorGUILayout.ToggleLeft(enableUnityConsoleLog, PKFxSettings.EnablePopcornFXLogs);
			EditorGUILayout.EndHorizontal();
			EditorGUILayout.BeginHorizontal();
			PKFxSettings.EnableFileLogs = EditorGUILayout.ToggleLeft(enableFileLog, PKFxSettings.EnableFileLogs);
			EditorGUILayout.EndHorizontal();
		}

		private void DisplayRenderingCategory(PKFxEditorCategory category)
		{
			EditorGUILayout.BeginHorizontal();
			PKFxSettings.MaterialFactory = EditorGUILayout.ObjectField(materialFactoryLabel, PKFxSettings.MaterialFactory, typeof(PKFxMaterialFactory), false) as PKFxMaterialFactory;
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			bool value = EditorGUILayout.ToggleLeft(useHashAsMaterialNameLabel, PKFxSettings.UseHashesAsMaterialName);
			EditorGUILayout.EndHorizontal();

			if (PKFxSettings.UseHashesAsMaterialName != value)
			{
				category.EndCb = () =>
				{
					if (EditorUtility.DisplayDialog(useHashAsMaterialNameLabel.text, useHashAsMaterialNameDialogMessageLabel.text, "Yes", "No"))
					{
						PKFxSettings.UseHashesAsMaterialName = value;

						if (AssetDatabase.IsValidFolder("Assets" + PKFxSettings.UnityPackFxPath + "/UnityMaterials"))
							AssetDatabase.DeleteAsset("Assets" + PKFxSettings.UnityPackFxPath + "/UnityMaterials");
						PKFxMenus.CreatePKFxFXMaterialsIFN();
					}
				};
			}

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.DebugEffectsBoundingBoxes = EditorGUILayout.ToggleLeft(debugEffectsBoundingBoxes, PKFxSettings.DebugEffectsBoundingBoxes);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.DebugEffectsRaycasts = EditorGUILayout.ToggleLeft(debugEffectsRaycasts, PKFxSettings.DebugEffectsRaycasts);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.EnableSoftParticles = EditorGUILayout.ToggleLeft(enableSoftParticlesLabel, PKFxSettings.EnableSoftParticles);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.DisableDynamicEffectBounds = EditorGUILayout.ToggleLeft(disableDynamicEffectBoundsLabel, PKFxSettings.DisableDynamicEffectBounds);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.UseGPUBillboarding = EditorGUILayout.ToggleLeft(useGPUBillboarding, PKFxSettings.UseGPUBillboarding);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.UseMeshInstancing = EditorGUILayout.ToggleLeft(useMeshInstancingLabel, PKFxSettings.UseMeshInstancing);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.EnableLocalizedPages = EditorGUILayout.ToggleLeft(enableLocalizedPages, PKFxSettings.EnableLocalizedPages);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.EnableLocalizedByDefault = EditorGUILayout.ToggleLeft(enableLocalizedPagesByDefault, PKFxSettings.EnableLocalizedByDefault);
			EditorGUILayout.EndHorizontal();

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.FreeUnusedBatches = EditorGUILayout.ToggleLeft(freeUnusedBactchesLabel, PKFxSettings.FreeUnusedBatches);
			EditorGUILayout.EndHorizontal();

			using (new EditorGUI.DisabledScope(!PKFxSettings.FreeUnusedBatches))
			{
				EditorGUI.indentLevel++;

				EditorGUILayout.BeginHorizontal();
				int frameCountBeforeFreeingBatch = (int)PKFxSettings.FrameCountBeforeFreeingUnusedBatches;
				frameCountBeforeFreeingBatch = EditorGUILayout.IntField(frameCountBeforeFreeUnusedBactchesLabel, frameCountBeforeFreeingBatch);
				frameCountBeforeFreeingBatch = Math.Max(frameCountBeforeFreeingBatch, 1);
				PKFxSettings.FrameCountBeforeFreeingUnusedBatches = (uint)frameCountBeforeFreeingBatch;
				EditorGUILayout.EndHorizontal();

				EditorGUI.indentLevel--;
			}

			EditorGUILayout.BeginHorizontal();
			PKFxSettings.AutomaticMeshResizing = EditorGUILayout.ToggleLeft(automaticMeshResizingLabel, PKFxSettings.AutomaticMeshResizing);
			EditorGUILayout.EndHorizontal();

			using (new EditorGUI.DisabledScope(!PKFxSettings.AutomaticMeshResizing))
			{
				EditorGUI.indentLevel++;

				EditorGUILayout.BeginHorizontal();
				PKFxSettings.VertexBufferSizeMultiplicator = EditorGUILayout.FloatField(vertexBufferSizeMultiplicatorLabel, PKFxSettings.VertexBufferSizeMultiplicator);
				EditorGUILayout.EndHorizontal();

				EditorGUILayout.BeginHorizontal();
				PKFxSettings.IndexBufferSizeMultiplicator = EditorGUILayout.FloatField(indexBufferSizeMultiplicatorLabel, PKFxSettings.IndexBufferSizeMultiplicator);
				EditorGUILayout.EndHorizontal();

				EditorGUI.indentLevel--;
			}
			DisplayPopcornFxRenderers();

		}

		private void DisplayThreadingCategory()
		{
			EditorGUILayout.BeginHorizontal();
			PKFxSettings.SingleThreadedExecution = EditorGUILayout.ToggleLeft(singleThreadedExecLabel, PKFxSettings.SingleThreadedExecution);
			EditorGUILayout.EndHorizontal();

			using (new EditorGUI.DisabledScope(PKFxSettings.SingleThreadedExecution))
			{
				EditorGUILayout.BeginHorizontal();
				PKFxSettings.OverrideThreadPoolConfig = EditorGUILayout.ToggleLeft(overrideThreadPoolConfigLabel, PKFxSettings.OverrideThreadPoolConfig);
				EditorGUILayout.EndHorizontal();
			}
			DisplayThreadAffinities();
		}

		private Vector2 m_AffinityScrollPosition = new Vector2();

		private void DisplayThreadAffinities()
		{
			GUIStyle scrollViewStyle = new GUIStyle();
			scrollViewStyle.stretchHeight = true;
			scrollViewStyle.stretchWidth = true;

			GUIStyle boldStyleWhite = new GUIStyle();
			boldStyleWhite.fontStyle = FontStyle.Bold;
			boldStyleWhite.normal.textColor = Color.white;
			boldStyleWhite.hover.textColor = Color.white;

			GUIStyle boldStyleGreen = new GUIStyle();
			boldStyleGreen.fontStyle = FontStyle.Bold;
			boldStyleGreen.normal.textColor = Color.green;
			boldStyleGreen.hover.textColor = Color.green;

			GUIStyle boldStyleRed = new GUIStyle();
			boldStyleRed.fontStyle = FontStyle.Bold;
			boldStyleRed.normal.textColor = Color.red;
			boldStyleRed.hover.textColor = Color.red;

			using (new EditorGUI.DisabledScope(PKFxSettings.SingleThreadedExecution || !PKFxSettings.OverrideThreadPoolConfig))
			{
				int workerThreads = Mathf.Max(EditorGUILayout.DelayedIntField("Worker threads count", PKFxSettings.ThreadsAffinity.Count), 0);

				while (workerThreads > PKFxSettings.ThreadsAffinity.Count)
					PKFxSettings.ThreadsAffinity.Add(~0);
				while (workerThreads < PKFxSettings.ThreadsAffinity.Count)
					PKFxSettings.ThreadsAffinity.RemoveAt(PKFxSettings.ThreadsAffinity.Count - 1);

				m_AffinityScrollPosition = EditorGUILayout.BeginScrollView(m_AffinityScrollPosition, scrollViewStyle);

				for (int i = 0; i < PKFxSettings.ThreadsAffinity.Count; ++i)
				{
					int affinity = PKFxSettings.ThreadsAffinity[i];

					EditorGUILayout.LabelField("Affinity for Worker " + i + ":", affinity == 0 ? boldStyleRed : boldStyleGreen);

					EditorGUILayout.BeginHorizontal();

					if (EditorGUILayout.ToggleLeft("Full affinity", affinity == ~0, boldStyleWhite, GUILayout.Width(100)))
						affinity = ~0;
					else if (affinity == ~0)
						affinity = 0;

					for (int b = 0; b < 32; ++b)
					{
						bool curValue = (affinity & (1 << b)) != 0;
						bool nextValue = EditorGUILayout.ToggleLeft(b.ToString(), curValue, GUILayout.Width(35));

						if (nextValue)
						{
							affinity |= (1 << b);
						}
						else if (curValue)
						{
							affinity ^= (1 << b);
						}
					}
					EditorGUILayout.EndHorizontal();

					PKFxSettings.ThreadsAffinity[i] = affinity;
				}
				EditorGUILayout.EndScrollView();
			}
		}

		private Vector2 m_BuffersScrollPosition = new Vector2();

		private void DisplayPopcornFxRenderers()
		{
			GUIStyle scrollViewStyle = new GUIStyle();
			scrollViewStyle.stretchHeight = true;
			scrollViewStyle.stretchWidth = true;

			GUIStyle boldStyleBlack = new GUIStyle();
			boldStyleBlack.fontStyle = FontStyle.Bold;

			GUIStyle italicStyle = new GUIStyle();
			italicStyle.fontStyle = FontStyle.Italic;

			++EditorGUI.indentLevel;
			showBuffers = EditorGUILayout.Foldout(showBuffers, "Stored PopcornFX Buffers");
			if (showBuffers)
			{
				if (GUILayout.Button("Clear all particle meshes"))
				{
					PKFxSettings.MeshesDefaultSize.Clear();
				}
				m_BuffersScrollPosition = EditorGUILayout.BeginScrollView(m_BuffersScrollPosition, scrollViewStyle);

				using (new EditorGUI.DisabledScope(true))
				{
					foreach (PKFxSettings.SParticleMeshDefaultSize particleMesh in PKFxSettings.MeshesDefaultSize)
					{
						EditorGUILayout.LabelField(particleMesh.m_GeneratedName, boldStyleBlack);

						EditorGUILayout.BeginVertical();
						++EditorGUI.indentLevel;
						particleMesh.m_DefaultVertexBufferSize = EditorGUILayout.IntField("Vertex count", particleMesh.m_DefaultVertexBufferSize);
						particleMesh.m_DefaultIndexBufferSize = EditorGUILayout.IntField("Index count", particleMesh.m_DefaultIndexBufferSize);

						if (PKFxSettings.DisableDynamicEffectBounds)
						{
							particleMesh.m_StaticWorldBounds.center = EditorGUILayout.Vector3Field("Static world bounds center", particleMesh.m_StaticWorldBounds.center);
							particleMesh.m_StaticWorldBounds.size = EditorGUILayout.Vector3Field("Static world bounds size", particleMesh.m_StaticWorldBounds.size);
						}

						--EditorGUI.indentLevel;
						EditorGUILayout.EndVertical();
					}
				}
				EditorGUILayout.EndScrollView();
			}
			--EditorGUI.indentLevel;
		}

		private static void ShowInspector()
		{
			try
			{
				var editorAsm = typeof(UnityEditor.Editor).Assembly;
				var type = editorAsm.GetType("UnityEditor.InspectorWindow");
				UnityEngine.Object[] findObjectsOfTypeAll = Resources.FindObjectsOfTypeAll(type);

				if (findObjectsOfTypeAll.Length > 0)
				{
					((EditorWindow)findObjectsOfTypeAll[0]).Focus();
				}
				else
				{
					EditorWindow.GetWindow(type);
				}
			}
			catch
			{
			}
		}
	}
}