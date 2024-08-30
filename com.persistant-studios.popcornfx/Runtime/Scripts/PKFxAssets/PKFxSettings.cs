//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using System.IO;
using System.Collections.Generic;
using System;
using UnityEngine.Rendering;
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
	public class PKFxSettings : ScriptableObject
	{
		public enum ERenderPipeline
		{
			Legacy,
			URP	  ,
			HDRP
		}


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
		[SerializeField] private bool m_QualityCategory = true;

		[SerializeField] private bool m_EnableRaycastForCollisions = false;
		[SerializeField] private bool m_UpdateSimManually = false;
		[SerializeField] private bool m_UseApplicationAudioLoopback = false;

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
		public static bool QualityCategory
		{
			get { return Instance.m_QualityCategory; }
			set { Instance.m_QualityCategory = value; }
		}

		public static bool EnableRaycastForCollisions
		{
			get { return Instance.m_EnableRaycastForCollisions; }
			set { Instance.m_EnableRaycastForCollisions = value; }
		}

		public static bool UpdateSimManually
		{
			get { return Instance.m_UpdateSimManually; }
			set { Instance.m_UpdateSimManually = value; }
		}

		public static bool UseApplicationAudioLoopback
		{
			get { return Instance.m_UseApplicationAudioLoopback; }
			set { Instance.m_UseApplicationAudioLoopback = value; }
		}

		public void GetRenderingLayerForBatchDesc(SBatchDesc batchDesc, out int layer)
		{
			Debug.Assert((batchDesc.m_CameraId >= 0 && batchDesc.m_CameraId < MaxCameraCount), "[PKFX] Wrong camera ID feeded to materials factory");
#if UNITY_6000_0_OR_NEWER
			PKFxRenderingPlugin renderingPlugin = FindAnyObjectByType<PKFxRenderingPlugin>();
#else
			PKFxRenderingPlugin renderingPlugin = FindObjectOfType<PKFxRenderingPlugin>();
#endif
			if (renderingPlugin != null)
				layer = renderingPlugin.GetLayerForCameraID(batchDesc.m_CameraId);
			else
				layer = -1;
		}

		public int GetCameraLayer(int index)
		{
			if (ManualCameraLayer)
				return 0;
			if (index < MaxCameraSupport)
				return LayerMask.NameToLayer(PKFxManager.CameraLayers[index]);
#if UNITY_EDITOR
			else if (index == MaxCameraSupport)
				return LayerMask.NameToLayer(PKFxManager.EditorCameraLayer);
#endif
			return 0;
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
		[SerializeField] private PKFxMaterialFactory m_MaterialFactory = null;
		[SerializeField] private bool m_EnableSoftParticles = true;
		[SerializeField] private bool m_DisableDynamicEffectBounds = false;

		[SerializeField] private bool m_EnableThumbnails = true;
		[SerializeField] private bool m_UseThumbnailsInBuilds = false;
		[SerializeField] private bool m_EnableAnimatedThumbnails = true;
		[SerializeField] private bool m_UseAnimatedThumbnailsInBuilds = false;

		[SerializeField] private bool m_AutomaticMeshResizing = true;
		[SerializeField] private float m_VertexBufferSizeMultiplicator = 0.5f;
		[SerializeField] private float m_IndexBufferSizeMultiplicator = 0.5f;
		[SerializeField] private List<SParticleMeshDefaultSize> m_MeshesDefaultSize = new List<SParticleMeshDefaultSize>();
		[SerializeField] private bool m_UseMeshInstancing = true;
		[SerializeField] private bool m_FreeUnusedBatches = false;
		[SerializeField] private uint m_FrameCountBeforeFreeingUnusedBatches = 240;
		[SerializeField] private bool m_ManualCameraLayer = false;
		[SerializeField] private int m_MaxCameraSupport = 1;
		[SerializeField] private bool m_EnableEditorCamera = true;

		[SerializeField] private bool m_EnablePopcornFXLight = false;
		[SerializeField] private int m_MaxPopcornFXLights = 16;

		[SerializeField] private bool m_EnablePopcornFXSound = false;
		[SerializeField] private int  m_MaxPopcornFXSounds	 = 16;

        [SerializeField] private bool m_EnablePopcornFXDecal = false;
        [SerializeField] private int m_MaxPopcornFXDecals = 16;

        private PKFxRaycasts.RaycastPackCallback	m_CustomRaycast = null;
		private PKFxRaycasts.SpherecastPackCallback m_CustomSpherecast = null;

		public ERenderPipeline RenderPipeline { get => CheckRenderPipeline(); }

		public static bool RenderingCategory
		{
			get { return Instance.m_RenderingCategory; }
			set { Instance.m_RenderingCategory = value; }
		}

		public static bool ManualCameraLayer
		{
			get { return Instance.m_ManualCameraLayer; }
			set { Instance.m_ManualCameraLayer = value; }
		}
		public static int MaxCameraSupport
		{
			get { return Instance.m_MaxCameraSupport; }
			set { Instance.m_MaxCameraSupport = value; }
		}

		public static int MaxCameraCount
		{
			get
			{
#if UNITY_EDITOR
				if (Instance.m_EnableEditorCamera)
					return Instance.m_MaxCameraSupport + 1;
#endif
				return Instance.m_MaxCameraSupport;
			}
		}

		public static bool EnableEditorCamera
		{
			get { return Instance.m_EnableEditorCamera; }
			set { Instance.m_EnableEditorCamera = value; }
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

		public static bool EnableThumbnails
		{
			get { return Instance.m_EnableThumbnails; }
			set { Instance.m_EnableThumbnails = value; }
		}

		public static bool UseThumbnailsInBuilds
		{
			get { return Instance.m_UseThumbnailsInBuilds; }
			set
			{
				if (value != Instance.m_UseThumbnailsInBuilds)
				{
					Instance.m_UseThumbnailsInBuilds = value;
#if UNITY_EDITOR
					PKFxAssetCreationUtils.MoveThumbnails(Instance.m_UseThumbnailsInBuilds, false);
#endif
				}
			}
		}

		public static bool EnableAnimatedThumbnails
		{
			get { return Instance.m_EnableAnimatedThumbnails; }
			set { Instance.m_EnableAnimatedThumbnails = value; }
		}

		public static bool UseAnimatedThumbnailsInBuilds
		{
			get { return Instance.m_UseAnimatedThumbnailsInBuilds; }
			set
			{
				if (value != Instance.m_UseAnimatedThumbnailsInBuilds)
				{
					Instance.m_UseAnimatedThumbnailsInBuilds = value;
#if UNITY_EDITOR
					PKFxAssetCreationUtils.MoveThumbnails(Instance.m_UseAnimatedThumbnailsInBuilds, true);
#endif
				}
			}
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
		public static bool EnablePopcornFXLight
		{
			get { return Instance.m_EnablePopcornFXLight; }
			set { Instance.m_EnablePopcornFXLight = value; }
		}
		public static int MaxPopcornFXLights
		{
			get { return Instance.m_MaxPopcornFXLights; }
			set { Instance.m_MaxPopcornFXLights = value; }
		}

		public static bool EnablePopcornFXSound
		{
			get { return Instance.m_EnablePopcornFXSound; }
			set { Instance.m_EnablePopcornFXSound = value; }
		}

		public static int MaxPopcornFXSounds
		{
			get { return Instance.m_MaxPopcornFXSounds; }
			set { Instance.m_MaxPopcornFXSounds = value; }
		}

        public static bool EnablePopcornFXDecal
        {
            get { return Instance.m_EnablePopcornFXDecal; }
            set { Instance.m_EnablePopcornFXDecal = value; }
        }

        public static int MaxPopcornFXDecals
        {
            get { return Instance.m_MaxPopcornFXDecals; }
            set { Instance.m_MaxPopcornFXDecals = value; }
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

		public static PKFxRaycasts.RaycastPackCallback CustomRaycast
		{
			get { return Instance.m_CustomRaycast; }
			set { Instance.m_CustomRaycast = value; }
		}
		public static PKFxRaycasts.SpherecastPackCallback CustomSpherecast
		{
			get { return Instance.m_CustomSpherecast; }
			set { Instance.m_CustomSpherecast = value; }
		}

		// LOD

		[SerializeField] private bool m_LODCategory = true;
		[SerializeField] private bool m_LODAdvanceCategory = false;
		[SerializeField] private bool m_OverrideLODSettings = false;
		[SerializeField] private float m_LODDefaultMinDistance = 5.0f;
		[SerializeField] private float m_LODDefaultMaxDistance = 200.0f;
		[SerializeField] private float m_LODDefaultMinMinDistance = 2.0f;
		[SerializeField] private float m_LODMinDistance = 5.0f;
		[SerializeField] private float m_LODMaxDistance = 200.0f;
		[SerializeField] private float m_LODMinMinDistance = 2.0f;

		public static bool LODCategory
		{
			get { return Instance.m_LODCategory; }
			set { Instance.m_LODCategory = value; }
		}

		public static bool LODAdvanceCategory
		{
			get { return Instance.m_LODAdvanceCategory; }
			set { Instance.m_LODAdvanceCategory = value; }
		}

		public static bool OverrideLODSettings
		{
			get { return Instance.m_OverrideLODSettings; }
			set
			{
				Instance.m_OverrideLODSettings = value;
				if (!Instance.m_OverrideLODSettings)
				{
					Instance.m_LODMinDistance = Instance.m_LODDefaultMinDistance;
					Instance.m_LODMaxDistance = Instance.m_LODDefaultMaxDistance;
					Instance.m_LODMinMinDistance = Instance.m_LODDefaultMinMinDistance;
				}
			}
		}

		public static float LODDefaultMinDistance
		{
			get { return Instance.m_LODDefaultMinDistance; }
			set
			{
				Instance.m_LODDefaultMinDistance = value;
				if (!Instance.m_OverrideLODSettings)
					Instance.m_LODMinDistance = value;
			}
		}

		public static float LODDefaultMaxDistance
		{
			get { return Instance.m_LODDefaultMaxDistance; }
			set
			{
				Instance.m_LODDefaultMaxDistance = value;
				if (!Instance.m_OverrideLODSettings)
					Instance.m_LODMaxDistance = value;
			}
		}
		public static float LODDefaultMinMinDistance
		{
			get { return Instance.m_LODDefaultMinMinDistance; }
			set
			{
				Instance.m_LODDefaultMinMinDistance = value;
				if (!Instance.m_OverrideLODSettings)
					Instance.m_LODMinMinDistance = value;
			}
		}

		public static float LODMinDistance
		{
			get { return Instance.m_LODMinDistance; }
			set { Instance.m_LODMinDistance = value; }
		}

		public static float LODMaxDistance
		{
			get { return Instance.m_LODMaxDistance; }
			set { Instance.m_LODMaxDistance = value; }
		}

		public static float LODMinMinDistance
		{
			get { return Instance.m_LODMinMinDistance; }
			set { Instance.m_LODMinMinDistance = value; }
		}

		// Profiler

		[SerializeField] private bool		m_ProfilerCategory = true;
		[SerializeField] private float		m_ProfilerSceneBudgetInSeconds = 0.002f;
		[SerializeField] private float		m_ProfilerPerEffectBudgetInSeconds = 0.0005f;
		[SerializeField] private uint		m_ProfilerSceneParticleBudget = 10000;
		[SerializeField] private uint		m_ProfilerPerEffectParticleBudget = 1000;
		[SerializeField] private uint		m_CPPMarkersMaxDepth = 2;

		[SerializeField] private Color		m_ProfilerColorBudgetExceeded = Color.red;
		[SerializeField] private Color		m_ProfilerColorBudgetNormal = Color.green;

		[SerializeField] private KeyCode	m_ProfilerHideKey = KeyCode.E;
		[SerializeField] private KeyCode	m_ProfilerPauseKey = KeyCode.R;
		[SerializeField] private KeyCode	m_ProfilerSortKey = KeyCode.T;

		public static bool ProfilerCategory
		{
			get { return Instance.m_ProfilerCategory; }
			set { Instance.m_ProfilerCategory = value; }
		}

		public static float ProfilerSceneBudgetInSeconds
		{
			get { return Instance.m_ProfilerSceneBudgetInSeconds; }
			set { Instance.m_ProfilerSceneBudgetInSeconds = value; }
		}

		public static float ProfilerPerEffectBudgetInSeconds
		{
			get { return Instance.m_ProfilerPerEffectBudgetInSeconds; }
			set { Instance.m_ProfilerPerEffectBudgetInSeconds = value; }
		}

		public static uint ProfilerSceneParticleBudget
		{
			get { return Instance.m_ProfilerSceneParticleBudget; }
			set { Instance.m_ProfilerSceneParticleBudget = value; }
		}

		public static uint CPPMarkersMaxDepth
		{
			get { return Instance.m_CPPMarkersMaxDepth; }
			set { Instance.m_CPPMarkersMaxDepth = value; }
		}

		public static uint ProfilerPerEffectParticleBudget
		{
			get { return Instance.m_ProfilerPerEffectParticleBudget; }
			set { Instance.m_ProfilerPerEffectParticleBudget = value; }
		}

		public static Color ProfilerColorBudgetExceeded
		{
			get { return Instance.m_ProfilerColorBudgetExceeded; }
			set { Instance.m_ProfilerColorBudgetExceeded = value; }
		}

		public static Color ProfilerColorBudgetNormal
		{
			get { return Instance.m_ProfilerColorBudgetNormal; }
			set { Instance.m_ProfilerColorBudgetNormal = value; }
		}

		public static KeyCode ProfilerHideKey
		{
			get { return Instance.m_ProfilerHideKey; }
			set { Instance.m_ProfilerHideKey = value; }
		}

		public static KeyCode ProfilerPauseKey
		{
			get { return Instance.m_ProfilerPauseKey; }
			set { Instance.m_ProfilerPauseKey = value; }
		}

		public static KeyCode ProfilerSortKey
		{
			get { return Instance.m_ProfilerSortKey; }
			set { Instance.m_ProfilerSortKey = value; }
		}

		// Feature Set

		[SerializeField] private bool m_UseGPUBillboarding = false;
		[SerializeField] private bool m_EnableLocalizedPages = true;
		[SerializeField] private bool m_EnableLocalizedByDefault = false;

		[SerializeField] private bool m_FeatureSetCategory = true;

		public static bool FeatureSetCategory
		{
			get { return Instance.m_FeatureSetCategory; }
			set { Instance.m_FeatureSetCategory = value; }
		}

		public static bool UseGPUBillboarding
		{
			get { return Instance.m_UseGPUBillboarding; }
			set { Instance.m_UseGPUBillboarding = value; }
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

		[SerializeField]
		[HideInInspector]
		[Tooltip("Enables the distortion particles material, adding a postFX pass.")]
		private bool m_EnableDistortion = false;
		[HideInInspector]
		public static bool EnableDistortion
		{
			get { return Instance.m_EnableDistortion; }
			set { Instance.m_EnableDistortion = value; }
		}

		[SerializeField]
		[HideInInspector]
		[Tooltip("Enables the distortion blur pass, adding another postFX pass.")]
		private bool m_EnableBlur = false;
		[HideInInspector]
		public static bool EnableBlur
		{
			get { return Instance.m_EnableBlur; }
			set { Instance.m_EnableBlur = value; }
		}

		[SerializeField]
		[HideInInspector]
		[Tooltip("Blur factor. Adjusts the blur's spread.")]
		public float m_BlurFactor = 0.2f;
		[HideInInspector]
		public static float BlurFactor
		{
			get { return Instance.m_BlurFactor; }
			set { Instance.m_BlurFactor = value; }
		}

		// Debug

		[SerializeField] private bool m_DebugCategory = true;
		[SerializeField] private bool m_EnablePopcornFXLogs = true;
		[SerializeField] private bool m_EnableFileLogs = false;
		[SerializeField] private bool m_DebugEffectsBoundingBoxes = false;
		[SerializeField] private bool m_DebugEffectsRaycasts = false;

		public static bool DebugCategory
		{
			get { return Instance.m_DebugCategory; }
			set { Instance.m_DebugCategory = value; }
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

		// Baking

		[SerializeField] private bool m_BakingCategory = true;
		[SerializeField] private bool m_EnableEffectHotreload = true;
		[SerializeField] private bool m_EnableHotreloadInPlayMode = false;
		[SerializeField] private bool m_UseHashesAsMaterialName = true;
		[SerializeField] private bool m_EnableForceDeterminism = false;

		[SerializeField] private string m_PopcornPackFxPath = null;
		[SerializeField] private string m_UnityPackFxPath = "/PopcornFXAssets";

#if UNITY_EDITOR
		[SerializeField] private bool m_EnableAssetPlatformVersion = false;
		[SerializeField] private BuildTarget m_CurrentPlatform = BuildTarget.NoTarget;
#endif

		public static bool BakingCategory
		{
			get { return Instance.m_BakingCategory; }
			set { Instance.m_BakingCategory = value; }
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

		public static bool EnableEffectHotreload
		{
			get { return Instance.m_EnableEffectHotreload; }
			set
			{
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

		public static bool UseHashesAsMaterialName
		{
			get { return Instance.m_UseHashesAsMaterialName; }
			set { Instance.m_UseHashesAsMaterialName = value; }
		}

#if UNITY_EDITOR
		public static bool EnableAssetPlatformVersion
		{
			get { return Instance.m_EnableAssetPlatformVersion; }
			set { Instance.m_EnableAssetPlatformVersion = value; }
		}

		public static string CurrentPlatformName
		{
			get
			{
				if (Instance.m_CurrentPlatform == BuildTarget.NoTarget)
					return "Editor";
				return Instance.m_CurrentPlatform.ToString();
			}
		}

		public static BuildTarget CurrentPlatform
		{
			get { return Instance.m_CurrentPlatform; }
			set { Instance.m_CurrentPlatform = value; }
		}
#endif

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

		public static bool ReimportAssets(List<string> assetsList, string platformName)
		{
			if (Instance.m_PopcornPackFxPath == null || Instance.m_PopcornPackFxPath.Length == 0)
			{
				Debug.LogWarning("[PopcornFX] Source Pack path is required to import your FXs", Instance);
				return false;
			}
			PKFxManager.SetForceDetermismOnBake(PKFxSettings.EnableForceDeterminism);
			PKFxManager.ReimportAssets(assetsList, platformName);
			return true;
		}

		public void AddGUIDForAsset(string assetPath, string newGUID)
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

		public void Setup()
		{
#if UNITY_EDITOR
			if (!string.IsNullOrEmpty(PopcornPackFxPath) && !Directory.Exists(PopcornPackFxPath))
				Debug.Log("[PopcornFX] Valid Source Pack path is required to import your FXs", this);
#endif
		}

		[SerializeField] private string[]	m_QualityVersionNames = new string[0];
		[SerializeField] private string		m_CurrentQualityVersionName;


		public static string[] QualityVersionNames
		{
			get { return Instance.m_QualityVersionNames; }
			set { Instance.m_QualityVersionNames = value; }
		}

		public static string CurrentQualityVersionName
		{
			get { return Instance.m_CurrentQualityVersionName; }
			set { Instance.m_CurrentQualityVersionName = value; }
		}

		public void StoreQualityLevelSettings(string[] levels, int currentLevels)
		{
			m_QualityVersionNames = levels;
			m_CurrentQualityVersionName = levels[currentLevels];
		}


		static public ERenderPipeline CheckRenderPipeline()
		{
			if (GraphicsSettings.currentRenderPipeline)
			{
				if (GraphicsSettings.currentRenderPipeline.GetType().ToString().Contains("HighDefinition"))
				{
					return ERenderPipeline.HDRP;
				}
				else
				{
					return ERenderPipeline.URP;
				}
			}
			else
			{
				return ERenderPipeline.Legacy;
			}
		}

	}
}
