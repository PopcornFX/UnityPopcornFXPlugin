//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;

namespace PopcornFX
{
	public enum EBillboardLocation : int
	{
		CPU,
		GPU
	}

	public enum EBillboardMode : int
	{
		ScreenAligned,
		ViewposAligned,
		AxisAligned,
		AxisAlignedSpheroid,
		AxisAlignedCapsule,
		PlaneAligned,
		BillboardMode_Count,
	}

	public enum EShaderVariationFlags : int
	{
		Has_RibbonComplex = (1 << 0),
		Has_AnimBlend = (1 << 1),
		Has_AlphaRemap = (1 << 2),
		Has_Lighting = (1 << 3),
		Has_LightingLegacy = (1 << 4),
		Has_Soft = (1 << 5),
		Has_DistortionMap = (1 << 6),
		Has_Color = (1 << 7),
		Has_DiffuseMap = (1 << 8),
		Has_DoubleSided = (1 << 9),
		Has_CastShadow = (1 << 10),
		Has_Atlas = (1 << 11),
		Has_Size2 = (1 << 12),
		Has_DiffuseRamp = (1 << 13)
	};

	public enum EBlendMode : int
	{
		// Transparent:
		Additive = 0,
		AdditiveNoAlpha,
		AlphaBlend,
		PremultipliedAlpha,
		// Opaque:
		Solid,
		Masked,
		UniformFlags_Count,
	};

	public enum ERendererType : int
	{
		Billboard,
		Ribbon,
		Mesh,
		Triangle,
		RendererType_Count
	};

	[Serializable]
	public class SBatchLitFeatureDesc
	{
		public string m_NormalMap;
		public string m_RoughMetalMap;

		public bool m_CastShadows;

		public float m_NormalBendingFactor;
		public float m_Roughness;
		public float m_Metalness;

		public SBatchLitFeatureDesc(SRenderingFeatureLitDesc desc)
		{
			string normalStr = null, roughtMetalStr = null;

			if (desc.m_NormalMap != IntPtr.Zero)
			{
				string normalTextureFileName = Marshal.PtrToStringAnsi(desc.m_NormalMap);
				string filePath = Path.GetDirectoryName(normalTextureFileName) + "/" + Path.GetFileNameWithoutExtension(normalTextureFileName);
				string extension = Path.GetExtension(normalTextureFileName);
				normalStr = filePath.Replace('\\', '/') + "_linear" + extension;
			}
			if (desc.m_RoughMetalMap != IntPtr.Zero)
			{
				string RMTextureFileName = Marshal.PtrToStringAnsi(desc.m_RoughMetalMap);
				string filePath = Path.GetDirectoryName(RMTextureFileName) + "/" + Path.GetFileNameWithoutExtension(RMTextureFileName);
				string extension = Path.GetExtension(RMTextureFileName);
				roughtMetalStr = filePath.Replace('\\', '/') + "_linear" + extension;
			}
			m_NormalMap = normalStr;
			m_RoughMetalMap = roughtMetalStr;

			m_CastShadows = desc.m_CastShadows != 0 ? true : false;

			m_NormalBendingFactor = desc.m_NormalBendingFactor;
			m_Roughness = desc.m_Roughness;
			m_Metalness = desc.m_Metalness;
		}

		internal string GetGeneratedName()
		{
			string name = "[Lit:";

			name += " ";
			name += m_NormalMap == null ? "none" : m_NormalMap;
			name += " ";
			name += m_RoughMetalMap == null ? "none" : m_RoughMetalMap;
			name += " ";
			name += m_CastShadows ? "Shadows" : "noShadows";
			name += " ";
			name += m_NormalBendingFactor;
			name += " ";
			name += m_Roughness;
			name += " ";
			name += m_Metalness;

			name += "]";
			return name;
		}
	}


	[Serializable]
	public class SEventDesc
	{
		public int		m_Slot;
		public string	m_Name;

		public SEventDesc(SNativeEventDesc desc)
		{
			m_Name = Marshal.PtrToStringAnsi(desc.m_EventName);
			m_Slot = desc.m_Slot;
		}
		public override string ToString()
		{
			return m_Slot.ToString() + ":" + m_Name.ToString();
		}
	}

	[Serializable]
	public class SBatchDesc
	{
		public ERendererType		m_Type;
		public int					m_ShaderVariationFlags;
		public EBlendMode			m_BlendMode;
		public bool					m_RotateUVs;
		public int					m_DrawOrder;
		public string				m_DiffuseMap;
		// Billboards/Ribbons:
		public string				m_AlphaRemap;
		public string				m_DiffuseRampMap;
		public EBillboardMode		m_BillboardMode;
		//  For meshes:
		public string				m_MeshAsset;
		public string				m_SpecularMap;
		public bool					m_HasMeshAtlas;
		public float				m_InvSoftnessDistance;
		public string				m_GeneratedName;
		//Lit Feature
		public SBatchLitFeatureDesc	m_LitFeature;

		//Internal
		internal int				m_InternalId;
		public SBatchDesc(ERendererType type, SPopcornRendererDesc desc, int idx)
		{
			string diffuseStr = null;
			string diffuseRampStr = null;
			string alphaRemapStr = null;

			if (desc.m_DiffuseMap != IntPtr.Zero)
				diffuseStr = Marshal.PtrToStringAnsi(desc.m_DiffuseMap);
			if (desc.m_AlphaRemap != IntPtr.Zero)
				alphaRemapStr = Marshal.PtrToStringAnsi(desc.m_AlphaRemap);
			if (desc.m_DiffuseRampMap != IntPtr.Zero)
				diffuseRampStr = Marshal.PtrToStringAnsi(desc.m_DiffuseRampMap);

			m_Type = type;
			m_ShaderVariationFlags = desc.m_ShaderVariationFlags;
			m_BlendMode = desc.m_BlendMode;
			m_RotateUVs = desc.m_RotateTexture != 0 ? true : false;
			m_DrawOrder = desc.m_DrawOrder;
			m_DiffuseMap = diffuseStr;
			m_DiffuseRampMap = diffuseRampStr;
			m_AlphaRemap = alphaRemapStr;
			m_BillboardMode = desc.m_BillboardMode;
			m_InvSoftnessDistance = desc.m_InvSoftnessDistance;

			unsafe
			{
				SRenderingFeatureLitDesc* litDesc = (SRenderingFeatureLitDesc*)desc.m_LitRendering.ToPointer();
				if (litDesc != null)
					m_LitFeature = new SBatchLitFeatureDesc(*litDesc);
				else
					m_LitFeature = null;
			}

			m_GeneratedName = GenerateNameFromDescription();
			m_InternalId = idx;
		}

		public SBatchDesc(SMeshRendererDesc desc, int idx)
		{
			string diffuseStr = null;
			string meshAssetStr = null;

			if (desc.m_DiffuseMap != IntPtr.Zero)
				diffuseStr = Marshal.PtrToStringAnsi(desc.m_DiffuseMap);
			if (desc.m_MeshAsset != IntPtr.Zero)
				meshAssetStr = Path.ChangeExtension(Marshal.PtrToStringAnsi(desc.m_MeshAsset), ".fbx");

			m_Type = ERendererType.Mesh;
			m_ShaderVariationFlags = desc.m_ShaderVariationFlags;
			m_BlendMode = desc.m_BlendMode;
			m_RotateUVs = false;
			m_DiffuseMap = diffuseStr;
			m_SpecularMap = null;
			m_MeshAsset = meshAssetStr;
			m_HasMeshAtlas = desc.m_HasMeshAtlas == 1 ? true : false;
			

			unsafe
			{
				SRenderingFeatureLitDesc* litDesc = (SRenderingFeatureLitDesc*)desc.m_LitRendering.ToPointer();
				if (litDesc != null)
					m_LitFeature = new SBatchLitFeatureDesc(*litDesc);
				else
					m_LitFeature = null;
			}
			m_GeneratedName = GenerateNameFromDescription();
			m_InternalId = idx;
		}

		public bool HasShaderVariationFlag(EShaderVariationFlags flag)
		{
			return (m_ShaderVariationFlags & (int)flag) == (int)flag;
		}

		// All of those functions are used to generate a description for the renderer:
		public static string MaterialFlagsToString(int materialFlags)
		{
			string finalName = "";

			if ((materialFlags & (int)EShaderVariationFlags.Has_RibbonComplex) != 0)
				finalName += " RibbonComplex";
			if ((materialFlags & (int)EShaderVariationFlags.Has_Atlas) != 0)
				finalName += " Atlas";
			if ((materialFlags & (int)EShaderVariationFlags.Has_AnimBlend) != 0)
				finalName += " AnimBlend";
			if ((materialFlags & (int)EShaderVariationFlags.Has_AlphaRemap) != 0)
				finalName += " AlphaRemap";
			if ((materialFlags & (int)EShaderVariationFlags.Has_DiffuseRamp) != 0)
				finalName += " DiffuseRamp";
			if ((materialFlags & (int)EShaderVariationFlags.Has_Lighting) != 0)
				finalName += " Lighting";
			if ((materialFlags & (int)EShaderVariationFlags.Has_Soft) != 0)
				finalName += " Soft";
			if ((materialFlags & (int)EShaderVariationFlags.Has_DistortionMap) != 0)
				finalName += " Distortion";
			if ((materialFlags & (int)EShaderVariationFlags.Has_Color) != 0)
				finalName += " Color";
			if ((materialFlags & (int)EShaderVariationFlags.Has_DiffuseMap) != 0)
				finalName += " Diffuse";
			if ((materialFlags & (int)EShaderVariationFlags.Has_DoubleSided) != 0)
				finalName += " DoubleSided";
			if ((materialFlags & (int)EShaderVariationFlags.Has_CastShadow) != 0)
				finalName += " CastShadow";
			return finalName.Length == 0 ? " MaterialBasic" : finalName;
		}

		public static string BlendModeToString(EBlendMode blendMode)
		{
			string finalName = "";
			if (blendMode == EBlendMode.Additive)
				finalName += " Additive";
			if (blendMode == EBlendMode.AdditiveNoAlpha)
				finalName += " AdditiveNoAlpha";
			if (blendMode == EBlendMode.AlphaBlend)
				finalName += " AlphaBlend";
			if (blendMode == EBlendMode.PremultipliedAlpha)
				finalName += " PremultipliedAlpha";
			if (blendMode == EBlendMode.Solid)
				finalName += " Solid";
			if (blendMode == EBlendMode.Masked)
				finalName += " Masked";
			return finalName;
		}

		public string GenerateNameFromDescription()
		{
			string finalName;

			if (m_Type == ERendererType.Billboard)
				finalName = "Billboard";
			else if (m_Type == ERendererType.Ribbon)
				finalName = "Ribbon";
			else if (m_Type == ERendererType.Mesh)
				finalName = "Mesh";
			else if (m_Type == ERendererType.Triangle)
				finalName = "Triangle";
			else
				finalName = "Unknown";
			finalName += " ";
			finalName += MaterialFlagsToString(m_ShaderVariationFlags);
			finalName += " ";
			finalName += BlendModeToString(m_BlendMode);
			finalName += " ";
			finalName += m_RotateUVs ? "RotateUvs" : "UVs";
			finalName += " ";
			finalName += m_DiffuseMap == null ? "(none)" : m_DiffuseMap;
			finalName += " ";
			finalName += m_LitFeature == null ? "(none)" : m_LitFeature.GetGeneratedName();
			if (m_Type != ERendererType.Mesh)
			{
				finalName += " ";
				finalName += m_AlphaRemap == null ? "(none)" : m_AlphaRemap;
				finalName += " ";
				finalName += m_DiffuseRampMap == null ? "(none)" : m_DiffuseRampMap;
				finalName += " ";
				finalName += m_InvSoftnessDistance;
			}
			else
			{
				finalName += " ";
				finalName += m_SpecularMap == null ? "(none)" : m_SpecularMap;
			}
			return finalName;
		}
	}

	public class SMeshDesc
	{
		public MeshFilter m_Slice;
		public PKFxProceduralRenderer m_Procedural;
		public Material m_Material;
		public GameObject m_RenderingObject;
		public SBatchDesc m_BatchDesc;
		public PKFxMeshInstancesRenderer m_InstancesRenderer;
#if UNITY_EDITOR
		public Color m_BoundsDebugColor;
#endif

		public SMeshDesc(Material mat, SBatchDesc batchDesc, GameObject renderingObject)
		{
			m_Slice = null;
			m_RenderingObject = renderingObject;
			m_Material = mat;
			m_BatchDesc = batchDesc;
			m_InstancesRenderer = null;
#if UNITY_EDITOR
			m_BoundsDebugColor = UnityEngine.Random.ColorHSV(0f, 1f, 1f, 1f, 0.5f, 1f);
#endif
		}

		public SMeshDesc(MeshFilter m, Material mat, SBatchDesc batchDesc, PKFxMeshInstancesRenderer mir, GameObject renderingObject)
		{
			m_Slice = m;
			m_RenderingObject = renderingObject;
			m_Material = mat;
			m_BatchDesc = batchDesc;
			m_InstancesRenderer = mir;
#if UNITY_EDITOR
			m_BoundsDebugColor = UnityEngine.Random.ColorHSV(0f, 1f, 1f, 1f, 0.5f, 1f);
#endif
		}

		public bool HasShaderVariationFlag(EShaderVariationFlags flag)
		{
			return (m_BatchDesc.m_ShaderVariationFlags & (int)flag) == (int)flag;
		}
	}

}
