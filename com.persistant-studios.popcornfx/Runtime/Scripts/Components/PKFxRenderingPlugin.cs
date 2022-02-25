//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
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
		public Camera m_Camera = null;

		private PKFxCamera m_current = null;

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

		public Camera CameraForDrawProcedural
		{
			get 
			{ 
				if (PKFxManager.IsUnitTesting)
					return null;
				return m_Camera;
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
			PKFxManager.StartupPopcorn(true);
			PKFxSoundManager.ClearSounds();

			if (Camera.main == null)
				Debug.LogWarning("[PopcornFX] No main camera in the scene! No PopcornFX particles will be rendered. Make sure at least one of the camera with the PKFxCamera component on it is tagged as the MainCamera.");
			else if (Camera.main.GetComponent<PKFxCamera>() == null)
				Debug.LogWarning("[PopcornFX] No PKFxCamera in the scene! No PopcornFX particles will be rendered. Make sure at least one of the camera with the PKFxCamera component on it is tagged as the MainCamera.");
		}

		//----------------------------------------------------------------------------

		void Update()
		{
			float frameDt = Time.smoothDeltaTime * TimeMultiplier;

			m_Camera = Camera.main;

			if (m_Camera != null)
			{
				PKFxCamera current = m_Camera.GetComponent<PKFxCamera>();

				if (current)
				{
					if (current != m_current)
					{
						if (m_current)
							m_current.Clean();
						m_current = current;
					}
					current.UpdateCamera(m_EnableDistortion, m_EnableBlur);

				}
			}
		}

		//----------------------------------------------------------------------------

		void LateUpdate()
		{
			if (g_LastFrameCount != Time.frameCount)
			{
				PKFxManager.UpdateParticles();
				if (m_Camera != null)
				{
					PKFxCamera current = m_Camera.GetComponent<PKFxCamera>();

					if (current)
						current.LateUpdateCamera();
				}
			}
			PKFxSoundManager.DeleteSoundsIFN();

		}

#if !UNITY_EDITOR
	private void OnApplicationQuit()
	{
		PKFxManager.ShutdownPopcorn();
	}
#endif
	}
}