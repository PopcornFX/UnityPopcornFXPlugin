//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Serialization;

namespace PopcornFX
{
	public class PKFxRenderFeatureBinding : PKFxShaderInputBindings
	{
		public bool						m_UseShader = true;
#if UNITY_EDITOR
		public Shader					m_Shader = null;
		public Material					m_Material = null;
#endif

		public bool[]					m_RenderTypes;
		public bool[]					m_BlendMode;
		public bool						m_DoubleSided;
		private bool					m_UseShaderGraph;
		[FormerlySerializedAs("m_ShaderMask")]
		public EShaderVariationFlags	m_SupportedShaderMask = 0;
		public EShaderVariationFlags	m_MandatoryShaderMask = 0;
		public EBillboardLocation		m_BillboardingLocation = EBillboardLocation.CPU;

		public PKFxRenderFeatureBinding()
		{
			m_BlendMode = new bool[(int)EBlendMode.UniformFlags_Count];
			m_RenderTypes = new bool[(int)ERendererType.RendererType_Count];
			m_DoubleSided = false;
			m_UseShaderGraph = false;
		}

		public void Awake()
		{
			if (m_BlendMode.Length < (int)EBlendMode.UniformFlags_Count)
			{
				bool[] upgrade = new bool[(int)EBlendMode.UniformFlags_Count];
				for (uint i = 0; i < m_BlendMode.Length; ++i)
				{
					upgrade[i] = m_BlendMode[i];
				}
				m_BlendMode = upgrade;
			}
			if (m_RenderTypes.Length < (int)ERendererType.RendererType_Count)
			{
				bool[] upgrade = new bool[(int)ERendererType.RendererType_Count];
				for (uint i = 0; i < m_RenderTypes.Length; ++i)
				{
					upgrade[i] = m_RenderTypes[i];
				}
				m_RenderTypes = upgrade;
			}
			m_UseShaderGraph = (UnityEngine.Rendering.GraphicsSettings.currentRenderPipeline != null &&
								(UnityEngine.Rendering.GraphicsSettings.currentRenderPipeline.name == "UniversalRenderPipelineAsset" ||
								UnityEngine.Rendering.GraphicsSettings.currentRenderPipeline.name == "HDRenderPipelineAsset"));
		}

		public void OnEnable()
		{
			m_UseShaderGraph = (UnityEngine.Rendering.GraphicsSettings.currentRenderPipeline != null &&
								(UnityEngine.Rendering.GraphicsSettings.currentRenderPipeline.name == "UniversalRenderPipelineAsset" ||
								UnityEngine.Rendering.GraphicsSettings.currentRenderPipeline.name == "HDRenderPipelineAsset"));
		}

		public bool IsMatchingRendererDesc(SBatchDesc desc)
		{
			if (!m_RenderTypes[(int)desc.m_Type])
				return false;
			if (desc.m_Type == ERendererType.Billboard &&
				(PKFxSettings.UseGPUBillboarding && m_BillboardingLocation == EBillboardLocation.CPU ||
				!PKFxSettings.UseGPUBillboarding && m_BillboardingLocation == EBillboardLocation.GPU))
				return false;
			if (!m_BlendMode[(int)desc.m_BlendMode])
				return false;

			if (m_UseShaderGraph && (m_DoubleSided != desc.m_DoubleSided))
				return false;

			int supportedShaderMask = (int)m_SupportedShaderMask;
			int mandatoryShaderMask = (int)m_MandatoryShaderMask;

			if ((supportedShaderMask & desc.m_ShaderVariationFlags) != desc.m_ShaderVariationFlags)
				return false;
			if ((mandatoryShaderMask & desc.m_ShaderVariationFlags) != mandatoryShaderMask)
				return false;

			return true;
		}

		public bool IsMatchingRendererDesc(SMeshDesc desc)
		{
			return false;
		}
	}
}