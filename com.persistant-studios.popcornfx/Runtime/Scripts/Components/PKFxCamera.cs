//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.XR;
using System.Collections.Generic;

namespace PopcornFX
{
	public class PKFxCamera : MonoBehaviour
	{
		// Static data
		private static int				g_LastFrameCount = -1;
		public static short				g_CameraUID = 0;
		public static short				GetUniqueID { get { return 0; } }

		// Instance data
		private RenderTexture			m_DistortionRt = null;
		private CommandBuffer			m_CommandBuffer = null;
		private PKFxRenderingPlugin		m_RenderingPlugin;

		[HideInInspector]
		protected short					m_CameraID = 0;
		protected short					m_VRReservedID = 0;
		protected short					m_CurrentCameraID = 0;
		protected Camera				m_Camera;
		protected SCamDesc				m_CameraDescription;
		protected uint					m_CurrentFrameID = 0;
		protected uint					m_LastUpdateFrameID = 0;

		protected int					m_CameraIDRT = -1;

		public static Material			m_DistortionMat = null;
		public static Material			m_BlurMat = null;


		//----------------------------------------------------------------------------

		private bool _updated = false; // Used only when PKFxManager.m_HasSRP is true

		//----------------------------------------------------------------------------

		public int RenderPass
		{
			get { return m_CameraDescription.m_RenderPass; }
			set { m_CameraDescription.m_RenderPass = value; }
		}

		//----------------------------------------------------------------------------

		bool SetupDistortionPassIFN()
		{
			if (PKFxSettings.EnableSoftParticles == false)
				return false;
			if (m_CommandBuffer == null)
			{
				m_CommandBuffer = new CommandBuffer();
				if (m_CommandBuffer == null)
					return false;
				m_CommandBuffer.name = "PopcornFX Distortion and Blur";
				m_DistortionRt = new RenderTexture(m_Camera.pixelWidth, m_Camera.pixelHeight, 0, RenderTextureFormat.DefaultHDR);
				if (m_DistortionRt == null)
					return false;
				m_CameraIDRT = Shader.PropertyToID("TemporaryRT" + m_CameraID);
				m_Camera.AddCommandBuffer(CameraEvent.BeforeImageEffects, m_CommandBuffer);
			}
			if (m_RenderingPlugin.m_EnableDistortion)
			{
				if (m_DistortionMat == null)
				{
					m_DistortionMat = new Material(PKFxSettings.MaterialFactory.DistortionShader);
					if (m_DistortionMat == null)
						return false;
					m_DistortionMat.SetTexture("_DistortionTex", m_DistortionRt);
				}
			}
			if (m_RenderingPlugin.m_EnableBlur)
			{
				if (m_BlurMat == null)
				{
					m_BlurMat = new Material(PKFxSettings.MaterialFactory.BlurShader);
					if (m_BlurMat == null)
						return false;
					m_BlurMat.SetTexture("_DistortionTex", m_DistortionRt);
					m_BlurMat.SetFloat("_BlurFactor", m_RenderingPlugin.m_BlurFactor);
				}
			}
			return true;
		}

		//----------------------------------------------------------------------------

		public void Clean()
		{
			if (m_CommandBuffer != null)
				m_Camera.RemoveCommandBuffer(CameraEvent.BeforeImageEffects, m_CommandBuffer);
			if (-1 != m_CameraIDRT)
				m_CommandBuffer.ReleaseTemporaryRT(m_CameraIDRT);
			m_CommandBuffer = null;
			m_DistortionMat = null;
			m_BlurMat = null;
		}

		//----------------------------------------------------------------------------

		void Awake()
		{
			PKFxRenderingPlugin[] rendering = FindObjectsOfType<PKFxRenderingPlugin>();
			if (rendering.Length != 1)
			{
				if (rendering.Length == 0)
				{
					Debug.LogError("[PopcornFX] PopcornFX need a game object with PKFxRenderingPlugin script attached. (Hierarchy > PopcornFX > Rendering Plugin Manager)");
					return;
				}
				Debug.LogWarning("[PopcornFX] Multiple PKFxRenderingPlugin components detected, will cause issue with PopcornFX effects");
			}
			m_RenderingPlugin = rendering[0];
			m_CameraID = GetUniqueID;
			m_CurrentCameraID = m_CameraID;
#if !UNITY_SWITCH && !UNITY_XBOXONE && !UNITY_GAMECORE_SCARLETT

			if (Application.platform != RuntimePlatform.IPhonePlayer && UnityEngine.XR.XRSettings.enabled)
			{
				List<XRDisplaySubsystem> xrDisplaySubsystems = new List<XRDisplaySubsystem>();
				SubsystemManager.GetInstances<XRDisplaySubsystem>(xrDisplaySubsystems);
				foreach (var xrDisplay in xrDisplaySubsystems)
				{
					if (xrDisplay.running)
					{
						m_VRReservedID = GetUniqueID;
					}
				}
			}


#endif
			m_Camera = GetComponent<Camera>();
			// We disable the rendering of the distortion objects, this is going to be handled in a command buffer:
			m_Camera.cullingMask &= ~(1 << PKFxManagerImpl.m_DistortionLayer);
			//Enable depth texture on mobile
			if (PKFxSettings.EnableSoftParticles)
				m_Camera.depthTextureMode = DepthTextureMode.Depth;
		}

		//----------------------------------------------------------------------------

		void UpdateFrame()
		{
			m_CameraDescription.m_ViewportWidth = (uint)m_Camera.pixelWidth;
			m_CameraDescription.m_ViewportHeight = (uint)m_Camera.pixelHeight;
			m_CameraDescription.m_NearClip = m_Camera.nearClipPlane;
			m_CameraDescription.m_FarClip = m_Camera.farClipPlane;

			if (!m_Camera.stereoEnabled)
			{
				UpdateViewMatrix();
				UpdateProjectionMatrix();
				m_CurrentCameraID = m_CameraID;
				PKFxManager.UpdateCamera(m_CurrentCameraID, ref m_CameraDescription);
			}
			else
			{
				// stereo-cam's first eye.
				UpdateViewMatrix(true, Camera.StereoscopicEye.Left);
				UpdateProjectionMatrix(true, Camera.StereoscopicEye.Left);
				m_CurrentCameraID = m_CameraID;
				PKFxManager.UpdateCamera(m_CurrentCameraID, ref m_CameraDescription);
				// second eye.
				UpdateViewMatrix(true, Camera.StereoscopicEye.Right);
				UpdateProjectionMatrix(true, Camera.StereoscopicEye.Right);
				m_CurrentCameraID = m_VRReservedID;
				PKFxManager.UpdateCamera(m_CurrentCameraID, ref m_CameraDescription);
			}
		}

		//----------------------------------------------------------------------------

		void UpdateViewMatrix(bool isVR = false, Camera.StereoscopicEye eye = Camera.StereoscopicEye.Left)
		{
			if (!isVR)
				m_CameraDescription.m_ViewMatrix = m_Camera.worldToCameraMatrix;
			else
				m_CameraDescription.m_ViewMatrix = m_Camera.GetStereoViewMatrix(eye);
		}

		//----------------------------------------------------------------------------

		void UpdateProjectionMatrix(bool isVR = false, Camera.StereoscopicEye eye = Camera.StereoscopicEye.Left)
		{
			Matrix4x4 projectionMatrix;
			if (!isVR)
				projectionMatrix = m_Camera.projectionMatrix;
			else
				projectionMatrix = m_Camera.GetStereoProjectionMatrix(eye);

			m_CameraDescription.m_ProjectionMatrix = GL.GetGPUProjectionMatrix(projectionMatrix, false);
		}

		//----------------------------------------------------------------------------

		public void UpdateCamera(bool enableDistortion, bool enableBlur)
		{
			m_CurrentFrameID++;
			UpdateFrame();

			if (enableDistortion || enableBlur)
			{
				if (SetupDistortionPassIFN())
				{
					m_CommandBuffer.Clear();
					// We start by binding the distortion render target and we render all the distortion particles in it:
					m_CommandBuffer.SetRenderTarget(m_DistortionRt);
					m_CommandBuffer.ClearRenderTarget(false, true, Color.black);
					foreach (SMeshDesc desc in PKFxManagerImpl.m_Renderers)
					{
						if (desc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
						{
							if (desc.m_RenderingObject.activeInHierarchy)
							{
								if (desc.m_Slice != null)
									m_CommandBuffer.DrawMesh(desc.m_Slice.mesh, Matrix4x4.identity, desc.m_Material);
								else if (desc.m_Procedural != null)
									m_CommandBuffer.DrawProceduralIndirect(Matrix4x4.identity, desc.m_Material, 0, MeshTopology.Triangles, desc.m_Procedural.DrawIndirectArgs);
							}
						}
					}
					// Now we get a tmp render target to blur the camera RT:
					m_CommandBuffer.GetTemporaryRT(m_CameraIDRT, m_Camera.pixelWidth, m_Camera.pixelHeight);

					if (enableBlur && enableDistortion == false)
						m_CommandBuffer.Blit(BuiltinRenderTextureType.CameraTarget, m_CameraIDRT, m_BlurMat);
					else
						m_CommandBuffer.Blit(BuiltinRenderTextureType.CameraTarget, m_CameraIDRT, m_DistortionMat);
					if (enableDistortion && enableBlur)
						m_CommandBuffer.Blit(m_CameraIDRT, BuiltinRenderTextureType.CameraTarget, m_BlurMat);
					else
						m_CommandBuffer.Blit(m_CameraIDRT, BuiltinRenderTextureType.CameraTarget);
					
				}
			}
			else if (m_CommandBuffer != null)
			{
				m_Camera.RemoveCommandBuffer(CameraEvent.BeforeImageEffects, m_CommandBuffer);
				m_CommandBuffer = null;
				if (-1 != m_CameraIDRT)
					m_CommandBuffer.ReleaseTemporaryRT(m_CameraIDRT);
			}
			_updated = true;
		}

		//----------------------------------------------------------------------------

		public void LateUpdateCamera()
		{
			if (g_LastFrameCount != Time.frameCount)
			{
				g_LastFrameCount = Time.frameCount;
				if (_updated)
				{
					_updated = false;
					_Render();
				}
			}
		}

		//----------------------------------------------------------------------------

		void _Render()
		{
			if (m_CurrentFrameID != m_LastUpdateFrameID)
			{
				m_CurrentCameraID = m_CameraID;
				m_LastUpdateFrameID = m_CurrentFrameID;
			}
			else
				m_CurrentCameraID = m_VRReservedID;
			PKFxManager.Render(m_CurrentCameraID);
		}

	}
}
