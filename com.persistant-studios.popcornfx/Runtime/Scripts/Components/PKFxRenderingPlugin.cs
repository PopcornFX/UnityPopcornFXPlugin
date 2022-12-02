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
		private static int g_LastFrameCount = -1;

		// Exposed in "Advanced" Editor
		[Tooltip("Loads a user-defined mesh to be used for particles world collisions.")]
		[HideInInspector]
		public bool m_UseSceneMesh = false;
		[Tooltip("Link to the pre-built mesh asset.")]
		[HideInInspector]
		public PKFxMeshAsset m_SceneMesh;

		[HideInInspector]
		public float m_TimeMultiplier = 1.0f;

		[Tooltip("Enables the distortion particles material, adding a postFX pass.")]
		[HideInInspector]
		public bool m_EnableDistortion = false;
		[Tooltip("Enables the distortion blur pass, adding another postFX pass.")]
		[HideInInspector]
		public bool m_EnableBlur = false;
		[Tooltip("Blur factor. Adjusts the blur's spread.")]
		[HideInInspector]
		public float m_BlurFactor = 0.2f;

		[HideInInspector]
		public List<PKFxCamera> m_Cameras = null;

		[HideInInspector]
		public List<PKFxEffectAsset> m_PreloadEffect;

		public float TimeMultiplier
		{
			get { return m_TimeMultiplier; }
			set {
				m_TimeMultiplier = value;
				PKFxManager.TimeMultiplier = m_TimeMultiplier;
			}
		}

		[HideInInspector]
		[Tooltip("Max camera that can render particle. WARNING: Heavy performance hit as billboarding will be duplicated for each camera !")]
		[Range(1, 4)]
		public int m_MaxCameraSupport = 1;

		[HideInInspector]
		public int MaxCameraSupport
		{
			get { return m_MaxCameraSupport; }
			set { 
				m_MaxCameraSupport = value;
				PKFxManager.SetMaxCameraCount(m_MaxCameraSupport);
			}
		}


		//----------------------------------------------------------------------------

		void Start()
		{
#if UNITY_IOS
		if (m_EnableDistortion)
		{
			Debug.LogWarning("[PopcornFX] Image effects not supported, distortions disabled.", this);
			m_EnableDistortion = false;
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
						PKFxManager.PreloadEffectIFN(effect.AssetVirtualPath, effect.m_UsesMeshRenderer);
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
			if (!PKFxManager.IsDllLoaded())
			{
				gameObject.SetActive(false);
				Debug.LogWarning("[PopcornFX] Unable to load the PopcornFX native plugin, check that the library is present and its import settings.");
				return;
			}
			PKFxManager.StartupPopcorn(false);
			PKFxManager.SetMaxCameraCount(m_MaxCameraSupport);

			PKFxSoundManager.ClearSounds();

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
				if (i < m_MaxCameraSupport)
				{
					m_Cameras[i].UpdateCamera(m_EnableDistortion, m_EnableBlur);
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
					if (i < m_MaxCameraSupport)
					{
						m_Cameras[i].LateUpdateCamera();
					}
				}
			}

			PKFxManager.DrawMeshRenderers();

			PKFxSoundManager.DeleteSoundsIFN();
		}

		//----------------------------------------------------------------------------

		internal int RegisterCamera(PKFxCamera PKFxCamera)
		{
			if (!m_Cameras.Contains(PKFxCamera))
			{
				if (m_Cameras.Count >= m_MaxCameraSupport)
					Debug.LogWarning("[PKFX] All available Cameras slot are registered. Current camera is queue until slot is freed.");
				m_Cameras.Add(PKFxCamera);
				int id = m_Cameras.IndexOf(PKFxCamera);
				PKFxCamera.SetCullingMask((short)id);

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
					m_Cameras[i].SetCullingMask((short)i);
				}
				return true;
			}
			return false;
		}

#if !UNITY_EDITOR
		private void OnApplicationQuit()
		{
			PKFxManager.ShutdownPopcorn();
		}
#endif
	}
}