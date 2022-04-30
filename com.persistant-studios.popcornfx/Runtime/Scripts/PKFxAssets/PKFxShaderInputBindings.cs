//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PopcornFX
{
	public class PKFxShaderInputBindings : ScriptableObject
	{
		// --------------------------------------
		// Blend mode:
		// --------------------------------------
		public string			m_SourceBlendPropertyName = "_SrcBlendMode";
		public string			m_DestinationBlendPropertyName = "_DstBlendMode";
		public string			m_MaskThresholdPropertyName = "_CutOutAlpha";
		// --------------------------------------
		// Basic particles render features:
		// --------------------------------------
		public string			m_RotateUVsPropertyName = "_RotateUVs";
		public string			m_DiffuseMapPropertyName = "_MainTex";
		public string			m_DistortionMapPropertyName = "_MainTex";
		public string			m_EmissiveMapPropertyName = "_EmissiveMap";
		public string			m_EmissiveRampMapPropertyName = "_EmissiveRampMap";
		public string			m_AlphaRemapPropertyName = "_AlphaMap";
		public string			m_DiffuseRampMapPropertyName = "_DiffuseRampMap";
		public string			m_InvSoftnessDistancePropertyName = "_InvSoftnessDistance";
		// --------------------------------------
		// Lit render feature:
		// --------------------------------------
		public string			m_NormalMapPropertyName = "_NormalMap";
		public string			m_RoughMetalMapPropertyName = "";
		public string			m_RoughnessPropertyName = "_Roughness";
		public string			m_MetalnessPropertyName = "_Metalness";
		// --------------------------------------
		// Mesh renderer:
		// --------------------------------------
		public string 			m_MeshDiffuseColorPropertyName = "_BaseColor";
		public string 			m_MeshEmissiveColorPropertyName = "_EmissiveColor";
		public string 			m_MeshAlphaCursorPropertyName = "_AlphaCursor";
		// --------------------------------------
		// VAT render feature:
		// --------------------------------------
		public string			m_MeshVATCursorPropertyName = "_VATCursor";
		public string 			m_VATPositionMapPropertyName = "_VATPositionMap";
		public string 			m_VATNormalMapPropertyName = "_VATNormalMap";
		public string 			m_VATColorMapPropertyName = "_VATColorMap";
		public string 			m_VATRotationMapPropertyName = "_VATRotationMap";
		public string 			m_VATNumFramesPropertyName = "_VATNumFrames";
		public string 			m_VATPackedDataPropertyName = "_VATPackedData";
		public string 			m_VATColorPropertyName = "";
		public string 			m_VATBoundsPivotPropertyName = "_VATPivot";
		public string 			m_VATNormalizedDataPropertyName = "_VATNormalizedData";
		public string 			m_VATBoundsPositionPropertyName = "_VATBounds";
		public string 			m_VATPadToPowerOf2PropertyName = "_VATPadToPowerOf2";
		public string 			m_VATPaddedRatioPropertyName = "_VATPaddedRatio";

		public void BindMaterialProperties(SBatchDesc batchDesc, Material material, PKFxEffectAsset asset)
		{
			// Set the blend mode:
			int srcMode = 0;
			int dstMode = 0;
			// Additive and distortion
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap) ||
				batchDesc.m_BlendMode == EBlendMode.Additive)
			{
				srcMode = (int)UnityEngine.Rendering.BlendMode.SrcAlpha;
				dstMode = (int)UnityEngine.Rendering.BlendMode.One;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.AdditiveNoAlpha)
			{
				srcMode = (int)UnityEngine.Rendering.BlendMode.One;
				dstMode = (int)UnityEngine.Rendering.BlendMode.One;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.PremultipliedAlpha)
			{
				srcMode = (int)UnityEngine.Rendering.BlendMode.One;
				dstMode = (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.AlphaBlend)
			{
				srcMode = (int)UnityEngine.Rendering.BlendMode.SrcAlpha;
				dstMode = (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha;
			}
			else
			{
				srcMode = (int)UnityEngine.Rendering.BlendMode.One;
				dstMode = (int)UnityEngine.Rendering.BlendMode.One;
			}

			if (!string.IsNullOrEmpty(m_SourceBlendPropertyName))
			{
				material.SetInt(m_SourceBlendPropertyName, srcMode);
			}
			if (!string.IsNullOrEmpty(m_DestinationBlendPropertyName))
			{
				material.SetInt(m_DestinationBlendPropertyName, dstMode);
			}
			if (!string.IsNullOrEmpty(m_MaskThresholdPropertyName))
			{
				if (batchDesc.m_BlendMode == EBlendMode.Masked)
				{
					material.SetFloat(m_MaskThresholdPropertyName, batchDesc.m_AlphaClipThreshold);
				}
				else
				{
					material.SetFloat(m_MaskThresholdPropertyName, 0.0f);
				}
			}
			TextureWrapMode wrapMode = batchDesc.m_Type == ERendererType.Ribbon ? TextureWrapMode.Repeat : TextureWrapMode.Clamp;
			if (!string.IsNullOrEmpty(m_RotateUVsPropertyName))
			{
				material.SetInt(m_RotateUVsPropertyName, Convert.ToInt32(batchDesc.m_RotateUVs));
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap) &&
				!string.IsNullOrEmpty(m_DistortionMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_DiffuseMap))
			{
				Texture distortionMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_DiffuseMap, true, wrapMode);
				material.SetTexture(m_DistortionMapPropertyName, distortionMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DiffuseMap) &&
				!string.IsNullOrEmpty(m_DiffuseMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_DiffuseMap))
			{
				Texture diffuseMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_DiffuseMap, false, wrapMode);
				material.SetTexture(m_DiffuseMapPropertyName, diffuseMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Emissive) &&
				!string.IsNullOrEmpty(m_EmissiveMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_EmissiveMap))
			{
				Texture emissiveMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_EmissiveMap, false, wrapMode);
				material.SetTexture(m_EmissiveMapPropertyName, emissiveMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_EmissiveRamp) &&
				!string.IsNullOrEmpty(m_EmissiveRampMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_EmissiveRampMap))
			{
				Texture emissiveRampMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_EmissiveRampMap, false, TextureWrapMode.Clamp);
				material.SetTexture(m_EmissiveRampMapPropertyName, emissiveRampMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaRemap) &&
				!string.IsNullOrEmpty(m_AlphaRemapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_AlphaRemap))
			{
				Texture alphaRemap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_AlphaRemap, false, TextureWrapMode.Clamp);
				material.SetTexture(m_AlphaRemapPropertyName, alphaRemap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DiffuseRamp) &&
				!string.IsNullOrEmpty(m_DiffuseRampMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_DiffuseRampMap))
			{
				Texture diffuseRampMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_DiffuseRampMap, false, TextureWrapMode.Clamp);
				material.SetTexture(m_DiffuseRampMapPropertyName, diffuseRampMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Soft) &&
				!string.IsNullOrEmpty(m_InvSoftnessDistancePropertyName))
			{
				material.SetFloat(m_InvSoftnessDistancePropertyName, batchDesc.m_InvSoftnessDistance);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Lighting) && batchDesc.m_LitFeature != null)
			{
				if (!string.IsNullOrEmpty(m_NormalMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_LitFeature.m_NormalMap))
				{
					Texture normalMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_LitFeature.m_NormalMap, true, wrapMode);
					material.SetTexture(m_NormalMapPropertyName, normalMap);
				}
				if (!string.IsNullOrEmpty(m_RoughMetalMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_LitFeature.m_RoughMetalMap))
				{
					Texture roughMetalMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_LitFeature.m_RoughMetalMap, true, wrapMode);
					material.SetTexture(m_RoughMetalMapPropertyName, roughMetalMap);
				}
				if (!string.IsNullOrEmpty(m_RoughnessPropertyName))
				{
					material.SetFloat(m_RoughnessPropertyName, batchDesc.m_LitFeature.m_Roughness);
				}
				if (!string.IsNullOrEmpty(m_MetalnessPropertyName))
				{
					material.SetFloat(m_MetalnessPropertyName, batchDesc.m_LitFeature.m_Metalness);
				}
			}
			if (batchDesc.m_Type == ERendererType.Mesh)
			{
				bool hasFluidVAT = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_FluidVAT);
				bool hasSoftVAT = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SoftVAT);
				bool hasRigidVAT = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_RigidVAT);
				bool hasVAT = (hasFluidVAT || hasSoftVAT || hasRigidVAT) && batchDesc.m_VatFeature != null;

				if (hasVAT)
				{
					if (!string.IsNullOrEmpty(m_VATPositionMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_PositionMap))
					{
						Texture positionMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_VatFeature.m_PositionMap, false, TextureWrapMode.Clamp);
						material.SetTexture(m_VATPositionMapPropertyName, positionMap);
					}
					if (!string.IsNullOrEmpty(m_VATNormalMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_NormalMap))
					{
						Texture normalMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_VatFeature.m_NormalMap, false, TextureWrapMode.Clamp);
						material.SetTexture(m_VATNormalMapPropertyName, normalMap);
					}
					if (!string.IsNullOrEmpty(m_VATRotationMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_RotationMap))
					{
						Texture rotationMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_VatFeature.m_RotationMap, false, TextureWrapMode.Clamp);
						material.SetTexture(m_VATRotationMapPropertyName, rotationMap);
					}
					if (!string.IsNullOrEmpty(m_VATColorMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_ColorMap))
					{
						Texture colorMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_VatFeature.m_ColorMap, false, TextureWrapMode.Clamp);
						material.SetTexture(m_VATColorMapPropertyName, colorMap);
					}

					if (!string.IsNullOrEmpty(m_VATNumFramesPropertyName))
					{
						material.SetInt(m_VATNumFramesPropertyName, batchDesc.m_VatFeature.m_NumFrames);
					}
					if (!string.IsNullOrEmpty(m_VATPackedDataPropertyName))
					{
						material.SetInt(m_VATPackedDataPropertyName, Convert.ToInt32(batchDesc.m_VatFeature.m_PackedData));
					}
					if (hasRigidVAT && !string.IsNullOrEmpty(m_VATBoundsPivotPropertyName))
					{
						material.SetVector(m_VATBoundsPivotPropertyName, batchDesc.m_VatFeature.m_BoundsPivot);
					}
					if (!string.IsNullOrEmpty(m_VATNormalizedDataPropertyName))
					{
						material.SetInt(m_VATNormalizedDataPropertyName, Convert.ToInt32(batchDesc.m_VatFeature.m_NormalizedData));
					}
					if (!string.IsNullOrEmpty(m_VATBoundsPositionPropertyName))
					{
						material.SetVector(m_VATBoundsPositionPropertyName, batchDesc.m_VatFeature.m_BoundsPosition);
					}
					if (!string.IsNullOrEmpty(m_VATPadToPowerOf2PropertyName))
					{
						material.SetInt(m_VATPadToPowerOf2PropertyName, Convert.ToInt32(batchDesc.m_VatFeature.m_PadToPowerOf2));
					}
					if (batchDesc.m_VatFeature.m_PadToPowerOf2 && !string.IsNullOrEmpty(m_VATPaddedRatioPropertyName))
					{
						material.SetVector(m_VATPaddedRatioPropertyName, batchDesc.m_VatFeature.m_PaddedRatio);
					}
				}
			}
		}

		public void SetMaterialKeywords(SBatchDesc batchDesc, Material material)
		{
			if (batchDesc.m_Type == ERendererType.Mesh)
			{
				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_FluidVAT))
				{
					material.DisableKeyword("PK_HAS_VAT_NONE");
					material.EnableKeyword("PK_HAS_VAT_FLUID");
				}
				else if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_RigidVAT))
				{
					material.DisableKeyword("PK_HAS_VAT_NONE");
					material.EnableKeyword("PK_HAS_VAT_RIGID");
				}
				else if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SoftVAT))
				{
					material.DisableKeyword("PK_HAS_VAT_NONE");
					material.EnableKeyword("PK_HAS_VAT_SOFT");
				}
				else
					material.EnableKeyword("PK_HAS_VAT_NONE");
			}
			// Set the shader variation:
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DiffuseRamp))
				material.EnableKeyword("PK_HAS_DIFFUSE_RAMP");
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaRemap))
				material.EnableKeyword("PK_HAS_ALPHA_REMAP");
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AnimBlend))
				material.EnableKeyword("PK_HAS_ANIM_BLEND");
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
				material.EnableKeyword("PK_HAS_DISTORTION");
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Lighting))
				material.EnableKeyword("PK_HAS_LIGHTING");
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_RibbonComplex))
				material.EnableKeyword("PK_HAS_RIBBON_COMPLEX");
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Emissive))
			{
				material.DisableKeyword("PK_HAS_EMISSIVE_NONE");
				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_EmissiveRamp))
					material.EnableKeyword("PK_HAS_EMISSIVE_WITH_RAMP");
				else
					material.EnableKeyword("PK_HAS_EMISSIVE_BASIC");
			}
			else
			{
				material.EnableKeyword("PK_HAS_EMISSIVE_NONE");
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Soft) && PKFxSettings.EnableSoftParticles)
				material.EnableKeyword("PK_HAS_SOFT");
		}
	}
}