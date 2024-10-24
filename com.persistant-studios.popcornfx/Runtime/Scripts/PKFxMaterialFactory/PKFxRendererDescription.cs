//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.IO;
using System.Runtime.InteropServices;
using UnityEditor;
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
		Has_DiffuseRamp = (1 << 13),
		Has_FluidVAT = (1 << 14),
		Has_SoftVAT = (1 << 15),
		Has_RigidVAT = (1 << 16),
		Has_Emissive = (1 << 17),
		Has_EmissiveRamp = (1 << 18),
		Has_SkeletalAnim = (1 << 19),
		Has_SkeletalInterpol = (1 << 20),
		Has_SkeletalTrackInterpol = (1 << 21),
		Has_TransformUVs = (1 << 22),
        Has_LegacyLightingOpaque = (1 << 23),
        Has_AlphaMasks = (1 << 24),
		Has_UVDistortions = (1 << 25),
		Has_Dissolve = (1 << 26),
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
		Decal,
		Light,
		Sound,
		RendererType_Count
	};

	[Serializable]
	public class SBatchLitFeatureDesc
	{
		public bool m_Activated = false;
		public string m_NormalMap = null;
		public string m_RoughMetalMap = null;

		public bool m_CastShadows;

		public float m_NormalBendingFactor;
		public float m_Roughness;
		public float m_Metalness;

		public SBatchLitFeatureDesc(SRenderingFeatureLitDesc desc)
		{
			m_Activated = true;

			if (desc.m_NormalMap != IntPtr.Zero)
				m_NormalMap = Marshal.PtrToStringAnsi(desc.m_NormalMap);
			if (desc.m_RoughMetalMap != IntPtr.Zero)
				m_RoughMetalMap = Marshal.PtrToStringAnsi(desc.m_RoughMetalMap);

			m_CastShadows = desc.m_CastShadows != 0 ? true : false;

			m_NormalBendingFactor = desc.m_NormalBendingFactor;
			m_Roughness = desc.m_Roughness;
			m_Metalness = desc.m_Metalness;
		}

		internal string GetGeneratedShortName()
		{
			string name = "Lit_";

			name += (m_NormalMap == null || m_NormalMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_NormalMap) + "_";
			name += (m_RoughMetalMap == null || m_RoughMetalMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_RoughMetalMap) + "_";
			name += m_CastShadows ? "S_" : "";
			name += String.Format("{0:0.##}", m_NormalBendingFactor) + "_";
			name += String.Format("{0:0.##}", m_Roughness) + "_";
			name += String.Format("{0:0.##}", m_Metalness) + "_";

			return name;
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
			name += String.Format("{0:0.##}", m_NormalBendingFactor);
			name += " ";
			name += String.Format("{0:0.##}", m_Roughness);
			name += " ";
			name += String.Format("{0:0.##}", m_Metalness);

			name += "]";
			return name;
		}
	}

    [Serializable]
    public class SBatchAlphaMasksFeatureDesc
    {
        public bool m_Activated = false;

        public string m_AlphaMasks1Map;
        public string m_AlphaMasks2Map;
        public float m_AlphaMask1Intensity;
        public float m_AlphaMask2Intensity;
        public float m_AlphaMask1Weight;
        public float m_AlphaMask2Weight;
        public Vector2 m_AlphaMask1Scale;
        public Vector2 m_AlphaMask2Scale;
        public float m_AlphaMask1RotationSpeed;
        public float m_AlphaMask2RotationSpeed;
        public Vector2 m_AlphaMask1TranslationSpeed;
        public Vector2 m_AlphaMask2TranslationSpeed;

        public SBatchAlphaMasksFeatureDesc(SRenderingFeatureAlphaMasksDesc desc)
        {
            if (desc.m_AlphaMasks1Map != IntPtr.Zero)
                m_AlphaMasks1Map = Marshal.PtrToStringAnsi(desc.m_AlphaMasks1Map);
            if (desc.m_AlphaMasks2Map != IntPtr.Zero)
                m_AlphaMasks2Map = Marshal.PtrToStringAnsi(desc.m_AlphaMasks2Map);

            m_AlphaMask1Intensity = desc.m_AlphaMask1Intensity;
            m_AlphaMask2Intensity = desc.m_AlphaMask2Intensity;
            m_AlphaMask1Weight = desc.m_AlphaMask1Weight;
            m_AlphaMask2Weight = desc.m_AlphaMask2Weight;
            m_AlphaMask1Scale = desc.m_AlphaMask1Scale;
            m_AlphaMask2Scale = desc.m_AlphaMask2Scale;
            m_AlphaMask1RotationSpeed = desc.m_AlphaMask1RotationSpeed;
            m_AlphaMask2RotationSpeed = desc.m_AlphaMask2RotationSpeed;
            m_AlphaMask1TranslationSpeed = desc.m_AlphaMask1TranslationSpeed;
            m_AlphaMask2TranslationSpeed = desc.m_AlphaMask2TranslationSpeed;
        }

        internal string GetGeneratedShortName()
        {
            string name = "AlphaMasks_";

            name += (m_AlphaMasks1Map == null || m_AlphaMasks1Map.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_AlphaMasks1Map) + "_";
			name += String.Format("{0:0.##}", m_AlphaMask1Intensity);
			name += String.Format("{0:0.##}", m_AlphaMask1Weight);
			name += m_AlphaMask1Scale.ToString("F2");
			name += String.Format("{0:0.##}", m_AlphaMask1RotationSpeed);
			name += m_AlphaMask1TranslationSpeed.ToString("F2");

			name += (m_AlphaMasks2Map == null || m_AlphaMasks2Map.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_AlphaMasks2Map) + "_";
			name += String.Format("{0:0.##}", m_AlphaMask2Intensity);
			name += String.Format("{0:0.##}", m_AlphaMask2Weight);
			name += m_AlphaMask2Scale.ToString("F2");
			name += String.Format("{0:0.##}", m_AlphaMask2RotationSpeed);
			name += m_AlphaMask2TranslationSpeed.ToString("F2");
			return name;
        }

        internal string GetGeneratedName()
        {
            string name = "[AlphaMasks:";

                name += " ";
                name += m_AlphaMasks1Map == null ? "none" : m_AlphaMasks1Map;
                name += " ";
				name += m_AlphaMask1Intensity;
				name += " ";
				name += m_AlphaMask1Weight;
				name += " ";
				name += m_AlphaMask1Scale.ToString("F2");
				name += " ";
				name += m_AlphaMask1RotationSpeed;
				name += " ";
				name += m_AlphaMask1TranslationSpeed;

				name += " ";
				name += m_AlphaMasks2Map == null ? "none" : m_AlphaMasks2Map;
				name += " ";
				name += m_AlphaMask2Intensity;
				name += " ";
				name += m_AlphaMask2Weight;
				name += " ";
				name += m_AlphaMask2Scale.ToString("F2");
				name += " ";
				name += m_AlphaMask2RotationSpeed;
				name += " ";
				name += m_AlphaMask2TranslationSpeed;

			name += "]";
            return name;
        }
    }

	[Serializable]
	public class SBatchUVDistortionsFeatureDesc
	{
		public bool m_Activated = false;

		public string m_UVDistortions1Map;
		public string m_UVDistortions2Map;
		public float m_UVDistortions1Intensity;
		public float m_UVDistortions2Intensity;
		public Vector2 m_UVDistortions1Scale;
		public Vector2 m_UVDistortions2Scale;
		public float m_UVDistortions1RotationSpeed;
		public float m_UVDistortions2RotationSpeed;
		public Vector2 m_UVDistortions1TranslationSpeed;
		public Vector2 m_UVDistortions2TranslationSpeed;

		public SBatchUVDistortionsFeatureDesc(SRenderingFeatureUVDistortionsDesc desc)
		{
			if (desc.m_UVDistortions1Map != IntPtr.Zero)
				m_UVDistortions1Map = Marshal.PtrToStringAnsi(desc.m_UVDistortions1Map);
			if (desc.m_UVDistortions2Map != IntPtr.Zero)
				m_UVDistortions2Map = Marshal.PtrToStringAnsi(desc.m_UVDistortions2Map);

			m_UVDistortions1Intensity = desc.m_UVDistortions1Intensity;
			m_UVDistortions2Intensity = desc.m_UVDistortions2Intensity;
			m_UVDistortions1Scale = desc.m_UVDistortions1Scale;
			m_UVDistortions2Scale = desc.m_UVDistortions2Scale;
			m_UVDistortions1RotationSpeed = desc.m_UVDistortions1RotationSpeed;
			m_UVDistortions2RotationSpeed = desc.m_UVDistortions2RotationSpeed;
			m_UVDistortions1TranslationSpeed = desc.m_UVDistortions1TranslationSpeed;
			m_UVDistortions2TranslationSpeed = desc.m_UVDistortions2TranslationSpeed;
		}

		internal string GetGeneratedShortName()
		{
			string name = "UVDistortions_";

			name += (m_UVDistortions1Map == null || m_UVDistortions1Map.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_UVDistortions1Map) + "_";
			name += String.Format("{0:0.##}", m_UVDistortions1Intensity);
			name += m_UVDistortions1Scale.ToString("F2");
			name += String.Format("{0:0.##}", m_UVDistortions1RotationSpeed);
			name += m_UVDistortions1TranslationSpeed.ToString("F2");

			name += (m_UVDistortions2Map == null || m_UVDistortions2Map.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_UVDistortions2Map) + "_";
			name += String.Format("{0:0.##}", m_UVDistortions2Intensity);
			name += m_UVDistortions2Scale.ToString("F2");
			name += String.Format("{0:0.##}", m_UVDistortions2RotationSpeed);
			name += m_UVDistortions2TranslationSpeed.ToString("F2");

			return name;
		}

		internal string GetGeneratedName()
		{
			string name = "[UVDistortions:";

			name += " ";
			name += m_UVDistortions1Map == null ? "none" : m_UVDistortions1Map;
			name += " ";
			name += m_UVDistortions1Intensity;
			name += " ";
			name += m_UVDistortions1Scale.ToString("F2");
			name += " ";
			name += m_UVDistortions1RotationSpeed;
			name += " ";
			name += m_UVDistortions1TranslationSpeed.ToString("F2");

			name += " ";
			name += m_UVDistortions2Map == null ? "none" : m_UVDistortions2Map;
			name += " ";
			name += m_UVDistortions2Intensity;
			name += " ";
			name += m_UVDistortions2Scale.ToString("F2");
			name += " ";
			name += m_UVDistortions2RotationSpeed;
			name += " ";
			name += m_UVDistortions2TranslationSpeed.ToString("F2");
			name += " ";

			name += "]";
			return name;
		}
	}

	[Serializable]
	public class SBatchDissolveFeatureDesc
	{
		public bool m_Activated = false;

		public string m_DissolveMap;
		public float m_DissolveSoftness;

		public SBatchDissolveFeatureDesc(SRenderingFeatureDissolveDesc desc)
		{
			if (desc.m_DissolveMap != IntPtr.Zero)
				m_DissolveMap = Marshal.PtrToStringAnsi(desc.m_DissolveMap);

			m_DissolveSoftness = desc.m_DissolveSoftness;
		}

		internal string GetGeneratedShortName()
		{
			string name = "Dissolve_";
			// TODO BOTH
			name += (m_DissolveMap == null || m_DissolveMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_DissolveMap) + "_";
			name += String.Format("{0:0.##}", m_DissolveSoftness);
			return name;
		}

		internal string GetGeneratedName()
		{
			string name = "[Dissolve:";

			name += " ";
			name += m_DissolveMap == null ? "none" : m_DissolveMap;
			name += " ";
			name += m_DissolveSoftness;
			name += " ";

			name += "]";
			return name;
		}
	}

	[Serializable]
	public class SBatchVatFeatureDesc
	{
		public bool m_Activated = false;
		public string m_PositionMap = null;
		public string m_NormalMap = null;
		public string m_ColorMap = null;
		public string m_RotationMap = null;
		public int m_NumFrames;
		public bool m_PackedData;
		public Vector2 m_BoundsPivot;
		public bool m_NormalizedData;
		public Vector2 m_BoundsPosition;
		public bool m_PadToPowerOf2;
		public Vector2 m_PaddedRatio;

		public SBatchVatFeatureDesc(SRenderingFeatureVATDesc desc)
		{
			m_Activated = true;
			if (desc.m_PositionMap != IntPtr.Zero)
			{
				m_PositionMap = Marshal.PtrToStringAnsi(desc.m_PositionMap);
			}
			if (desc.m_NormalMap != IntPtr.Zero)
			{
				m_NormalMap = Marshal.PtrToStringAnsi(desc.m_NormalMap);
			}
			if (desc.m_ColorMap != IntPtr.Zero)
			{
				m_ColorMap = Marshal.PtrToStringAnsi(desc.m_ColorMap);
			}
			if (desc.m_RotationMap != IntPtr.Zero)
			{
				m_RotationMap = Marshal.PtrToStringAnsi(desc.m_RotationMap);
			}

			m_NumFrames = desc.m_NumFrames;
			m_PackedData = desc.m_PackedData != 0 ? true : false;
			m_BoundsPivot = desc.m_BoundsPivot;
			m_NormalizedData = desc.m_NormalizedData != 0 ? true : false;
			m_BoundsPosition = desc.m_BoundsPosition;
			m_PadToPowerOf2 = desc.m_PadToPowerOf2 != 0 ? true : false;
			m_PaddedRatio = desc.m_PaddedRatio;
		}

		internal string GetGeneratedShortName()
		{
			string name = "VAT_";

			name += (m_PositionMap == null || m_PositionMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_PositionMap);
			name += (m_NormalMap == null || m_NormalMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_NormalMap);
			name += (m_ColorMap == null || m_ColorMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_ColorMap);
			name += (m_RotationMap == null || m_RotationMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_RotationMap);
			name += m_NumFrames;
			name += m_PackedData ? "P" : "UP";
			name += m_BoundsPivot.ToString("F2");
			name += m_NormalizedData ? "N" : "U";
			name += m_BoundsPosition.ToString("F2");
			return name;
		}
		internal string GetGeneratedName()
		{
			string name = "[Vat:";

			name += " ";
			name += m_PositionMap == null ? "none" : m_PositionMap;
			name += " ";
			name += m_NormalMap == null ? "none" : m_NormalMap;
			name += " ";
			name += m_ColorMap == null ? "none" : m_ColorMap;
			name += " ";
			name += m_RotationMap == null ? "none" : m_RotationMap;
			name += " ";
			name += m_NumFrames;
			name += " ";
			name += m_PackedData ? "Packed Data" : "Unpacked Data";
			name += " ";
			name += m_BoundsPivot.ToString("F2");
			name += " ";
			name += m_NormalizedData ? "Normalized Data" : "notNormalized Data";
			name += " ";
			name += m_BoundsPosition.ToString("F2");
			name += "]";
			return name;
		}
	}

	[Serializable]
	public class SBatchSkeletalAnimFeatureDesc
	{
		public bool m_Activated = false;
		public string m_AnimTextureMap = null;
		public int m_TextureResolX;
		public int m_TextureResolY;
		public int m_AnimCount;
		public bool m_UseBoneScale;
		public Vector3 m_TranslationBoundsMin;
		public Vector3 m_TranslationBoundsMax;
		public Vector3 m_ScaleBoundsMin;
		public Vector3 m_ScaleBoundsMax;

		public SBatchSkeletalAnimFeatureDesc(SRenderingFeatureSkeletalAnimDesc desc)
		{
			m_Activated = true;
			if (desc.m_AnimTextureMap != IntPtr.Zero)
			{
				m_AnimTextureMap = Marshal.PtrToStringAnsi(desc.m_AnimTextureMap);
			}
			m_TextureResolX = desc.m_TextureResolX;
			m_TextureResolY = desc.m_TextureResolY;
			m_AnimCount = desc.m_AnimCount;
			m_UseBoneScale = desc.m_UseBoneScale != 0 ? true : false;
			m_TranslationBoundsMin = desc.m_TranslationBoundsMin;
			m_TranslationBoundsMax = desc.m_TranslationBoundsMax;
			m_ScaleBoundsMin = desc.m_ScaleBoundsMin;
			m_ScaleBoundsMax = desc.m_ScaleBoundsMax;
		}

		internal string GetGeneratedName()
		{
			string name = "[Skeletal Anim:";

			name += " ";
			name += m_AnimTextureMap == null ? "none" : m_AnimTextureMap;
			name += " ";
			name += m_AnimCount;
			name += " ";
			name += m_TextureResolX;
			name += " ";
			name += m_TextureResolY;
			name += " ";
			name += m_AnimCount;
			name += " ";
			name += m_UseBoneScale;
			name += " ";
			name += m_TranslationBoundsMin;
			name += " ";
			name += m_TranslationBoundsMax;
			name += " ";
			name += m_ScaleBoundsMin;
			name += " ";
			name += m_ScaleBoundsMax;
			name += "]";
			return name;
		}
		internal string GetGeneratedShortName()
		{
			string name = "BAT_";

			name += (m_AnimTextureMap == null || m_AnimTextureMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_AnimTextureMap);
			name += m_AnimCount;
			name += "_";
			name += m_TextureResolX;
			name += "_";
			name += m_TextureResolY;
			name += "_";
			name += m_AnimCount;
			name += "_";
			name += m_UseBoneScale ? "UBS" : "NBS";
			name += m_TranslationBoundsMin;
			name += "_";
			name += m_TranslationBoundsMax;
			name += "_";
			name += m_ScaleBoundsMin;
			name += "_";
			name += m_ScaleBoundsMax;
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
		public string				m_CustomName;
		public ERendererType		m_Type;
		public int					m_ShaderVariationFlags;
		public EBlendMode			m_BlendMode;
        public bool					m_IsLegacy;
		public bool					m_RotateUVs;
		public int					m_DrawOrder;
		public string				m_DiffuseMap;
		public string				m_EmissiveMap;
		// Billboards/Ribbons:
		public string				m_AlphaRemap;
		public string				m_DiffuseRampMap;
		public string				m_EmissiveRampMap;
		public EBillboardMode		m_BillboardMode;
		//  For meshes:
		public string				m_MeshAsset;
		public string				m_SpecularMap;
		public bool					m_HasMeshAtlas;
		public float				m_InvSoftnessDistance;
		public float				m_AlphaClipThreshold;
		public string				m_GeneratedName;
		public bool					m_TransformUVs_RGBOnly;
		public bool					m_UseVertexColor = false;
		// For Decals:
		public Vector4				m_DiffuseColor;
		public Vector3				m_EmissiveColor;

        // VAT:
        public SBatchVatFeatureDesc m_VatFeature = null;
		// Lit:
		public SBatchLitFeatureDesc m_LitFeature = null;

		// AnimatedMasked features
        public SBatchAlphaMasksFeatureDesc m_AlphaMasksFeature = null;
		public SBatchUVDistortionsFeatureDesc m_UVDistortionsFeature = null;
		public SBatchDissolveFeatureDesc m_DissolveFeature = null;

		// Skeletal anim:
		public SBatchSkeletalAnimFeatureDesc m_SkeletalAnimFeature = null;

        public Texture2D m_AtlasSubRects = null;

		//Internal
		[SerializeField]
		internal int				m_CameraId;

		public uint					m_UID;


		public SBatchDesc(ERendererType type, SPopcornRendererDesc desc)
		{
			string customName = null;
			if (desc.m_CustomName != IntPtr.Zero)
				customName = Marshal.PtrToStringUTF8(desc.m_CustomName);

			string diffuseStr = null;
			string emissiveStr = null;
			string diffuseRampStr = null;
			string emissiveRampStr = null;
			string alphaRemapStr = null;

            if (desc.m_DiffuseMap != IntPtr.Zero)
				diffuseStr = Marshal.PtrToStringAnsi(desc.m_DiffuseMap);
			if (desc.m_EmissiveMap != IntPtr.Zero)
				emissiveStr = Marshal.PtrToStringAnsi(desc.m_EmissiveMap);
			if (desc.m_AlphaRemap != IntPtr.Zero)
				alphaRemapStr = Marshal.PtrToStringAnsi(desc.m_AlphaRemap);
			if (desc.m_DiffuseRampMap != IntPtr.Zero)
				diffuseRampStr = Marshal.PtrToStringAnsi(desc.m_DiffuseRampMap);
			if (desc.m_EmissiveRampMap != IntPtr.Zero)
				emissiveRampStr = Marshal.PtrToStringAnsi(desc.m_EmissiveRampMap);

			m_CustomName = customName;
			m_Type = type;
			m_ShaderVariationFlags = desc.m_ShaderVariationFlags;
			m_BlendMode = desc.m_BlendMode;
            m_IsLegacy = desc.m_IsLegacy != 0 ? true : false;
			m_RotateUVs = desc.m_RotateTexture != 0 ? true : false;
			m_DrawOrder = desc.m_DrawOrder;
			m_DiffuseMap = diffuseStr;
			m_EmissiveMap = emissiveStr;
			m_DiffuseRampMap = diffuseRampStr;
			m_EmissiveRampMap = emissiveRampStr;
			m_AlphaRemap = alphaRemapStr;
           
            m_BillboardMode = desc.m_BillboardMode;
			m_InvSoftnessDistance = desc.m_InvSoftnessDistance;
			m_AlphaClipThreshold = desc.m_AlphaClipThreshold;
			m_TransformUVs_RGBOnly = desc.m_TransformUVs_RGBOnly != 0 ? true : false;
			m_UseVertexColor = false; // Only for mesh renderers

			m_VatFeature = null;
			m_SkeletalAnimFeature = null;

			m_CameraId = desc.m_CameraId;
			m_UID = desc.m_UID;

			unsafe
			{
				SRenderingFeatureLitDesc* litDesc = (SRenderingFeatureLitDesc*)desc.m_LitRendering.ToPointer();
				if (litDesc != null)
					m_LitFeature = new SBatchLitFeatureDesc(*litDesc);
				else
					m_LitFeature = null;

                SRenderingFeatureAlphaMasksDesc* animatedMaskedDesc = (SRenderingFeatureAlphaMasksDesc*)desc.m_AlphaMasks.ToPointer();
                if (animatedMaskedDesc != null)
                    m_AlphaMasksFeature = new SBatchAlphaMasksFeatureDesc(*animatedMaskedDesc);
                else
                    m_AlphaMasksFeature = null;

				SRenderingFeatureUVDistortionsDesc* uvDistortionsDesc = (SRenderingFeatureUVDistortionsDesc*)desc.m_UVDistortions.ToPointer();
				if (uvDistortionsDesc != null)
					m_UVDistortionsFeature = new SBatchUVDistortionsFeatureDesc(*uvDistortionsDesc);
				else
					m_UVDistortionsFeature = null;

				SRenderingFeatureDissolveDesc* dissolveDesc = (SRenderingFeatureDissolveDesc*)desc.m_Dissolve.ToPointer();
				if (dissolveDesc != null)
					m_DissolveFeature = new SBatchDissolveFeatureDesc(*dissolveDesc);
				else
					m_DissolveFeature = null;
			}

			m_GeneratedName = GenerateNameFromDescription();
		}

		public SBatchDesc(SMeshRendererDesc desc)
		{
			string customName = null;
			if (desc.m_CustomName != IntPtr.Zero)
				customName = Marshal.PtrToStringUTF8(desc.m_CustomName);

			string diffuseStr = null;
			string meshAssetStr = null;
			string emissiveStr = null;
			string diffuseRampStr = null;
			string emissiveRampStr = null;
			string alphaRemapStr = null;

			if (desc.m_DiffuseMap != IntPtr.Zero)
				diffuseStr = Marshal.PtrToStringAnsi(desc.m_DiffuseMap);
			if (desc.m_MeshAsset != IntPtr.Zero)
				meshAssetStr = Path.ChangeExtension(Marshal.PtrToStringAnsi(desc.m_MeshAsset), ".fbx");
			if (desc.m_EmissiveMap != IntPtr.Zero)
				emissiveStr = Marshal.PtrToStringAnsi(desc.m_EmissiveMap);
			if (desc.m_AlphaRemap != IntPtr.Zero)
				alphaRemapStr = Marshal.PtrToStringAnsi(desc.m_AlphaRemap);
			if (desc.m_DiffuseRampMap != IntPtr.Zero)
				diffuseRampStr = Marshal.PtrToStringAnsi(desc.m_DiffuseRampMap);
			if (desc.m_EmissiveRampMap != IntPtr.Zero)
				emissiveRampStr = Marshal.PtrToStringAnsi(desc.m_EmissiveRampMap);

			m_CustomName = customName;
			m_Type = ERendererType.Mesh;
			m_ShaderVariationFlags = desc.m_ShaderVariationFlags;
			m_BlendMode = desc.m_BlendMode;
			m_DrawOrder = desc.m_DrawOrder;
			m_RotateUVs = false;
			m_DiffuseMap = diffuseStr;
			m_DiffuseRampMap = diffuseRampStr;
			m_EmissiveMap = emissiveStr;
			m_EmissiveRampMap = emissiveRampStr;
			m_AlphaRemap = alphaRemapStr;
			m_InvSoftnessDistance = desc.m_InvSofnessDistance;
			m_AlphaClipThreshold = desc.m_AlphaClipThreshold;
			m_TransformUVs_RGBOnly = desc.m_TransformUVs_RGBOnly != 0 ? true : false;
			m_UseVertexColor = desc.m_UseVertexColor != 0 ? true : false;
			m_IsLegacy = desc.m_IsLegacy != 0 ? true : false;

			m_SpecularMap = null;
			m_MeshAsset = meshAssetStr;
			m_HasMeshAtlas = desc.m_HasMeshAtlas == 1 ? true : false;

			m_UID = desc.m_UID;

			unsafe
			{
				SRenderingFeatureVATDesc* vatDesc = (SRenderingFeatureVATDesc*)desc.m_VatRendering.ToPointer();
				if (vatDesc != null)
					m_VatFeature = new SBatchVatFeatureDesc(*vatDesc);
				else
					m_VatFeature = null;
			}

			unsafe
			{
				SRenderingFeatureLitDesc* litDesc = (SRenderingFeatureLitDesc*)desc.m_LitRendering.ToPointer();
				if (litDesc != null)
					m_LitFeature = new SBatchLitFeatureDesc(*litDesc);
				else
					m_LitFeature = null;

				SRenderingFeatureAlphaMasksDesc* animatedMaskedDesc = (SRenderingFeatureAlphaMasksDesc*)desc.m_AlphaMasks.ToPointer();
				if (animatedMaskedDesc != null)
					m_AlphaMasksFeature = new SBatchAlphaMasksFeatureDesc(*animatedMaskedDesc);
				else
					m_AlphaMasksFeature = null;

				SRenderingFeatureUVDistortionsDesc* uvDistortionsDesc = (SRenderingFeatureUVDistortionsDesc*)desc.m_UVDistortions.ToPointer();
				if (uvDistortionsDesc != null)
					m_UVDistortionsFeature = new SBatchUVDistortionsFeatureDesc(*uvDistortionsDesc);
				else
					m_UVDistortionsFeature = null;

				SRenderingFeatureDissolveDesc* dissolveDesc = (SRenderingFeatureDissolveDesc*)desc.m_Dissolve.ToPointer();
				if (dissolveDesc != null)
					m_DissolveFeature = new SBatchDissolveFeatureDesc(*dissolveDesc);
				else
					m_DissolveFeature = null;
			}

			unsafe
			{
				SRenderingFeatureSkeletalAnimDesc* skeletonAnimDesc = (SRenderingFeatureSkeletalAnimDesc*)desc.m_SkeletalAnim.ToPointer();
				if (skeletonAnimDesc != null)
					m_SkeletalAnimFeature = new SBatchSkeletalAnimFeatureDesc(*skeletonAnimDesc);
				else
					m_SkeletalAnimFeature = null;
			}

			unsafe
			{
				// The atlas is null when the effect is imported at first
				// This texture is only setup at runtime when the effect is preloaded
				if (desc.m_TextureAtlasCount == 0 || desc.m_TextureAtlas == null)
				{
					m_AtlasSubRects = new Texture2D(1, 1, TextureFormat.RGBAFloat, false, true);
					m_AtlasSubRects.SetPixel(0, 0, new Vector4(1, 1, 0, 0));
				}
				else
				{
					m_AtlasSubRects = new Texture2D(desc.m_TextureAtlasCount, 1, TextureFormat.RGBAFloat, false, true);
					Vector4 *subRects = (Vector4*)desc.m_TextureAtlas.ToPointer();
					for (int i = 0; i < desc.m_TextureAtlasCount; ++i)
					{
						Vector4 curCol = subRects[i];
						curCol.w = 1 - curCol.y - curCol.w; // Unity sampling is reversed in Y.
						m_AtlasSubRects.SetPixel(i, 0, curCol);
					}
				}
				m_AtlasSubRects.Apply();
			}

			m_GeneratedName = GenerateNameFromDescription();
		}

		public SBatchDesc(SDecalRendererDesc desc, Material mat = null, GameObject renderingObject = null)
		{
			m_Type = ERendererType.Decal;
			m_BlendMode = EBlendMode.AlphaBlend;
			m_ShaderVariationFlags = desc.m_ShaderVariationFlags;
			m_IsLegacy = true; // Unity only support legacy decal for now

			string diffuseStr = null;
			string emissiveStr = null;

			if (desc.m_DiffuseMap != IntPtr.Zero)
				diffuseStr = Marshal.PtrToStringAnsi(desc.m_DiffuseMap);	
			if (desc.m_EmissiveMap != IntPtr.Zero)
				emissiveStr = Marshal.PtrToStringAnsi(desc.m_EmissiveMap);

			m_DiffuseMap = diffuseStr;
			m_EmissiveMap = emissiveStr;

			m_DiffuseColor = desc.m_DiffuseColor; 
            m_EmissiveColor = desc.m_EmissiveColor;

            unsafe
            {
                // The atlas is null when the effect is imported at first
                // This texture is only setup at runtime when the effect is preloaded
                if (desc.m_TextureAtlasCount == 0 || desc.m_TextureAtlas == null)
                {
                    m_AtlasSubRects = new Texture2D(1, 1, TextureFormat.RGBAFloat, false, true);
                    m_AtlasSubRects.SetPixel(0, 0, new Vector4(1, 1, 0, 0));
                }
                else
                {
					m_AtlasSubRects = new Texture2D(desc.m_TextureAtlasCount, 1, TextureFormat.RGBAFloat, false, true);

					Vector4* subRects = (Vector4*)desc.m_TextureAtlas.ToPointer();
					for (int i = 0; i < desc.m_TextureAtlasCount; ++i)
					{
						Vector4 curCol = subRects[i];
						curCol.w = 1 - curCol.y - curCol.w; // Unity sampling is reversed in Y. 
						m_AtlasSubRects.SetPixel(i, 0, curCol);
					}
                }
                m_AtlasSubRects.Apply();
            }
			
            m_UID = desc.m_UID;
			m_GeneratedName = GenerateNameFromDescription();
        }

        public bool HasShaderVariationFlag(EShaderVariationFlags flag)
		{
			return (m_ShaderVariationFlags & (int)flag) == (int)flag;
		}

		// All of those functions are used to generate a description for the renderer:
		public string MaterialFlagsToString(int materialFlags)
		{
			string finalName = "";

            if ((materialFlags & (int)EShaderVariationFlags.Has_RibbonComplex) != 0)
				finalName += " RibbonComplex";
			if ((materialFlags & (int)EShaderVariationFlags.Has_TransformUVs) != 0)
				finalName += " TransformUVs";
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
			if ((materialFlags & (int)EShaderVariationFlags.Has_FluidVAT) != 0)
				finalName += " FluidVAT";
			if ((materialFlags & (int)EShaderVariationFlags.Has_SoftVAT) != 0)
				finalName += " SoftVAT";
			if ((materialFlags & (int)EShaderVariationFlags.Has_RigidVAT) != 0)
				finalName += " RigidVAT";
            if ((materialFlags & (int)EShaderVariationFlags.Has_LightingLegacy) != 0)
                finalName += " LightingLegacy";
            if ((materialFlags & (int)EShaderVariationFlags.Has_Size2) != 0)
                finalName += " Size2";
            if ((materialFlags & (int)EShaderVariationFlags.Has_Emissive) != 0)
                finalName += " Emissive";
            if ((materialFlags & (int)EShaderVariationFlags.Has_EmissiveRamp) != 0)
                finalName += " EmissiveRamp";
            if ((materialFlags & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
                finalName += " SkeletalAnim";
            if ((materialFlags & (int)EShaderVariationFlags.Has_SkeletalInterpol) != 0)
                finalName += " SkeletalInterpol";
            if ((materialFlags & (int)EShaderVariationFlags.Has_SkeletalTrackInterpol) != 0)
                finalName += " SkeletalTrackInterpol";

            return finalName.Length == 0 ? " MaterialBasic" : finalName;
		}

		public string BlendModeToString(EBlendMode blendMode)
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
				finalName += " Masked" + m_AlphaClipThreshold;
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
            else if (m_Type == ERendererType.Decal)
                finalName = "Decal";
            else
				finalName = "Unknown";
            finalName += " ";
            finalName += m_IsLegacy ? "Legacy" : "";
			finalName += " ";
			finalName += MaterialFlagsToString(m_ShaderVariationFlags);
			finalName += " ";
			finalName += BlendModeToString(m_BlendMode);
			finalName += " ";
			finalName += m_RotateUVs ? "RotateUvs" : "UVs";
			finalName += " ";
			finalName += m_TransformUVs_RGBOnly ? "RGBOnly" : "";
			finalName += " ";
			finalName += m_DiffuseMap == null ? "(none)" : m_DiffuseMap;
			finalName += " ";
			finalName += m_EmissiveMap == null ? "(none)" : m_EmissiveMap;
			finalName += " ";
            finalName += (m_LitFeature == null || !m_LitFeature.m_Activated) ? "(none)" : m_LitFeature.GetGeneratedName();
			finalName += " ";
			finalName += (m_VatFeature == null || !m_VatFeature.m_Activated) ? "(none)" : m_VatFeature.GetGeneratedName();
			finalName += " ";
            finalName += m_AlphaMasksFeature.GetGeneratedName();
			finalName += (m_SkeletalAnimFeature == null || !m_SkeletalAnimFeature.m_Activated) ? "(none)" : m_SkeletalAnimFeature.GetGeneratedName();
			if (m_Type != ERendererType.Mesh)
			{
				finalName += " ";
				finalName += m_AlphaRemap == null ? "(none)" : m_AlphaRemap;
				finalName += " ";
				finalName += m_DiffuseRampMap == null ? "(none)" : m_DiffuseRampMap;
				finalName += " ";
				finalName += m_EmissiveRampMap == null ? "(none)" : m_EmissiveRampMap;
				finalName += " ";
				finalName += m_InvSoftnessDistance;
			}
			else
			{
				finalName += " ";
				finalName += m_SpecularMap == null ? "(none)" : m_SpecularMap;
				finalName += " ";
				if (m_UseVertexColor)
					finalName += " VertexColor";
			}
			finalName += " " + m_DrawOrder;
			return finalName;
		}
		public string BlendModeToShortString(EBlendMode blendMode)
		{
			string finalName = "";
			if (blendMode == EBlendMode.Additive)
				finalName += "Add";
			if (blendMode == EBlendMode.AdditiveNoAlpha)
				finalName += "AddNA";
			if (blendMode == EBlendMode.AlphaBlend)
				finalName += "AB";
			if (blendMode == EBlendMode.PremultipliedAlpha)
				finalName += "PreA";
			if (blendMode == EBlendMode.Solid)
				finalName += "So";
			if (blendMode == EBlendMode.Masked)
				finalName += "M" + m_AlphaClipThreshold;
			finalName += "_";
			return finalName;
		}


		// All of those functions are used to generate a description for the renderer:
		public static string MaterialFlagsToShortString(int materialFlags)
		{
			string finalName = "";

			if ((materialFlags & (int)EShaderVariationFlags.Has_RibbonComplex) != 0)
				finalName += "RC";
			if ((materialFlags & (int)EShaderVariationFlags.Has_Atlas) != 0)
				finalName += "TUV";
			if ((materialFlags & (int)EShaderVariationFlags.Has_Atlas) != 0)
				finalName += "At";
			if ((materialFlags & (int)EShaderVariationFlags.Has_AnimBlend) != 0)
				finalName += "AB";
			if ((materialFlags & (int)EShaderVariationFlags.Has_AlphaRemap) != 0)
				finalName += "AR";
			if ((materialFlags & (int)EShaderVariationFlags.Has_DiffuseRamp) != 0)
				finalName += "DR";
			if ((materialFlags & (int)EShaderVariationFlags.Has_Lighting) != 0)
				finalName += "L";
			if ((materialFlags & (int)EShaderVariationFlags.Has_Soft) != 0)
				finalName += "S";
			if ((materialFlags & (int)EShaderVariationFlags.Has_DistortionMap) != 0)
				finalName += "D";
			if ((materialFlags & (int)EShaderVariationFlags.Has_Color) != 0)
				finalName += "C";
			if ((materialFlags & (int)EShaderVariationFlags.Has_DiffuseMap) != 0)
				finalName += "Di";
			if ((materialFlags & (int)EShaderVariationFlags.Has_DoubleSided) != 0)
				finalName += "DS";
			if ((materialFlags & (int)EShaderVariationFlags.Has_CastShadow) != 0)
				finalName += "CS";
			if ((materialFlags & (int)EShaderVariationFlags.Has_FluidVAT) != 0)
				finalName += "FVAT";
			if ((materialFlags & (int)EShaderVariationFlags.Has_SoftVAT) != 0)
				finalName += "SVAT";
			if ((materialFlags & (int)EShaderVariationFlags.Has_RigidVAT) != 0)
				finalName += "RVAT";
			return finalName.Length == 0 ? "U_" : finalName + "_";
		}

		public string GenerateShortNameFromDescription()
		{
			string finalName;

			if (m_Type == ERendererType.Billboard)
				finalName = "B";
			else if (m_Type == ERendererType.Ribbon)
				finalName = "R";
			else if (m_Type == ERendererType.Mesh)
				finalName = "M";
			else if (m_Type == ERendererType.Triangle)
				finalName = "T";
			else
				finalName = "U";
			finalName += "_";
            finalName += m_IsLegacy ? "L" : "";
            finalName += MaterialFlagsToShortString(m_ShaderVariationFlags);
			finalName += BlendModeToShortString(m_BlendMode);
			finalName += m_RotateUVs ? "RUV_" : "UV_";
			finalName += (m_DiffuseMap == null || m_DiffuseMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_DiffuseMap) + "_";
			finalName += (m_EmissiveMap == null || m_EmissiveMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_EmissiveMap) + "_";
            finalName += (m_LitFeature == null || !m_LitFeature.m_Activated) ? "U_" : m_LitFeature.GetGeneratedShortName();
			finalName += (m_VatFeature == null || !m_VatFeature.m_Activated) ? "U_" : m_VatFeature.GetGeneratedShortName();
			finalName += (m_SkeletalAnimFeature == null || !m_SkeletalAnimFeature.m_Activated) ? "U_" : m_SkeletalAnimFeature.GetGeneratedShortName();
            finalName += (m_AlphaMasksFeature == null || !m_AlphaMasksFeature.m_Activated) ? "U_" : m_AlphaMasksFeature.GetGeneratedShortName();
			if (m_Type != ERendererType.Mesh)
			{
				finalName += (m_AlphaRemap == null || m_AlphaRemap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_AlphaRemap) + "_";
				finalName += (m_DiffuseRampMap == null || m_DiffuseRampMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_DiffuseRampMap) + "_";
				finalName += (m_EmissiveRampMap == null || m_EmissiveRampMap.Length == 0) ? "U_" : Path.GetFileNameWithoutExtension(m_EmissiveRampMap) + "_";
				finalName += String.Format("{0:0.##}", m_InvSoftnessDistance);
			}
			else
			{
				finalName += m_SpecularMap == null ? "U_" : Path.GetFileName(m_SpecularMap) + "_";
				if (m_UseVertexColor)
					finalName += " VC";
			}
			return finalName;
		}
	}

	public class SMeshDesc
	{
		public MeshFilter m_Slice;

		public int m_VertexCount;
		public int m_IndexCount;

		public PKFxProceduralRenderer m_Procedural;
		public Material m_Material;
		public GameObject m_RenderingObject;
		public SBatchDesc m_BatchDesc;
		public PKFxMeshInstancesRenderer m_InstancesRenderer;
#if UNITY_EDITOR
		public Color m_BoundsDebugColor;
#endif

		private ComputeBuffer m_AtlasInfo = null;
		public IntPtr	GetNativeRawAtlasesBuffer() { return m_AtlasInfo == null ? IntPtr.Zero : m_AtlasInfo.GetNativeBufferPtr(); }
		public int		NativeRawAtlasesBufferSize { get { return m_AtlasInfo == null ? 0 : 8208; } } /*513 * 16*/

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
			Init();
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
			Init();
		}

		~SMeshDesc()
		{
			Clean();
		}

		public bool HasShaderVariationFlag(EShaderVariationFlags flag)
		{
			return (m_BatchDesc.m_ShaderVariationFlags & (int)flag) == (int)flag;
		}

		public void Clean()
		{
			if (m_AtlasInfo != null)
			{
				m_AtlasInfo.Release();
				m_AtlasInfo = null;
			}
		}

		public void Init()
		{
			Clean();

			if (HasShaderVariationFlag(EShaderVariationFlags.Has_Atlas) && HasShaderVariationFlag(EShaderVariationFlags.Has_TransformUVs))
				m_AtlasInfo = new ComputeBuffer(513, 16, ComputeBufferType.Raw);


			m_Material.SetBuffer("_Atlas", m_AtlasInfo);
		}

#if UNITY_EDITOR
		public void ResetComputeBuffer()
		{
			m_Material.SetBuffer("_Atlas", m_AtlasInfo);
		}
#endif

	}

#if UNITY_EDITOR
	// #13837: We have to reset the compute buffers when the shader is reloaded
	// See: https://discussions.unity.com/t/shader-stops-rendering-when-assets-are-updated/872071
	[InitializeOnLoad]
	public class PKFxMeshDescComputeBufferResetter : AssetPostprocessor
	{
		static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
		{
			if (PKFxManager.RenderingPlugin != null)
				PKFxManager.RenderingPlugin.ResetComputeBuffers();
		}
	}

#endif
}
