//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
	public class PKFxProceduralRenderer : MonoBehaviour
	{
		public Material m_InstancedMaterial = null;
		public Bounds m_Bounds = new Bounds();

		private uint m_VertexSize = 0; // in 32bits register
		private int m_VertexMultiplier = 0;
		private uint m_ReservedIndexCount = 0;
		private uint m_ReservedVertexCount = 0;

		private int m_NewParticleCount = 0;
		private int m_ParticleCount = 0;

		private ComputeBuffer m_RawBuffer = null;
		private ComputeBuffer m_RawIndexBuffer = null;
		private ComputeBuffer m_InputOffsets = null;
		private ComputeBuffer m_BillboardInfo = null;
		private ComputeBuffer m_AtlasInfo = null;
		private ComputeBuffer m_DrawIndirectArgs = null;

		private bool m_UseIndirectDraw = false;

		private PKFxRenderingPlugin m_RenderingPlugin = null;
		private int m_GUID = 0;

		private bool			m_Distortion = false;


		enum EBillboardingInputs
		{
			GenInput_ParticlePosition = 0,
			GenInput_ParticleSize,
			GenInput_ParticleRotation,
			GenInput_ParticleAxis0,
			GenInput_ParticleAxis1,
			GenInput_Color,
			GenInput_TextureID,
			GenInput_AlphaCursor,
			GenInput_Count,
		}

		private uint[] m_Offsets = new uint[(int)EBillboardingInputs.GenInput_Count];

		public int GUID { get { return m_GUID; } set { m_GUID = value; } }
		public int ParticleCount { get { return m_NewParticleCount; } set { m_NewParticleCount = value; OnSetParticleCount(); } }
		public int VertexMultiplier { get { return m_VertexMultiplier; } }
		public uint ReservedVertexCount { get { return m_ReservedVertexCount; } }
		public uint ReservedIndexCount { get { return m_ReservedIndexCount; } }

		public int NativeRawBillboardingInfoBufferSize { get { return 256 * 8; } }
		public int NativeRawAtlasesBufferSize { get { return 257 * 16; } }

		public IntPtr GetNativeRawVertexBuffer() { return m_RawBuffer == null ? IntPtr.Zero : m_RawBuffer.GetNativeBufferPtr(); }
		public IntPtr GetNativeRawIndexBuffer() { return m_RawIndexBuffer == null ? IntPtr.Zero : m_RawIndexBuffer.GetNativeBufferPtr(); }
		public IntPtr GetNativeRawBillboardingInfoBuffer() { return m_BillboardInfo == null ? IntPtr.Zero : m_BillboardInfo.GetNativeBufferPtr(); }
		public IntPtr GetNativeRawAtlasesBuffer() { return m_AtlasInfo == null ? IntPtr.Zero : m_AtlasInfo.GetNativeBufferPtr(); }
		public IntPtr GetNativeRawArgumentBuffer() { return m_DrawIndirectArgs == null ? IntPtr.Zero : m_DrawIndirectArgs.GetNativeBufferPtr(); }

		public ComputeBuffer DrawIndirectArgs { get { return m_DrawIndirectArgs; } }

		~PKFxProceduralRenderer()
		{
			OnDestroy();
		}

		public bool ResizeParticleComputeBuffer(uint particleCount,
												uint usedVertexCount,
												uint usedIndexCount,
												uint reservedVertexCount,
												uint reservedIndexCount)
		{
#if UNITY_2017 || UNITY_2018
		Debug.LogError("[PopcornFX] Rendering with geometry buffers is not supported in 2017.X, Disable the feature or upgrade rour project");
		return false;
#else//if UNITY_2019 and above
			if (usedVertexCount == 0 || m_InstancedMaterial == null)
				return false;

			int size = Convert.ToInt32(m_VertexSize * reservedVertexCount); // register count
			if (m_RawBuffer == null || m_RawBuffer.count < size)
			{
				if (m_RawBuffer != null)
				{
					m_RawBuffer.Release();
					m_RawBuffer = null;
				}
				m_RawBuffer = new ComputeBuffer(size, 4, ComputeBufferType.Raw); // 32 bits register
				m_ReservedVertexCount = reservedVertexCount;
				m_InstancedMaterial.SetBuffer("_In_RawData", m_RawBuffer);

				// Update offsets
				uint[] uploadOffsets = new uint[(int)EBillboardingInputs.GenInput_Count];
				for (int i = 0; i < 8; ++i)
					uploadOffsets[i] = m_Offsets[i] * reservedVertexCount;
				m_InputOffsets.SetData(uploadOffsets);
			}
			if (usedIndexCount > 0 && (m_RawIndexBuffer == null || m_RawIndexBuffer.count < usedIndexCount))
			{
				if (m_RawIndexBuffer != null)
				{
					m_RawIndexBuffer.Dispose();
					m_RawIndexBuffer = null;
				}
				m_RawIndexBuffer = new ComputeBuffer(Convert.ToInt32(reservedIndexCount), 4, ComputeBufferType.Raw); // Could be structured or raw
				m_ReservedIndexCount = reservedIndexCount;

				m_InstancedMaterial.SetBuffer("_In_Indexes", m_RawIndexBuffer);
			}
			m_ParticleCount = Convert.ToInt32(particleCount);
			return m_RawBuffer != null;
#endif
		}

		void OnDestroy()
		{
			if (m_RawBuffer != null)
			{
				m_RawBuffer.Release();
				m_RawBuffer = null;
			}
			if (m_RawIndexBuffer != null)
			{
				m_RawIndexBuffer.Release();
				m_RawIndexBuffer = null;
			}
			if (m_InputOffsets != null)
			{
				m_InputOffsets.Release();
				m_InputOffsets = null;
			}
			if (m_BillboardInfo != null)
			{
				m_BillboardInfo.Release();
				m_BillboardInfo = null;
			}
			if (m_AtlasInfo != null)
			{
				m_AtlasInfo.Release();
				m_AtlasInfo = null;
			}
			if (m_DrawIndirectArgs != null)
			{
				m_DrawIndirectArgs.Release();
				m_DrawIndirectArgs = null;
			}
		}

		public void Init(SBatchDesc desc, Material mat)
		{
			// If the execution is not single threaded and the raycasts are not enabled,
			// it means that the game thread do not need to wait for the end of the particles update
			if (!PKFxSettings.SingleThreadedExecution && !PKFxSettings.EnableRaycastForCollisions)
				m_UseIndirectDraw = true;
			if (desc == null || mat == null)
				return;

#if UNITY_2017 || UNITY_2018
		Debug.LogError("[PopcornFX] Rendering with geometry buffers is not supported in 2017.X, Disable the feature or upgrade rour project");
		return;
#else //if UNITY_2019 and above
			Debug.Assert(m_InputOffsets == null);
			Debug.Assert(m_AtlasInfo == null);
			Debug.Assert(m_BillboardInfo == null);
			if (m_InputOffsets != null)
			{
				m_InputOffsets.Release();
				m_InputOffsets = null;
			}
			if (m_BillboardInfo != null)
			{
				m_BillboardInfo.Release();
				m_BillboardInfo = null;
			}
			if (m_AtlasInfo != null)
			{
				m_AtlasInfo.Release();
				m_AtlasInfo = null;
			}
			if (m_DrawIndirectArgs != null)
			{
				m_DrawIndirectArgs.Release();
				m_DrawIndirectArgs = null;
			}
			m_InputOffsets = new ComputeBuffer(8, 4, ComputeBufferType.Raw);
			m_BillboardInfo = new ComputeBuffer(256, 8, ComputeBufferType.Raw);
			m_AtlasInfo = new ComputeBuffer(257, 16, ComputeBufferType.Raw);

			if (m_UseIndirectDraw)
				m_DrawIndirectArgs = new ComputeBuffer(5, sizeof(uint), ComputeBufferType.IndirectArguments);

			m_InstancedMaterial = mat;
			m_VertexMultiplier = 6;
			m_VertexSize = 0;

			// float4 Position
			m_Offsets[(int)EBillboardingInputs.GenInput_ParticlePosition] = m_VertexSize;
			m_VertexSize += 4;
			if (desc.HasShaderVariationFlag(EShaderVariationFlags.Has_Size2))
			{
				// float2 Size
				m_Offsets[(int)EBillboardingInputs.GenInput_ParticleSize] = m_VertexSize;
				m_VertexSize += 2;
				mat.EnableKeyword("BB_SizeFloat2");
			}
			else
			{
				// float Size
				m_Offsets[(int)EBillboardingInputs.GenInput_ParticleSize] = m_VertexSize;
				m_VertexSize += 1;
			}
			switch (desc.m_BillboardMode)
			{
				case EBillboardMode.ScreenAligned:
				case EBillboardMode.ViewposAligned:
					// float Rotation
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleRotation] = m_VertexSize;
					m_VertexSize += 1;
					// no Axis0
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleAxis0] = 0;
					// no Axis1
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleAxis1] = 0;
					mat.EnableKeyword("BB_FeatureC0");
					break;
				case EBillboardMode.AxisAligned:
				case EBillboardMode.AxisAlignedSpheroid:
					// no Rotation
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleRotation] = 0;
					// float3 Axis0
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleAxis0] = m_VertexSize;
					m_VertexSize += 3;
					// no Axis1
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleAxis1] = 0;
					mat.EnableKeyword("BB_FeatureC1");
					break;
				case EBillboardMode.AxisAlignedCapsule:
					// no Rotation
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleRotation] = 0;
					// float3 Axis0
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleAxis0] = m_VertexSize;
					m_VertexSize += 3;
					// no Axis1
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleAxis1] = 0;
					mat.EnableKeyword("BB_FeatureC1_Capsule");
					// 12 indices per particle:
					m_VertexMultiplier = 12;
					break;
				case EBillboardMode.PlaneAligned:
					// float Rotation
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleRotation] = m_VertexSize;
					m_VertexSize += 1;
					// float3 Axis0
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleAxis0] = m_VertexSize;
					m_VertexSize += 3;
					// float3 Axis1
					m_Offsets[(int)EBillboardingInputs.GenInput_ParticleAxis1] = m_VertexSize;
					m_VertexSize += 3;
					mat.EnableKeyword("BB_FeatureC2");
					break;

				default:
					break;
			}
			if (desc.HasShaderVariationFlag(EShaderVariationFlags.Has_Color))
			{
				// float4 Color
				m_Offsets[(int)EBillboardingInputs.GenInput_Color] = m_VertexSize;
				m_VertexSize += 4;
			}
			if (desc.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaRemap))
			{
				// float1 AlphaCursor
				m_Offsets[(int)EBillboardingInputs.GenInput_AlphaCursor] = m_VertexSize;
				m_VertexSize += 1;
			}
			if (desc.HasShaderVariationFlag(EShaderVariationFlags.Has_Atlas))
			{
				// float1 TextureID
				m_Offsets[(int)EBillboardingInputs.GenInput_TextureID] = m_VertexSize;
				m_VertexSize += 1;
				if (desc.HasShaderVariationFlag(EShaderVariationFlags.Has_AnimBlend))
					mat.EnableKeyword("PK_HAS_ANIM_BLEND");
				else
					mat.EnableKeyword("BB_Feature_Atlas");
			}

			if (desc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
				m_Distortion = true;
		
			mat.SetBuffer("_Atlas", m_AtlasInfo);
			mat.SetBuffer("_BillboardInfo", m_BillboardInfo);
			mat.SetBuffer("_InputOffsets", m_InputOffsets);
#endif
			m_RenderingPlugin = FindObjectOfType<PKFxRenderingPlugin>();
		}

		[ExecuteAlways]
		void Update()
		{
			if (m_Distortion) // Distortion render is called by camera holding the command buffer
				return;
			if (m_RenderingPlugin == null)
				return;


			if (PKFxManager.IsUnitTesting)
				RenderParticles(null);
			else
			{
				for (int i = 0; i < m_RenderingPlugin.m_Cameras.Count; ++i)
				{
					if (i < m_RenderingPlugin.m_MaxCameraSupport)
					{
						RenderParticles(m_RenderingPlugin.m_Cameras[i].GetComponent<Camera>());
					}
				}
			}

#if UNITY_EDITOR
			foreach (SceneView sc in SceneView.sceneViews)
			{
				if (sc != null &&
					!(float.IsNaN(sc.rootVisualElement.contentRect.width) || float.IsNaN(sc.rootVisualElement.contentRect.height)))
				{
					Camera camera = sc.camera;
					RenderParticleSceneView(camera);
				}
			}
#endif
			m_ParticleCount = m_NewParticleCount;
		}

		private void OnSetParticleCount()
		{
		}

		private void RenderParticles(Camera camera)
		{
			if (m_RawBuffer != null && m_InstancedMaterial != null)
			{
				if (m_UseIndirectDraw)
					Graphics.DrawProceduralIndirect(m_InstancedMaterial, m_Bounds, MeshTopology.Triangles, m_DrawIndirectArgs, 0, null);// camera);
				else
					Graphics.DrawProcedural(m_InstancedMaterial, m_Bounds, MeshTopology.Triangles, m_VertexMultiplier * m_ParticleCount, 1, null);//camera);
			}
		}

		private void RenderParticleSceneView(Camera camera)
		{
#if UNITY_EDITOR
			if (m_RawBuffer != null && m_InstancedMaterial != null)
			{
				if (m_UseIndirectDraw)
					Graphics.DrawProceduralIndirect(m_InstancedMaterial, m_Bounds, MeshTopology.Triangles, m_DrawIndirectArgs, 0, camera);
				else
					Graphics.DrawProcedural(m_InstancedMaterial, m_Bounds, MeshTopology.Triangles, m_VertexMultiplier * m_ParticleCount, 1, camera);
			}
#endif// UNITY_EDITOR
		}

#if UNITY_EDITOR
		private void OnDrawGizmos()
		{
			if (m_ParticleCount > 0 && m_RawBuffer != null)
			{
				if (PKFxSettings.DebugEffectsBoundingBoxes)
				{
					Gizmos.color = PKFxManagerImpl.m_Renderers[m_GUID].m_BoundsDebugColor;
					Gizmos.DrawWireCube(m_Bounds.center, m_Bounds.size);
				}
			}
		}
#endif// UNITY_EDITOR

	}
}