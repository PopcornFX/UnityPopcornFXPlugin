//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections.Generic;
using UnityEngine;

//----------------------------------------------------------------------------a

namespace PopcornFX
{
	[RequireComponent(typeof(PKFxLogger))]
	public class PKFxRenderingPlugin : MonoBehaviour
	{
		// Static data
		private static int				g_LastFrameCount = -1;

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
		//Collision Baking
		[Tooltip("Output path for the scene mesh, relative to the PackFX directory")]
		[HideInInspector]
		public string					m_OutputPkmmPath = "Meshes/UnityScene.pkmm";
		[Tooltip("List of the GameObjects to be searched for potential meshes.")]
		[HideInInspector]
		public GameObject[]				m_GameObjectsToSearch;
		[HideInInspector]
		public GameObject[]				m_MeshGameObjects;

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

				if (value.Length > 4)
				{
					Debug.LogError("[PopcornFX] A maximum of 4 camera layers is expected.");
					return;
				}
				m_CameraLayers = value;
				PKFxManager.SetMaxCameraCount(MaxCameraSupport());

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
			PKFxRenderingPlugin[] rendering = FindObjectsOfType<PKFxRenderingPlugin>();
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
			PKFxManager.SetMaxCameraCount(MaxCameraSupport());
			PKFxManager.StartupPopcorn(false);

			PKFxSoundManager.ClearSounds();

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

			if (Camera.main == null)
				Debug.LogWarning("[PopcornFX] No main camera in the scene! No PopcornFX particles will be rendered. Make sure at least one of the camera with the PKFxCamera component on it is tagged as the MainCamera.");
			else if (Camera.main.GetComponent<PKFxCamera>() == null)
				Debug.LogWarning("[PopcornFX] No PKFxCamera in the scene! No PopcornFX particles will be rendered. Make sure at least one of the camera with the PKFxCamera component on it is tagged as the MainCamera.");
		}

		public void UpdateSimulation()
		{
			_LateUpdate();
		}

		//----------------------------------------------------------------------------

		void Update()
		{
			float frameDt = Time.smoothDeltaTime * TimeMultiplier;

			for (int i = 0; i < m_Cameras.Count; ++i)
			{
				if (i < MaxCameraSupport())
				{
					m_Cameras[i].UpdateCamera();
				}
			}
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
			if (g_LastFrameCount != Time.frameCount)
			{
				PKFxManager.UpdateParticles();

				for (int i = 0; i < m_Cameras.Count; ++i)
				{
					if (i < MaxCameraSupport())
					{
						m_Cameras[i].LateUpdateCamera();
					}
				}
			}
			if (PKFxSettings.EnablePopcornFXLight)
			{
				m_LightPool.DrawLightRenderers();
			}

			PKFxManager.DrawMeshRenderers();

			PKFxSoundManager.DeleteSoundsIFN();
		}

		//----------------------------------------------------------------------------

		internal int RegisterCamera(PKFxCamera PKFxCamera)
		{
			if (!m_Cameras.Contains(PKFxCamera))
			{
				UpdateLayerMask();
				if (m_CameraLayers == null)
				{
					Debug.LogError("[PopcornFX] The PKFxRenderingPlugin's camera layers must be assigned before registering any cameras.");
					return -1;
				}
				m_Cameras.Add(PKFxCamera);
				int id = m_Cameras.IndexOf(PKFxCamera);
				if (m_Cameras.Count > MaxCameraSupport())
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
			if (m_Cameras.Contains(PKFxCamera))
			{
				m_Cameras.Remove(PKFxCamera);

				for (int i = 0; i < m_Cameras.Count; ++i)
				{
					m_Cameras[i].m_CameraID = (short)i;
					m_Cameras[i].SetCullingMask((short)i, m_CameraLayers[i], m_AllPopcornFXLayerMask);
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

		public int MaxCameraSupport()
		{
			if (m_CameraLayers == null || m_CameraLayers.Length == 0)
			{
				m_CameraLayers = new int[1];
				m_CameraLayers[0] = PKFxSettings.Instance.GetCameraLayer(0);
			}
			return m_CameraLayers.Length;
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

#if !UNITY_EDITOR
		private void OnApplicationQuit()
		{
			PKFxManager.ShutdownPopcorn();
		}
#endif

		//----------------------------------------------------------------------------

		public void SetLightsBuffer(IntPtr lightsInfos, int count)
		{
			if (PKFxSettings.EnablePopcornFXLight)
			{
				m_LightPool.SetLightBuffer(lightsInfos, count);
			}
		}
	}
}
