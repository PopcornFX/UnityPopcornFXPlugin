//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//----------------------------------------------------------------------------a

namespace PopcornFX
{
	[RequireComponent(typeof(PKFxLogger))]
	public class PKFxRenderingPlugin : MonoBehaviour
	{
		// Exposed in "Advanced" Editor
		[Tooltip("Loads a user-defined mesh to be used for particles world collisions.")]
		[HideInInspector]
		public bool						m_UseSceneMesh = false;
		[Tooltip("Link to the pre-built mesh asset.")]
		[HideInInspector]
		public PKFxMeshAsset			m_SceneMesh;
		[HideInInspector]
		public float					m_TimeMultiplier = 1.0f;
		[HideInInspector]
		public List<PKFxCamera>			m_Cameras = null;
		[HideInInspector]
		public List<PKFxEffectAsset>	m_PreloadEffect;
		[HideInInspector]
		private static PKFxLightPool	m_LightPool;
		[HideInInspector]
		private static PKFxAudioPool	m_SoundPool;
		[HideInInspector]
		private static PKFxDecalPool	m_DecalPool;
		//Collision Baking
		[Tooltip("Output path for the scene mesh, relative to the PackFX directory")]
		[HideInInspector]
		public string					m_OutputPkmmPath = "Meshes/UnityScene.pkmm";
		[Tooltip("List of the GameObjects to be searched for potential meshes.")]
		[HideInInspector]
		public GameObject[]				m_GameObjectsToSearch;
		[HideInInspector]
		public GameObject[]				m_MeshGameObjects;

		private PKFxCamera				m_EditorCamera = null;

		public float TimeMultiplier
		{
			get { return m_TimeMultiplier; }
			set
			{
				m_TimeMultiplier = value;
				PKFxManager.TimeMultiplier = m_TimeMultiplier;
			}
		}

		private LayerMask m_AllPopcornFXLayerMask;

		[HideInInspector]
		[Tooltip("The layers for each camera. The size of this array determines the max cameras that can render particles. WARNING: Heavy performance hit as billboarding will be duplicated for each camera !")]
		public int[] m_CameraLayers = null;

		public int[] CameraLayers
		{
			get { return m_CameraLayers; }
			set
			{
				if (value == null || value.Length == 0)
				{
					Debug.LogError("[PopcornFX] A minimum of 1 camera layer is expected.");
					return;
				}

				if (value.Length > PKFxSettings.MaxCameraCount)
				{
					Debug.LogError("[PopcornFX] A maximum of " + PKFxSettings.MaxCameraCount + " camera layers is expected.");
					return;
				}
				m_CameraLayers = value;

				UpdateLayerMask();
			}
		}

		//----------------------------------------------------------------------------

		void Start()
		{
#if UNITY_IOS
			if (PKFxSettings.EnableDistortion)
			{
				Debug.LogWarning("[PopcornFX] Image effects not supported, distortions disabled.", this);
				PKFxSettings.EnableDistortion = false;
			}
#endif
			PKFxManager.TimeMultiplier = TimeMultiplier;
			if (m_UseSceneMesh && m_SceneMesh != null && !PKFxSettings.EnableRaycastForCollisions)
			{
				PKFxManager.SetSceneMesh(m_SceneMesh);
			}

			if (m_PreloadEffect != null)
			{
				foreach (PKFxEffectAsset effect in m_PreloadEffect)
				{
					if (effect != null)
					{
						PKFxManager.SetBuiltAsset(effect);
						PKFxManager.PreloadEffectDependencies(effect);
						PKFxManager.PreloadEffectIFN(effect.AssetVirtualPath, effect.m_RequiresGameThreadCollect);
						PKFxManager.SetBuiltAsset(null);
					}
				}
			}
		}

		//----------------------------------------------------------------------------

		void Awake()
		{
#if UNITY_6000_0_OR_NEWER
			PKFxRenderingPlugin[] rendering = FindObjectsByType<PKFxRenderingPlugin>(FindObjectsSortMode.None);
#else
			PKFxRenderingPlugin[] rendering = FindObjectsOfType<PKFxRenderingPlugin>();
#endif
			if (rendering.Length != 1)
			{
				gameObject.SetActive(false);
				return;
			}
			PKFxManager.RenderingPlugin = this;

			if (!PKFxManager.IsDllLoaded())
			{
				gameObject.SetActive(false);
				Debug.LogWarning("[PopcornFX] Unable to load the PopcornFX native plugin, check that the library is present and its import settings.");
				return;
			}
			PKFxManager.SetQualityLevelSettings();
			PKFxManager.UpdateQualityLevels();
			PKFxManager.StartupPopcorn(false);

			UpdateCameraLayerIFN();
			PKFxManager.SetMaxCameraCount(PKFxSettings.MaxCameraCount);
#if UNITY_EDITOR
			if (PKFxSettings.EnableEditorCamera)
				gameObject.AddComponent<PKFxCamera>();
#endif

			if (PKFxSettings.EnablePopcornFXLight)
			{
				if (PKFxManager.LightPool == null)
				{
					GameObject lightPool = new GameObject("PopcornFX Lights Pool");
					DontDestroyOnLoad(lightPool);
					PKFxManager.LightPool = lightPool.AddComponent<PKFxLightPool>();
				}

				m_LightPool = PKFxManager.LightPool;
				m_LightPool.m_MaxLightNumber = PKFxSettings.MaxPopcornFXLights;
			}
			if (PKFxSettings.EnablePopcornFXSound)
			{
				if (PKFxManager.SoundPool == null)
				{
					GameObject soundPool = new GameObject("PopcornFX Sounds Pool");
					DontDestroyOnLoad(soundPool);
					PKFxManager.SoundPool = soundPool.AddComponent<PKFxAudioPool>();

				}

				m_SoundPool = PKFxManager.SoundPool;
				m_SoundPool.m_MaxSoundNumber = PKFxSettings.MaxPopcornFXSounds;
			}
			if (PKFxSettings.EnablePopcornFXDecal)
			{
				if (PKFxManager.DecalPool == null)
				{
					GameObject decalPool = new GameObject("PopcornFX Decals Pool");
					DontDestroyOnLoad(decalPool);
					PKFxManager.DecalPool = decalPool.AddComponent<PKFxDecalPool>();

				}

				m_DecalPool = PKFxManager.DecalPool;
				m_DecalPool.m_MaxDecalNumber = PKFxSettings.MaxPopcornFXSounds;
			}
		}

		//----------------------------------------------------------------------------

		public void UpdateSimulation()
		{
			_LateUpdate();
		}

		//----------------------------------------------------------------------------

		void Update()
		{
			if (PKFxManager.CanSkipUpdate())
				return;
			for (int i = 0; i < m_Cameras.Count; ++i)
			{
				if (i < PKFxSettings.MaxCameraSupport)
				{
					m_Cameras[i].UpdateCamera();
				}
			}
#if UNITY_EDITOR
			if (m_EditorCamera != null)
				m_EditorCamera.UpdateCamera();
#endif
		}

		//----------------------------------------------------------------------------

		private void LateUpdate()
		{
			if (PKFxSettings.UpdateSimManually)
				return;
			_LateUpdate();
		}

		//----------------------------------------------------------------------------

		void _LateUpdate()
		{
			if (PKFxManager.CanSkipUpdate())
				return;
			PKFxManager.UpdateParticles();
			for (int i = 0; i < m_Cameras.Count; ++i)
			{
				if (i < PKFxSettings.MaxCameraSupport)
				{
					m_Cameras[i].LateUpdateCamera();
				}
			}
#if UNITY_EDITOR
			if (m_EditorCamera != null)
				m_EditorCamera.LateUpdateCamera();
#endif
			if (PKFxSettings.EnablePopcornFXLight)
			{
				m_LightPool.DrawLightRenderers();
			}
			if (PKFxSettings.EnablePopcornFXSound)
			{
				m_SoundPool.DrawSoundRenderers();
			}

			if (PKFxSettings.EnablePopcornFXDecal)
			{
				m_DecalPool.DrawDecalRenderers();
			}
			PKFxManager.DrawMeshRenderers();

#if UNITY_EDITOR
			if (PKFxSettings.DebugEffectsBoundingBoxes && !PKFxSettings.UseGPUBillboarding)
				PKFxManager.DrawBounds();
#endif
		}

		//----------------------------------------------------------------------------

		internal int RegisterCamera(PKFxCamera PKFxCamera)
		{
#if UNITY_EDITOR
			if (PKFxCamera.IsEditorCamera)
			{
				m_EditorCamera = PKFxCamera;
				short id = (short)PKFxSettings.MaxCameraSupport;
				PKFxCamera.SetCullingMask(id, 1 << m_CameraLayers[id], m_AllPopcornFXLayerMask);
				return id;
			}
#endif
			if (!m_Cameras.Contains(PKFxCamera))
			{
				UpdateCameraLayerIFN();
				if (m_CameraLayers == null)
				{
					Debug.LogError("[PopcornFX] The PKFxRenderingPlugin's camera layers must be assigned before registering any cameras.");
					return -1;
				}
				m_Cameras.Add(PKFxCamera);
				int id = m_Cameras.IndexOf(PKFxCamera);
				if (m_Cameras.Count > PKFxSettings.MaxCameraSupport)
					Debug.LogWarning("[PKFX] All available Cameras slot are registered. Current camera is queue until slot is freed.");
				else
					PKFxCamera.SetCullingMask((short)id, 1 << m_CameraLayers[id], m_AllPopcornFXLayerMask);
				return id;
			}
			return -1;
		}

		//----------------------------------------------------------------------------

		internal bool UnRegisterCamera(PKFxCamera PKFxCamera)
		{
#if UNITY_EDITOR
			if (PKFxCamera == m_EditorCamera)
			{
				m_EditorCamera = null;
				PKFxCamera.ResetEditorCameraMask();
				return true;
			}
#endif
			if (m_Cameras.Contains(PKFxCamera))
			{
				m_Cameras.Remove(PKFxCamera);

				for (int i = 0; i < m_Cameras.Count; ++i)
				{
					m_Cameras[i].m_CameraID = (short)i;
					m_Cameras[i].SetCullingMask((short)i, 1 << m_CameraLayers[i], m_AllPopcornFXLayerMask);
				}
				return true;
			}
			return false;
		}

		//----------------------------------------------------------------------------

		public int GetLayerForCameraID(int cameraID)
		{
			return m_CameraLayers[cameraID];
		}

		//----------------------------------------------------------------------------

		public void UpdateLayerMask()
		{
			m_AllPopcornFXLayerMask = 0;
			for (int i = 0; i < m_CameraLayers.Length; ++i)
			{
				m_AllPopcornFXLayerMask |= 1 << m_CameraLayers[i];
			}
		}

		//----------------------------------------------------------------------------

		public bool UpdateCameraLayerIFN()
		{
			if (m_CameraLayers == null || m_CameraLayers.Length != PKFxSettings.MaxCameraCount)
			{
				int[] prevCameraLayers = CameraLayers;
				int[] newCameraLayers = new int[PKFxSettings.MaxCameraCount];
				int editorCameraLayer = LayerMask.NameToLayer(PKFxManager.EditorCameraLayer);
				for (int i = 0; i < PKFxSettings.MaxCameraCount; i++)
				{
					if (prevCameraLayers != null && i < prevCameraLayers.Length)
					{
						if (string.IsNullOrEmpty(LayerMask.LayerToName(prevCameraLayers[i])) || prevCameraLayers[i] == editorCameraLayer)
							newCameraLayers[i] = PKFxSettings.Instance.GetCameraLayer(i);
						else
							newCameraLayers[i] = prevCameraLayers[i];
					}
					else
						newCameraLayers[i] = PKFxSettings.Instance.GetCameraLayer(i);
				}
				CameraLayers = newCameraLayers;
				return true;
			}
			return false;
		}

#if !UNITY_EDITOR
		private void OnApplicationQuit()
		{
			PKFxRaycasts.Clear();
			PKFxManager.ShutdownPopcorn();
		}
#endif

		//----------------------------------------------------------------------------

		public void AddLightBuffer(IntPtr lightsInfos, int count)
		{
			if (PKFxSettings.EnablePopcornFXLight)
			{
				m_LightPool.AddLightBuffer(lightsInfos, count);
			}
		}

		public void SetSoundsBuffer(IntPtr soundInfos, int count)
		{
			if (PKFxSettings.EnablePopcornFXSound)
			{
				m_SoundPool.SetSoundBuffer(soundInfos, count);
			}
		}

		public void AddDecalBuffer(IntPtr decalInfos, int count)
		{
			if (PKFxSettings.EnablePopcornFXDecal)
			{
				m_DecalPool.AddDecalBuffer(decalInfos, count);
			}
		}

#if UNITY_EDITOR
		public void ResetComputeBuffers()
		{
			StartCoroutine(ResetComputeBuffersRoutine());
		}

		private IEnumerator ResetComputeBuffersRoutine()
		{
			yield return new WaitForEndOfFrame();
			foreach (SMeshDesc desc in PKFxManagerImpl.m_Renderers)
				desc.ResetComputeBuffer();
		}
#endif

	}
}