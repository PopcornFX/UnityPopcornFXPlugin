//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PopcornFX
{
	public class PKFxRenderFeatureBinding : ScriptableObject
	{
		public bool						m_UseShader = true;
		public Shader					m_Shader = null;
		public Material					m_Material = null;

		public bool[]					m_RenderTypes;
		public bool[]					m_BlendMode;
		public EShaderVariationFlags	m_ShaderMask = 0;
		public EBillboardLocation		m_BillboardingLocation = EBillboardLocation.CPU;

		//Advanced
		public string					m_MeshColorPropertyName = "_BaseColor";
		public string					m_NormalMapPropertyName = null;

		public PKFxRenderFeatureBinding()
		{
			m_BlendMode = new bool[(int)EBlendMode.UniformFlags_Count];
			m_RenderTypes = new bool[(int)ERendererType.RendererType_Count];
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

			if ((((int)m_ShaderMask ^ desc.m_ShaderVariationFlags) & desc.m_ShaderVariationFlags) != 0)
				return false;

			return true;
		}

		public bool IsMatchingRendererDesc(SMeshDesc desc)
		{
			return false;
		}

		public Material GetMaterial()
		{
			if (m_UseShader)
			{
				if (m_Shader == null)
					return null;
				return new Material(m_Shader);
			}
			else
			{
				if (m_Material == null)
					return null;
				return new Material(m_Material);
			}
		}

		public string GetMeshColorPropertyName()
		{
			if (m_MeshColorPropertyName == null || m_MeshColorPropertyName.Length == 0)
				return null;
			return m_MeshColorPropertyName;
		}

		public string GetNormalMapPropertyName()
		{
			if (m_NormalMapPropertyName == null || m_NormalMapPropertyName.Length == 0)
				return null;
			return m_NormalMapPropertyName;
		}
	}
}