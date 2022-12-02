//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using System.IO;
using System.Collections.Generic;
using System;
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
	public class PKFxSettings : ScriptableObject
	{
		public const string kSettingsAssetName = "PKFxSettings";
		public const string kSettingsAssetExtension = ".asset";
		private static PKFxSettings g_Instance;

		public static void SetInstance(PKFxSettings settings)
		{
			g_Instance = settings;
		}

		public static PKFxSettings Instance
		{
			get
			{
				if (g_Instance == null)
				{
					g_Instance = Resources.Load(kSettingsAssetName) as PKFxSettings;
					if (g_Instance == null)
					{
						Debug.LogWarning("[PopcornFX] Can't load PKFxSettings asset, using default values...");
						g_Instance = CreateInstance<PKFxSettings>();
					}
					g_Instance.Setup();
				}
				return g_Instance;
			}
		}

		// General:
		[SerializeField] private bool m_GeneralCategory = true;
		[SerializeField] private bool m_AssetCategory = true;
		[SerializeField] private bool m_EnableRaycastForCollisions = false;
		[SerializeField] private string m_PopcornPackFxPath = null;
		[SerializeField] private string m_UnityPackFxPath = "/PopcornFXAssets";
		[SerializeField] private bool m_EnableForceDeterminism = false;
		[SerializeField] private bool m_EnablePopcornFXLogs = true;
		[SerializeField] private bool m_EnableEffectHotreload = true;
		[SerializeField] private bool m_EnableHotreloadInPlayMode = false;
		[SerializeField] private bool m_EnableFileLogs = false;

		[SerializeField] public string[]	m_PopcornLayerName = new string[5];
		

		public static bool GeneralCategory
		{
			get { return Instance.m_GeneralCategory; }
			set { Instance.m_GeneralCategory = value; }
		}
		public static bool AssetCategory
		{
			get { return Instance.m_AssetCategory; }
			set { Instance.m_AssetCategory = value; }
		}

		public static bool EnableRaycastForCollisions
		{
			get { return Instance.m_EnableRaycastForCollisions; }
			set { Instance.m_EnableRaycastForCollisions = value; }
		}

		public static string PopcornPackFxPath
		{
			get { return Instance.m_PopcornPackFxPath; }
			set
			{
				if (value.CompareTo(Instance.m_PopcornPackFxPath) != 0)
				{
					Instance.m_PopcornPackFxPath = value;
#if UNITY_EDITOR
					PKFxManager.StartupPopcornFileWatcher(EnableEffectHotreload);
#endif
				}
			}
		}

		public static string UnityPackFxPath
		{
			get { return Instance.m_UnityPackFxPath; }
			set
			{
				if (value.CompareTo(Instance.m_UnityPackFxPath) != 0)
				{
					Instance.m_UnityPackFxPath = value;
#if UNITY_EDITOR
					if (!AssetDatabase.IsValidFolder("Assets" + Instance.m_UnityPackFxPath + "/UnityMaterials"))
						AssetDatabase.CreateFolder("Assets" + Instance.m_UnityPackFxPath, "UnityMaterials");
					PKFxManager.StartupPopcornFileWatcher(EnableEffectHotreload);
#endif
				}
			}
		}

		public void GetRenderingLayerForBatchDesc(SBatchDesc batchDesc, out int layer)
		{
			Debug.Assert((batchDesc.m_CameraId >= 0 && batchDesc.m_CameraId < m_PopcornLayerName.Length), "[PKFX] Wrong camera ID feeded to materials factory");


			layer = LayerMask.NameToLayer(m_PopcornLayerName[batchDesc.m_CameraId]);
		}

		internal string[] GetRenderingLayerMaskNames()
		{
			return m_PopcornLayerName;
		}

		public static bool EnableEffectHotreload
		{
			get { return Instance.m_EnableEffectHotreload; }
			set {
				Instance.m_EnableEffectHotreload = value;

#if UNITY_EDITOR
				if (!value)
					PKFxManager.PausePackWatcher();
				else
					PKFxManager.RestartPackWatcher();
#endif
			}
		}

		public static bool EnableHotreloadInPlayMode
		{
			get { return Instance.m_EnableHotreloadInPlayMode; }
			set { Instance.m_EnableHotreloadInPlayMode = value; }
		}

		public static bool EnableForceDeterminism
		{
			get { return Instance.m_EnableForceDeterminism; }
			set
			{
				if (Instance.m_EnableForceDeterminism != value)
				{
					Instance.m_EnableForceDeterminism = value;
#if UNITY_EDITOR
					PKFxManager.SetForceDetermismOnBake(Instance.m_EnableForceDeterminism);
#endif
				}
			}
		}

		public static bool EnablePopcornFXLogs
		{
			get { return Instance.m_EnablePopcornFXLogs; }
			set { Instance.m_EnablePopcornFXLogs = value; }
		}

		public static bool EnableFileLogs
		{
			get { return Instance.m_EnableFileLogs; }
			set { Instance.m_EnableFileLogs = value; }
		}

		public static List<string> AssetPathList
		{
			get; set;
		}

		[Serializable]
		public class CAssetGUID
		{
			public string	m_Path;
			public string	m_New;
			public string	m_Old;

#if UNITY_EDITOR
			public CAssetGUID(string path, GUID newID, GUID oldID)
			{
				m_Path = path;
				m_New = newID.ToString();
				m_Old = oldID.ToString();
			}
#endif
			public CAssetGUID(string path, string newID, string oldID)
			{
				m_Path = path;
				m_New = newID;
				m_Old = oldID;
			}
		}

		[SerializeField] private List<CAssetGUID> m_AssetGUID = new List<CAssetGUID>();

		public static List<CAssetGUID> AssetGUID
		{
			get { return Instance.m_AssetGUID; }
			set { Instance.m_AssetGUID = value; }
		}

#if UNITY_EDITOR

		public static bool GetProjetAssetPath()
		{

			if (Instance.m_PopcornPackFxPath == null || Instance.m_PopcornPackFxPath.Length == 0 || !Directory.Exists(Instance.m_PopcornPackFxPath))
			{
				Debug.LogWarning("[PopcornFX] Source Pack path is required to import your FXs", Instance);
				return false;
			}
			const int assetsOffsets = 7;// "Assets/".Length;
			string[] assetsGUID = AssetDatabase.FindAssets("t:PKFxEffectAsset");
			if (PKFxSettings.AssetPathList == null)
				PKFxSettings.AssetPathList = new List<string>();
			else
				PKFxSettings.AssetPathList.Clear();
			foreach (string guid in assetsGUID)
			{
				GUID id = new GUID(guid);
				string assetPath = AssetDatabase.GUIDToAssetPath(id);
				assetPath = assetPath.Substring(assetsOffsets + Instance.m_UnityPackFxPath.Length);
				assetPath = assetPath.Substring(0, assetPath.LastIndexOf(".asset"));
				PKFxSettings.AssetPathList.Add(assetPath);

			}
			return true;
		}

		public static bool GetAllAssetPath()
		{
			if (Instance.m_PopcornPackFxPath == null || Instance.m_PopcornPackFxPath.Length == 0 || !Directory.Exists(Instance.m_PopcornPackFxPath))
			{
				Debug.LogWarning("[PopcornFX] Source Pack path is required to import your FXs", Instance);
				return false;
			}
			PKFxManager.GetAllAssetPath();
			return true;
		}

		public static bool ReimportAssets(List<string> assetsList)
		{
			if (Instance.m_PopcornPackFxPath == null || Instance.m_PopcornPackFxPath.Length == 0)
			{
				Debug.LogWarning("[PopcornFX] Source Pack path is required to import your FXs", Instance);
				return false;
			}
			PKFxManager.SetForceDetermismOnBake(PKFxSettings.EnableForceDeterminism);
			PKFxManager.ReimportAssets(assetsList);
			return true;
		}

		internal void AddGUIDForAsset(string assetPath, string newGUID)
		{
			CAssetGUID entry = m_AssetGUID.Find(x => x.m_Path.Equals(assetPath));
			if (entry != null)
			{
				if (entry.m_New == newGUID.ToString())
					return;
				entry.m_Old = entry.m_New;
				entry.m_New = newGUID.ToString();
			}
			else
			{
				m_AssetGUID.Add(new CAssetGUID(assetPath, newGUID, newGUID));
			}
		}
		internal void AddGUIDForAsset(string assetPath, GUID newGUID)
		{
			CAssetGUID entry = m_AssetGUID.Find(x => x.m_Path.Equals(assetPath));
			if (entry != null)
			{
				if (entry.m_New == newGUID.ToString())
					return;
				entry.m_Old = entry.m_New;
				entry.m_New = newGUID.ToString();
			}
			else
			{
				m_AssetGUID.Add(new CAssetGUID(assetPath, newGUID, newGUID));
			}
		}

#endif

		// Rendering
		[System.Serializable]
		public class SParticleMeshDefaultSize
		{
			public string m_GeneratedName;
			public int m_DefaultVertexBufferSize;
			public int m_DefaultIndexBufferSize;

			public Bounds m_StaticWorldBounds = new Bounds(Vector3.zero, new Vector3(100, 100, 100));
		}

		[SerializeField] private bool m_RenderingCategory = false;
		[SerializeField] private bool m_DebugEffectsBoundingBoxes = false;
		[SerializeField] private bool m_DebugEffectsRaycasts = false;
		[SerializeField] private PKFxMaterialFactory m_MaterialFactory = null;
		[SerializeField] private bool m_EnableSoftParticles = true;
		[SerializeField] private bool m_DisableDynamicEffectBounds = false;
		[SerializeField] private bool m_UseGPUBillboarding = false;
		[SerializeField] private bool m_UseMeshInstancing = true;

		[SerializeField] private bool m_EnableLocalizedPages = true;
		[SerializeField] private bool m_EnableLocalizedByDefault = false;

		[SerializeField] private bool m_FreeUnusedBatches = false;
		[SerializeField] private uint m_FrameCountBeforeFreeingUnusedBatches = 240;

		[SerializeField] private bool m_AutomaticMeshResizing = true;
		[SerializeField] private float m_VertexBufferSizeMultiplicator = 0.5f;
		[SerializeField] private float m_IndexBufferSizeMultiplicator = 0.5f;
		[SerializeField] private List<SParticleMeshDefaultSize> m_MeshesDefaultSize = new List<SParticleMeshDefaultSize>();
		[SerializeField] private bool m_UseHashesAsMaterialName = true;

		[SerializeField] private bool m_UpdateSimManually = false;

		public static bool RenderingCategory
		{
			get { return Instance.m_RenderingCategory; }
			set { Instance.m_RenderingCategory = value; }
		}

		public static bool DebugEffectsBoundingBoxes
		{
			get { return Instance.m_DebugEffectsBoundingBoxes; }
			set { Instance.m_DebugEffectsBoundingBoxes = value; }
		}

		public static bool DebugEffectsRaycasts
		{
			get { return Instance.m_DebugEffectsRaycasts; }
			set { Instance.m_DebugEffectsRaycasts = value; }
		}

		public static bool EnableLocalizedPages
		{
			get { return Instance.m_EnableLocalizedPages; }
			set { Instance.m_EnableLocalizedPages = value; }
		}

		public static bool EnableLocalizedByDefault
		{
			get { return Instance.m_EnableLocalizedByDefault; }
			set { Instance.m_EnableLocalizedByDefault = value; }
		}

		public static bool FreeUnusedBatches
		{
			get { return Instance.m_FreeUnusedBatches; }
			set { Instance.m_FreeUnusedBatches = value; }
		}

		public static uint FrameCountBeforeFreeingUnusedBatches
		{
			get { return Instance.m_FrameCountBeforeFreeingUnusedBatches; }
			set { Instance.m_FrameCountBeforeFreeingUnusedBatches = value; }
		}

		public static PKFxMaterialFactory MaterialFactory
		{
			get
			{
				if (Instance.m_MaterialFactory == null)
				{
					PKFxMaterialFactoryDefault materialFactoryDefault = PKFxMaterialFactoryDefault.CreateMaterialFactoryDefaultAsset();

					Instance.m_MaterialFactory = materialFactoryDefault;
				}
				return Instance.m_MaterialFactory;
			}
			set { Instance.m_MaterialFactory = value; }
		}

		public static bool EnableSoftParticles
		{
			get { return Instance.m_EnableSoftParticles; }
			set { Instance.m_EnableSoftParticles = value; }
		}

		public static bool DisableDynamicEffectBounds
		{
			get { return Instance.m_DisableDynamicEffectBounds; }
			set { Instance.m_DisableDynamicEffectBounds = value; }
		}

		public static bool UseGPUBillboarding
		{
			get { return Instance.m_UseGPUBillboarding; }
			set { Instance.m_UseGPUBillboarding = value; }
		}

		public static bool UseMeshInstancing
		{
			get { return Instance.m_UseMeshInstancing; }
			set { Instance.m_UseMeshInstancing = value; }
		}

		public static bool AutomaticMeshResizing
		{
			get { return Instance.m_AutomaticMeshResizing; }
			set { Instance.m_AutomaticMeshResizing = value; }
		}

		public static float VertexBufferSizeMultiplicator
		{
			get { return Instance.m_VertexBufferSizeMultiplicator; }
			set { Instance.m_VertexBufferSizeMultiplicator = value; }
		}

		public static float IndexBufferSizeMultiplicator
		{
			get { return Instance.m_IndexBufferSizeMultiplicator; }
			set { Instance.m_IndexBufferSizeMultiplicator = value; }
		}

		public static List<SParticleMeshDefaultSize> MeshesDefaultSize
		{
			get { return Instance.m_MeshesDefaultSize; }
			set { Instance.m_MeshesDefaultSize = value; }
		}

		// Threading:
		[SerializeField] private bool m_ThreadingCategory = false;
		[SerializeField] private bool m_SingleThreadedExecution = false;
		[SerializeField] private bool m_OverrideThreadPoolConfig = false;
		[SerializeField] private List<int> m_ThreadsAffinity = new List<int>();

		public static bool ThreadingCategory
		{
			get { return Instance.m_ThreadingCategory; }
			set { Instance.m_ThreadingCategory = value; }
		}

		public static bool SingleThreadedExecution
		{
			get { return Instance.m_SingleThreadedExecution; }
			set { Instance.m_SingleThreadedExecution = value; }
		}

		public static bool OverrideThreadPoolConfig
		{
			get { return Instance.m_OverrideThreadPoolConfig; }
			set { Instance.m_OverrideThreadPoolConfig = value; }
		}

		public static List<int> ThreadsAffinity
		{
			get { return Instance.m_ThreadsAffinity; }
			set { Instance.m_ThreadsAffinity = value; }
		}

		public static bool UseHashesAsMaterialName
		{
			get { return Instance.m_UseHashesAsMaterialName; }
			set { Instance.m_UseHashesAsMaterialName = value; }
		}

		public static bool UpdateSimManually
		{
			get { return Instance.m_UpdateSimManually; }
			set { Instance.m_UpdateSimManually = value; }
		}

		public void Setup()
		{
#if UNITY_EDITOR
			if (PopcornPackFxPath == null || !Directory.Exists(PopcornPackFxPath))
				Debug.Log("[PopcornFX] Valid Source Pack path is required to import your FXs", this);
#endif
		}
	}
}