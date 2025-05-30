//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
#if     UNITY_EDITOR
using UnityEditor;
#endif
using UnityEngine.Rendering;

namespace PopcornFX
{
	[Serializable]
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
		public string 			m_MeshCullingModePropertyName = "_Cull";
		// --------------------------------------
		// Decal renderer:
		// --------------------------------------
		public string m_DecalDiffuseColorPropertyName = "_DiffuseColor";
		public string m_DecalEmissiveColorPropertyName = "_EmissiveColor";

        public string m_DecalHasAtlasPropertyName = "_HasAtlas";
        public string m_DecalAtlasIdPropertyName = "_AtlasId";

        public string m_DecalTextureAtlasPropertyName = "_AtlasRects";
        public string m_DecalAtlasCountPropertyName = "_AtlasCount";

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
		// --------------------------------------
		// Skeletal anim render feature:
		// --------------------------------------
		public string 			m_SkeletalAnimTexturePropertyName = "_SkeletalAnimTexture";
		public string 			m_SkeletalAnimTextureResolPropertyName = "_SkeletalAnimTextureResol";
		public string 			m_SkeletalAnimCountPropertyName = "_SkeletalAnimCount";
		public string			m_SkeletalAnimUseBoneScalePropertyName = "_SkeletalAnimUseBoneScale";
		public string			m_SkeletalAnimCursor0PropertyName = "_SkeletalAnimCursor0";
		public string			m_SkeletalAnimIdx0PropertyName = "_SkeletalAnimIdx0";
		public string			m_SkeletalAnimCursor1PropertyName = "_SkeletalAnimCursor1";
		public string			m_SkeletalAnimIdx1PropertyName = "_SkeletalAnimIdx1";
		public string			m_SkeletalAnimTransitionPropertyName = "_SkeletalAnimTransition";
		public string			m_SkeletalAnimTranslationBoundsMinPropertyName = "_SkeletalAnimTranslationBoundsMin";
		public string			m_SkeletalAnimTranslationBoundsMaxPropertyName = "_SkeletalAnimTranslationBoundsMax";
		public string			m_SkeletalAnimScaleBoundsMinPropertyName = "_SkeletalAnimScaleBoundsMin";
		public string			m_SkeletalAnimScaleBoundsMaxPropertyName = "_SkeletalAnimScaleBoundsMax";
		public string			m_MeshTransformRow0PropertyName = "_SkeletalMeshTransform0";
		public string			m_MeshTransformRow1PropertyName = "_SkeletalMeshTransform1";
		public string			m_MeshTransformRow2PropertyName = "_SkeletalMeshTransform2";
		public string			m_MeshTransformRow3PropertyName = "_SkeletalMeshTransform3";
		// Mesh Texture Atlas:
		public string			m_TextureAtlasPropertyName = "_AtlasRects";
		public string 			m_AtlasCountPropertyName = "_AtlasCount";
		public string 			m_AtlasIdPropertyName = "_AtlasId";
		// Transform UVs:
		public string			m_TransformUVsRGBOnlyPropertyName = "_TransformUVs_RGBOnly";
		public string			m_UseVertexColorPropertyName = "_UseVertexColor";
        // AlphaMasks:
        public string			m_AlphaMask1MapPropertyName = "_AlphaMasksTex1";
        public string			m_AlphaMask2MapPropertyName = "_AlphaMasksTex2";
        public string			m_AlphaMask1IntensityPropertyName = "_AlphaMask1Intensity";
		public string			m_AlphaMask2IntensityPropertyName = "_AlphaMask2Intensity";
		public string			m_AlphaMask1WeightPropertyName = "_AlphaMask1Weight";
		public string			m_AlphaMask2WeightPropertyName = "_AlphaMask2Weight";
		public string			m_AlphaMask1ScaleXPropertyName = "_AlphaMask1ScaleX";
		public string			m_AlphaMask1ScaleYPropertyName = "_AlphaMask1ScaleY";
		public string			m_AlphaMask2ScaleXPropertyName = "_AlphaMask2ScaleX";
		public string			m_AlphaMask2ScaleYPropertyName = "_AlphaMask2ScaleY";
		public string			m_AlphaMask1RotationSpeedPropertyName = "_AlphaMask1RotationSpeed";
		public string			m_AlphaMask2RotationSpeedPropertyName = "_AlphaMask2RotationSpeed";
		public string			m_AlphaMask1TranslationSpeedXPropertyName = "_AlphaMask1TranslationSpeedX";
		public string			m_AlphaMask1TranslationSpeedYPropertyName = "_AlphaMask1TranslationSpeedY";
		public string			m_AlphaMask2TranslationSpeedXPropertyName = "_AlphaMask2TranslationSpeedX";
		public string			m_AlphaMask2TranslationSpeedYPropertyName = "_AlphaMask2TranslationSpeedY";
		// UVDistortions:
		public string			m_UVDistortions1MapPropertyName = "_UVDistortionsTex1";
		public string			m_UVDistortions2MapPropertyName = "_UVDistortionsTex2";
        public string			m_UVDistortions1IntensityPropertyName = "_UVDistortions1Intensity";
		public string			m_UVDistortions2IntensityPropertyName = "_UVDistortions2Intensity";
		public string			m_UVDistortions1ScaleXPropertyName = "_UVDistortions1ScaleX";
		public string			m_UVDistortions1ScaleYPropertyName = "_UVDistortions1ScaleY";
		public string			m_UVDistortions2ScaleXPropertyName = "_UVDistortions2ScaleX";
		public string			m_UVDistortions2ScaleYPropertyName = "_UVDistortions2ScaleY";
		public string			m_UVDistortions1RotationSpeedPropertyName = "_UVDistortions1RotationSpeed";
		public string			m_UVDistortions2RotationSpeedPropertyName = "_UVDistortions2RotationSpeed";
		public string			m_UVDistortions1TranslationSpeedXPropertyName = "_UVDistortions1TranslationSpeedX";
		public string			m_UVDistortions1TranslationSpeedYPropertyName = "_UVDistortions1TranslationSpeedY";
		public string			m_UVDistortions2TranslationSpeedXPropertyName = "_UVDistortions2ranslationSpeedX";
		public string			m_UVDistortions2TranslationSpeedYPropertyName = "_UVDistortions2TranslationSpeedY";
		// Dissolve:
		public string			m_DissolveMapPropertyName = "_DissolveTex";
		public string			m_DissolveSoftnessPropertyName = "_DissolveSoftness";

		public enum UnityBlendMode : int // Duplicate of AlphaMode that is EditorOnly from shadergraph...
		{
			Alpha,
			Premultiply,
			Additive,
			Multiply,
		}

		public void BindMaterialProperties(SBatchDesc batchDesc, Material material, PKFxEffectAsset asset, bool logError = true)
		{
			// Set the blend mode:
			int srcMode = 0;
			int dstMode = 0;
			UnityBlendMode blend = UnityBlendMode.Alpha;
			// Additive and distortion
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
			{
				blend = UnityBlendMode.Additive;
				srcMode = (int)UnityEngine.Rendering.BlendMode.SrcAlpha;
				if (PKFxSettings.MaterialFactory.m_FactoryType == PKFxMaterialFactory.FactoryType.URP)
					dstMode = (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha;
				else
					dstMode = (int)UnityEngine.Rendering.BlendMode.One;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.Additive)
			{
				blend = UnityBlendMode.Additive;
				srcMode = (int)UnityEngine.Rendering.BlendMode.SrcAlpha;
				dstMode = (int)UnityEngine.Rendering.BlendMode.One;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.AdditiveNoAlpha)
			{
				blend = UnityBlendMode.Additive;
				srcMode = (int)UnityEngine.Rendering.BlendMode.One;
				dstMode = (int)UnityEngine.Rendering.BlendMode.One;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.PremultipliedAlpha)
			{
				blend = UnityBlendMode.Premultiply;
				srcMode = (int)UnityEngine.Rendering.BlendMode.One;
				dstMode = (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.AlphaBlend)
			{
				blend = UnityBlendMode.Alpha;
				srcMode = (int)UnityEngine.Rendering.BlendMode.SrcAlpha;
				dstMode = (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha;
			}
			else
			{
				blend = UnityBlendMode.Additive;
				srcMode = (int)UnityEngine.Rendering.BlendMode.One;
				dstMode = (int)UnityEngine.Rendering.BlendMode.One;
			}

			if (batchDesc.m_DrawOrder != 0 && PKFxSettings.MaterialFactory.m_FactoryType == PKFxMaterialFactory.FactoryType.URP)
			{
				material.SetFloat("_QueueOffset", batchDesc.m_DrawOrder);
			}
			if (!string.IsNullOrEmpty(m_SourceBlendPropertyName))
			{
				material.SetInt(m_SourceBlendPropertyName, srcMode);
				if (PKFxSettings.MaterialFactory.m_FactoryType == PKFxMaterialFactory.FactoryType.URP)
				{
					material.SetInt("_Blend", (int)blend);
				}
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
				if (logError && distortionMap == null)
				{
					Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_DiffuseMap, asset.AssetVirtualPath));
				}
				material.SetTexture(m_DistortionMapPropertyName, distortionMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DiffuseMap) &&
				!string.IsNullOrEmpty(m_DiffuseMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_DiffuseMap))
			{
				Texture diffuseMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_DiffuseMap, false, wrapMode);
				if (logError && diffuseMap == null)
				{
					Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_DiffuseMap, asset.AssetVirtualPath));
				}
				material.SetTexture(m_DiffuseMapPropertyName, diffuseMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Emissive) &&
				!string.IsNullOrEmpty(m_EmissiveMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_EmissiveMap))
			{
				Texture emissiveMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_EmissiveMap, false, wrapMode);
				if (logError && emissiveMap == null)
				{
					Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_EmissiveMap, asset.AssetVirtualPath));
				}
				material.SetTexture(m_EmissiveMapPropertyName, emissiveMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_EmissiveRamp) &&
				!string.IsNullOrEmpty(m_EmissiveRampMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_EmissiveRampMap))
			{
				Texture emissiveRampMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_EmissiveRampMap, false, TextureWrapMode.Clamp);
				if (logError && emissiveRampMap == null)
				{
					Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_EmissiveRampMap, asset.AssetVirtualPath));
				}
				material.SetTexture(m_EmissiveRampMapPropertyName, emissiveRampMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaRemap) &&
				!string.IsNullOrEmpty(m_AlphaRemapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_AlphaRemap))
			{
				Texture alphaRemap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_AlphaRemap, true, TextureWrapMode.Clamp);
				if (logError && alphaRemap == null)
				{
					Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_AlphaRemap, asset.AssetVirtualPath));
				}
				material.SetTexture(m_AlphaRemapPropertyName, alphaRemap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DiffuseRamp) &&
				!string.IsNullOrEmpty(m_DiffuseRampMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_DiffuseRampMap))
			{
				Texture diffuseRampMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_DiffuseRampMap, false, TextureWrapMode.Clamp);
				if (logError && diffuseRampMap == null)
				{
					Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_DiffuseRampMap, asset.AssetVirtualPath));
				}
				material.SetTexture(m_DiffuseRampMapPropertyName, diffuseRampMap);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Soft) &&
				!string.IsNullOrEmpty(m_InvSoftnessDistancePropertyName))
			{
				material.SetFloat(m_InvSoftnessDistancePropertyName, batchDesc.m_InvSoftnessDistance);
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_TransformUVs) &&
				!string.IsNullOrEmpty(m_TransformUVsRGBOnlyPropertyName))
			{
				material.SetInt(m_TransformUVsRGBOnlyPropertyName, Convert.ToInt32(batchDesc.m_TransformUVs_RGBOnly));
			}
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Lighting) && (batchDesc.m_LitFeature != null && batchDesc.m_LitFeature.m_Activated))
			{
				if (!string.IsNullOrEmpty(m_NormalMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_LitFeature.m_NormalMap))
				{
					Texture normalMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_LitFeature.m_NormalMap, true, wrapMode);
					if (logError && normalMap == null)
					{
						Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_LitFeature.m_NormalMap, asset.AssetVirtualPath));
					}
					material.SetTexture(m_NormalMapPropertyName, normalMap);
				}
				if (!string.IsNullOrEmpty(m_RoughMetalMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_LitFeature.m_RoughMetalMap))
				{
					Texture roughMetalMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_LitFeature.m_RoughMetalMap, true, wrapMode);
					if (logError && roughMetalMap == null)
					{
						Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_LitFeature.m_RoughMetalMap, asset.AssetVirtualPath));
					}
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
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaMasks))
			{
				if (!string.IsNullOrEmpty(m_AlphaMask1MapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_AlphaMasksFeature.m_AlphaMasks1Map))
				{
					Texture alphaMask1 = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_AlphaMasksFeature.m_AlphaMasks1Map, true, TextureWrapMode.Clamp);
					if (logError && alphaMask1 == null)
					{
						Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_AlphaMasksFeature.m_AlphaMasks1Map, asset.AssetVirtualPath));
					}
					material.SetTexture(m_AlphaMask1MapPropertyName, alphaMask1);
				}

				if (!string.IsNullOrEmpty(m_AlphaMask2MapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_AlphaMasksFeature.m_AlphaMasks2Map))
				{
					Texture alphaMask2 = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_AlphaMasksFeature.m_AlphaMasks2Map, true, TextureWrapMode.Clamp);
					if (logError && alphaMask2 == null)
					{
						Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_AlphaMasksFeature.m_AlphaMasks2Map, asset.AssetVirtualPath));
					}
					material.SetTexture(m_AlphaMask2MapPropertyName, alphaMask2);
				}

				if (!string.IsNullOrEmpty(m_AlphaMask1IntensityPropertyName))
					material.SetFloat(m_AlphaMask1IntensityPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask1Intensity);

				if (!string.IsNullOrEmpty(m_AlphaMask2IntensityPropertyName))
					material.SetFloat(m_AlphaMask2IntensityPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask2Intensity);

				if (!string.IsNullOrEmpty(m_AlphaMask1WeightPropertyName))
					material.SetFloat(m_AlphaMask1WeightPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask1Weight);

				if (!string.IsNullOrEmpty(m_AlphaMask2WeightPropertyName))
					material.SetFloat(m_AlphaMask2WeightPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask2Weight);

				if (!string.IsNullOrEmpty(m_AlphaMask1RotationSpeedPropertyName))
					material.SetFloat(m_AlphaMask1RotationSpeedPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask1RotationSpeed);

				if (!string.IsNullOrEmpty(m_AlphaMask2RotationSpeedPropertyName))
					material.SetFloat(m_AlphaMask2RotationSpeedPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask2RotationSpeed);

				if (!string.IsNullOrEmpty(m_AlphaMask1ScaleXPropertyName))
					material.SetFloat(m_AlphaMask1ScaleXPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask1Scale.x);

				if (!string.IsNullOrEmpty(m_AlphaMask1ScaleYPropertyName))
					material.SetFloat(m_AlphaMask1ScaleYPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask1Scale.y);

				if (!string.IsNullOrEmpty(m_AlphaMask2ScaleXPropertyName))
					material.SetFloat(m_AlphaMask2ScaleXPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask2Scale.x);

				if (!string.IsNullOrEmpty(m_AlphaMask2ScaleYPropertyName))
					material.SetFloat(m_AlphaMask2ScaleYPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask2Scale.y);

				if (!string.IsNullOrEmpty(m_AlphaMask1TranslationSpeedXPropertyName))
					material.SetFloat(m_AlphaMask1TranslationSpeedXPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask1TranslationSpeed.x);

				if (!string.IsNullOrEmpty(m_AlphaMask1TranslationSpeedYPropertyName))
					material.SetFloat(m_AlphaMask1TranslationSpeedYPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask1TranslationSpeed.y);

				if (!string.IsNullOrEmpty(m_AlphaMask2TranslationSpeedXPropertyName))
					material.SetFloat(m_AlphaMask2TranslationSpeedXPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask2TranslationSpeed.x);

				if (!string.IsNullOrEmpty(m_AlphaMask2TranslationSpeedYPropertyName))
					material.SetFloat(m_AlphaMask2TranslationSpeedYPropertyName, batchDesc.m_AlphaMasksFeature.m_AlphaMask2TranslationSpeed.y);
			}

			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_UVDistortions))
			{
				if (!string.IsNullOrEmpty(m_UVDistortions1MapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_UVDistortionsFeature.m_UVDistortions1Map))
				{
					Texture uvDistortions1Map = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_UVDistortionsFeature.m_UVDistortions1Map, true, TextureWrapMode.Clamp);
					if (logError && uvDistortions1Map == null)
					{
						Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_UVDistortionsFeature.m_UVDistortions1Map, asset.AssetVirtualPath));
					}
					material.SetTexture(m_UVDistortions1MapPropertyName, uvDistortions1Map);
				}

				if (!string.IsNullOrEmpty(m_UVDistortions2MapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_UVDistortionsFeature.m_UVDistortions2Map))
				{
					Texture uvDistortions2Map = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_UVDistortionsFeature.m_UVDistortions2Map, true, TextureWrapMode.Clamp);
					if (logError && uvDistortions2Map == null)
					{
						Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_UVDistortionsFeature.m_UVDistortions2Map, asset.AssetVirtualPath));
					}
					material.SetTexture(m_UVDistortions2MapPropertyName, uvDistortions2Map);
				}

				if (!string.IsNullOrEmpty(m_UVDistortions1IntensityPropertyName))
					material.SetFloat(m_UVDistortions1IntensityPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions1Intensity);

				if (!string.IsNullOrEmpty(m_UVDistortions2IntensityPropertyName))
					material.SetFloat(m_UVDistortions2IntensityPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions2Intensity);

				if (!string.IsNullOrEmpty(m_UVDistortions1RotationSpeedPropertyName))
					material.SetFloat(m_UVDistortions1RotationSpeedPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions1RotationSpeed);

				if (!string.IsNullOrEmpty(m_UVDistortions2RotationSpeedPropertyName))
					material.SetFloat(m_UVDistortions2RotationSpeedPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions2RotationSpeed);

				if (!string.IsNullOrEmpty(m_UVDistortions1ScaleXPropertyName))
					material.SetFloat(m_UVDistortions1ScaleXPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions1Scale.x);

				if (!string.IsNullOrEmpty(m_UVDistortions1ScaleYPropertyName))
					material.SetFloat(m_UVDistortions1ScaleYPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions1Scale.y);

				if (!string.IsNullOrEmpty(m_UVDistortions2ScaleXPropertyName))
					material.SetFloat(m_UVDistortions2ScaleXPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions2Scale.x);

				if (!string.IsNullOrEmpty(m_UVDistortions2ScaleYPropertyName))
					material.SetFloat(m_UVDistortions2ScaleYPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions2Scale.y);

				if (!string.IsNullOrEmpty(m_UVDistortions1TranslationSpeedXPropertyName))
					material.SetFloat(m_UVDistortions1TranslationSpeedXPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions1TranslationSpeed.x);

				if (!string.IsNullOrEmpty(m_UVDistortions1TranslationSpeedYPropertyName))
					material.SetFloat(m_UVDistortions1TranslationSpeedYPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions1TranslationSpeed.y);

				if (!string.IsNullOrEmpty(m_UVDistortions2TranslationSpeedXPropertyName))
					material.SetFloat(m_UVDistortions2TranslationSpeedXPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions2TranslationSpeed.x);

				if (!string.IsNullOrEmpty(m_UVDistortions2TranslationSpeedYPropertyName))
					material.SetFloat(m_UVDistortions2TranslationSpeedYPropertyName, batchDesc.m_UVDistortionsFeature.m_UVDistortions2TranslationSpeed.y);
			}

			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Dissolve))
			{
				if (!string.IsNullOrEmpty(m_DissolveMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_DissolveFeature.m_DissolveMap))
				{
					Texture dissolveMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_DissolveFeature.m_DissolveMap, true, TextureWrapMode.Clamp);
					if (logError && dissolveMap == null)
					{
						Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_DissolveFeature.m_DissolveMap, asset.AssetVirtualPath));
					}
					material.SetTexture(m_DissolveMapPropertyName, dissolveMap);
				}

				if (!string.IsNullOrEmpty(m_DissolveSoftnessPropertyName))
					material.SetFloat(m_DissolveSoftnessPropertyName, batchDesc.m_DissolveFeature.m_DissolveSoftness);
			}

			if (batchDesc.m_Type == ERendererType.Mesh)
			{
				bool hasFluidVAT = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_FluidVAT);
				bool hasSoftVAT = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SoftVAT);
				bool hasRigidVAT = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_RigidVAT);
				bool hasVAT = (hasFluidVAT || hasSoftVAT || hasRigidVAT) && (batchDesc.m_VatFeature != null && batchDesc.m_VatFeature.m_Activated);
				bool hasSkeletalAnim = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SkeletalAnim);
				bool isDoubleSided = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DoubleSided);

				if (!string.IsNullOrEmpty(m_MeshCullingModePropertyName))
				{
					if (isDoubleSided)
						material.SetInt(m_MeshCullingModePropertyName, (int)UnityEngine.Rendering.CullMode.Off);
					else
						material.SetInt(m_MeshCullingModePropertyName, (int)UnityEngine.Rendering.CullMode.Back);
				}

				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Lighting) && (batchDesc.m_LitFeature != null && batchDesc.m_LitFeature.m_Activated))
					material.doubleSidedGI = isDoubleSided;

				if (!string.IsNullOrEmpty(m_UseVertexColorPropertyName))
				{
					material.SetInt(m_UseVertexColorPropertyName, Convert.ToInt32(batchDesc.m_UseVertexColor));
				}
				if (hasVAT)
				{
					if (!string.IsNullOrEmpty(m_VATPositionMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_PositionMap))
					{
						Texture positionMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_VatFeature.m_PositionMap, false, TextureWrapMode.Clamp);
						if (logError && positionMap == null)
						{
							Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_VatFeature.m_PositionMap, asset.AssetVirtualPath));
						}
						material.SetTexture(m_VATPositionMapPropertyName, positionMap);
					}
					if (!string.IsNullOrEmpty(m_VATNormalMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_NormalMap))
					{
						Texture normalMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_VatFeature.m_NormalMap, false, TextureWrapMode.Clamp);
						if (logError && normalMap == null)
						{
							Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_VatFeature.m_NormalMap, asset.AssetVirtualPath));
						}
						material.SetTexture(m_VATNormalMapPropertyName, normalMap);
					}
					if (!string.IsNullOrEmpty(m_VATRotationMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_RotationMap))
					{
						Texture rotationMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_VatFeature.m_RotationMap, false, TextureWrapMode.Clamp);
						if (logError && rotationMap == null)
						{
							Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_VatFeature.m_RotationMap, asset.AssetVirtualPath));
						}
						material.SetTexture(m_VATRotationMapPropertyName, rotationMap);
					}
					if (!string.IsNullOrEmpty(m_VATColorMapPropertyName) && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_ColorMap))
					{
						Texture colorMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_VatFeature.m_ColorMap, false, TextureWrapMode.Clamp);
						if (logError && colorMap == null)
						{
							Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_VatFeature.m_ColorMap, asset.AssetVirtualPath));
						}
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
				if (hasSkeletalAnim)
				{
					if (!string.IsNullOrEmpty(m_SkeletalAnimTexturePropertyName))
					{
						Texture animMap = PKFxMaterialFactory.GetTextureAsset(asset, batchDesc.m_SkeletalAnimFeature.m_AnimTextureMap, true, TextureWrapMode.Clamp);
						if (logError && animMap == null)
						{
							Debug.LogError(string.Format("[PopcornFX] Failed to create Texture for \"{0}\" in effect \"{1}\"", batchDesc.m_SkeletalAnimFeature.m_AnimTextureMap, asset.AssetVirtualPath));
						}
						material.SetTexture(m_SkeletalAnimTexturePropertyName, animMap);
					}
					if (!string.IsNullOrEmpty(m_SkeletalAnimTextureResolPropertyName))
					{
						Vector2 texResol = new Vector2((float)batchDesc.m_SkeletalAnimFeature.m_TextureResolX, (float)batchDesc.m_SkeletalAnimFeature.m_TextureResolY);
						material.SetVector(m_SkeletalAnimTextureResolPropertyName, texResol);
					}
					if (!string.IsNullOrEmpty(m_SkeletalAnimCountPropertyName))
					{
						material.SetFloat(m_SkeletalAnimCountPropertyName, (float)batchDesc.m_SkeletalAnimFeature.m_AnimCount);
					}
					if (!string.IsNullOrEmpty(m_SkeletalAnimUseBoneScalePropertyName))
					{
						material.SetInt(m_SkeletalAnimUseBoneScalePropertyName, batchDesc.m_SkeletalAnimFeature.m_UseBoneScale ? 1 : 0);
					}
					if (!string.IsNullOrEmpty(m_SkeletalAnimTranslationBoundsMinPropertyName))
					{
						material.SetVector(m_SkeletalAnimTranslationBoundsMinPropertyName, batchDesc.m_SkeletalAnimFeature.m_TranslationBoundsMin);
					}
					if (!string.IsNullOrEmpty(m_SkeletalAnimTranslationBoundsMaxPropertyName))
					{
						material.SetVector(m_SkeletalAnimTranslationBoundsMaxPropertyName, batchDesc.m_SkeletalAnimFeature.m_TranslationBoundsMax);
					}
					if (!string.IsNullOrEmpty(m_SkeletalAnimScaleBoundsMinPropertyName))
					{
						material.SetVector(m_SkeletalAnimScaleBoundsMinPropertyName, batchDesc.m_SkeletalAnimFeature.m_ScaleBoundsMin);
					}
					if (!string.IsNullOrEmpty(m_SkeletalAnimScaleBoundsMaxPropertyName))
					{
						material.SetVector(m_SkeletalAnimScaleBoundsMaxPropertyName, batchDesc.m_SkeletalAnimFeature.m_ScaleBoundsMax);
					}
				}
            }
			else if (batchDesc.m_Type == ERendererType.Decal)
			{
                if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Atlas))
                {
                    if (!string.IsNullOrEmpty(m_DecalHasAtlasPropertyName))
						material.SetInt(m_DecalHasAtlasPropertyName, 1);		

                    if (!string.IsNullOrEmpty(m_DecalTextureAtlasPropertyName))
						material.SetTexture(m_DecalTextureAtlasPropertyName, batchDesc.m_AtlasSubRects);
                }
            }
		}

#if UNITY_EDITOR

		private void _EnableMaterialKeywords(Material material, string keyword)
		{
			material.EnableKeyword(keyword);
			material.EnableKeyword(keyword + "_ON");
			material.SetFloat(keyword, 1.0f);
			material.SetFloat(keyword + "_ON", 1.0f);
		}
		private void _DisableMaterialKeywords(Material material, string keyword)
		{
			material.DisableKeyword(keyword);
			material.DisableKeyword(keyword + "_ON");
			material.SetFloat(keyword, 0.0f);
			material.SetFloat(keyword + "_ON", 0.0f);
		}

        public void SetMaterialKeywords(SBatchDesc batchDesc, Material material)
		{
			if (batchDesc.m_Type == ERendererType.Mesh)
			{
				// VAT enum: PK_HAS_VAT_FLUID PK_HAS_VAT_RIGID PK_HAS_VAT_SOFT PK_HAS_VAT_NONE
				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_FluidVAT))
				{
					_EnableMaterialKeywords(material, "PK_HAS_VAT_FLUID");
					_DisableMaterialKeywords(material, "PK_HAS_VAT_RIGID");
					_DisableMaterialKeywords(material, "PK_HAS_VAT_SOFT");
					_DisableMaterialKeywords(material, "PK_HAS_VAT_NONE");
					material.SetFloat("PK_HAS_VAT", 2.0f);
				}
				else if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_RigidVAT))
				{
					_DisableMaterialKeywords(material, "PK_HAS_VAT_FLUID");
					_EnableMaterialKeywords(material, "PK_HAS_VAT_RIGID");
					_DisableMaterialKeywords(material, "PK_HAS_VAT_SOFT");
					_DisableMaterialKeywords(material, "PK_HAS_VAT_NONE");
					material.SetFloat("PK_HAS_VAT", 1.0f);
				}
				else if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SoftVAT))
				{
					_DisableMaterialKeywords(material, "PK_HAS_VAT_FLUID");
					_DisableMaterialKeywords(material, "PK_HAS_VAT_RIGID");
					_EnableMaterialKeywords(material, "PK_HAS_VAT_SOFT");
					_DisableMaterialKeywords(material, "PK_HAS_VAT_NONE");
					material.SetFloat("PK_HAS_VAT", 3.0f);
				}
				else
				{
					_DisableMaterialKeywords(material, "PK_HAS_VAT_FLUID");
					_DisableMaterialKeywords(material, "PK_HAS_VAT_RIGID");
					_DisableMaterialKeywords(material, "PK_HAS_VAT_SOFT");
					_EnableMaterialKeywords(material, "PK_HAS_VAT_NONE");
					material.SetFloat("PK_HAS_VAT", 0.0f);
				}

				// Skeletal anim: PK_HAS_SKELETAL_ANIM
				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SkeletalAnim))
					_EnableMaterialKeywords(material, "PK_HAS_SKELETAL_ANIM");
				else
					_DisableMaterialKeywords(material, "PK_HAS_SKELETAL_ANIM");
				// Skeletal anim: PK_HAS_SKELETAL_ANIM_INTERPOL
				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SkeletalInterpol))
					_EnableMaterialKeywords(material, "PK_HAS_SKELETAL_ANIM_INTERPOL");
				else
					_DisableMaterialKeywords(material, "PK_HAS_SKELETAL_ANIM_INTERPOL");
				// Skeletal anim: PK_HAS_SKELETAL_ANIM_INTERPOL_TRACKS
				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SkeletalTrackInterpol))
					_EnableMaterialKeywords(material, "PK_HAS_SKELETAL_ANIM_INTERPOL_TRACKS");
				else
					_DisableMaterialKeywords(material, "PK_HAS_SKELETAL_ANIM_INTERPOL_TRACKS");
			}
            if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DiffuseMap))
                _EnableMaterialKeywords(material, "PK_HAS_DIFFUSE_MAP");
            else
                _DisableMaterialKeywords(material, "PK_HAS_DIFFUSE_MAP");
            // Anim-blend / ribbon-complex : PK_HAS_RIBBON_COMPLEX or PK_HAS_ANIM_BLEND
            if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_RibbonComplex))
			{
				_EnableMaterialKeywords(material, "PK_HAS_RIBBON_COMPLEX");
			}
			else if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Atlas))
			{
				_EnableMaterialKeywords(material, "PK_HAS_ATLAS");
				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AnimBlend))
				{
					_EnableMaterialKeywords(material, "PK_HAS_ANIM_BLEND");
				}
			}
			else
			{
				_DisableMaterialKeywords(material, "PK_HAS_RIBBON_COMPLEX");
				_DisableMaterialKeywords(material, "PK_HAS_ANIM_BLEND");
			}
			// Emissive enum: PK_HAS_EMISSIVE_BASIC PK_HAS_EMISSIVE_WITH_RAMP PK_HAS_EMISSIVE_NONE
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Emissive))
			{
				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_EmissiveRamp))
				{
					_DisableMaterialKeywords(material, "PK_HAS_EMISSIVE_NONE");
					_DisableMaterialKeywords(material, "PK_HAS_EMISSIVE_BASIC");
					_EnableMaterialKeywords(material, "PK_HAS_EMISSIVE_WITH_RAMP");
					material.SetFloat("PK_HAS_EMISSIVE", 2.0f);
				}
				else
				{
					_DisableMaterialKeywords(material, "PK_HAS_EMISSIVE_NONE");
					_DisableMaterialKeywords(material, "PK_HAS_EMISSIVE_WITH_RAMP");
					_EnableMaterialKeywords(material, "PK_HAS_EMISSIVE_BASIC");
					material.SetFloat("PK_HAS_EMISSIVE", 1.0f);
				}
			}
			else
			{
				_DisableMaterialKeywords(material, "PK_HAS_EMISSIVE_WITH_RAMP");
				_DisableMaterialKeywords(material, "PK_HAS_EMISSIVE_BASIC");
				_EnableMaterialKeywords(material, "PK_HAS_EMISSIVE_NONE");
				material.SetFloat("PK_HAS_EMISSIVE", 0.0f);
			}
			// Diffuse ramp: PK_HAS_DIFFUSE_RAMP
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DiffuseRamp))
				_EnableMaterialKeywords(material, "PK_HAS_DIFFUSE_RAMP");
			else
				_DisableMaterialKeywords(material, "PK_HAS_DIFFUSE_RAMP");
			// Alpha remap: PK_HAS_ALPHA_REMAP
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaRemap))
				_EnableMaterialKeywords(material, "PK_HAS_ALPHA_REMAP");
			else
				_DisableMaterialKeywords(material, "PK_HAS_ALPHA_REMAP");
			// Distortion: PK_HAS_DISTORTION
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
			{
				_EnableMaterialKeywords(material, "PK_HAS_DISTORTION");
				material.SetFloat("_DistortionFactor", PKFxSettings.BlurFactor);
			}
			else
				_DisableMaterialKeywords(material, "PK_HAS_DISTORTION");
			// Lighting: PK_HAS_LIGHTING
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Lighting))
				_EnableMaterialKeywords(material, "PK_HAS_LIGHTING");
			else
				_DisableMaterialKeywords(material, "PK_HAS_LIGHTING");
			// Soft particles: PK_HAS_SOFT
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Soft) && PKFxSettings.EnableSoftParticles)
				_EnableMaterialKeywords(material, "PK_HAS_SOFT");
			else
				_DisableMaterialKeywords(material, "PK_HAS_SOFT");
			// Soft particles: PK_HAS_TRANSFORM_UVS
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_TransformUVs))
				_EnableMaterialKeywords(material, "PK_HAS_TRANSFORM_UVS");
			else
				_DisableMaterialKeywords(material, "PK_HAS_TRANSFORM_UVS");
            if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Atlas))
                _EnableMaterialKeywords(material, "PK_HAS_ATLAS");
            else
                _DisableMaterialKeywords(material, "PK_HAS_ATLAS");
            // Basic Transform UVs
            if (batchDesc.m_RotateUVs && batchDesc.m_Type != ERendererType.Ribbon)
                _EnableMaterialKeywords(material, "PK_HAS_ROTATE_UVS");
            else
                _DisableMaterialKeywords(material, "PK_HAS_ROTATE_UVS");
            if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaMasks))
                _EnableMaterialKeywords(material, "PK_HAS_ALPHA_MASKS");
            else
                _DisableMaterialKeywords(material, "PK_HAS_ALPHA_MASKS");
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_UVDistortions))
				_EnableMaterialKeywords(material, "PK_HAS_UV_DISTORTIONS");
			else
				_DisableMaterialKeywords(material, "PK_HAS_UV_DISTORTIONS");
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Dissolve))
				_EnableMaterialKeywords(material, "PK_HAS_DISSOLVE");
			else
				_DisableMaterialKeywords(material, "PK_HAS_DISSOLVE");
		}

		private bool m_ShowBindings = false;

		public bool	DrawEditorShaderInputBindings(SBatchDesc batchDesc, bool headerGroup = false)
		{
			if (batchDesc == null)
				return false;
			return DrawEditorShaderInputBindings(	batchDesc.m_ShaderVariationFlags,
													batchDesc.m_BlendMode != EBlendMode.Solid && batchDesc.m_BlendMode != EBlendMode.Masked,
													batchDesc.m_BlendMode != EBlendMode.Masked,
													batchDesc.m_Type == ERendererType.Mesh,
													batchDesc.m_Type == ERendererType.Decal,
													headerGroup);
		}

		public bool	DrawEditorShaderInputBindings(	int shaderVariationFlags,
													bool bindingHasTransparent,
													bool bindingHasMasked,
													bool bindingHasMeshRenderer,
													bool bindingHasDecalRenderer,
													bool headerGroup = false)
		{
			SerializedObject serializedObject = new SerializedObject(this);
			if (headerGroup)
				m_ShowBindings = EditorGUILayout.BeginFoldoutHeaderGroup(m_ShowBindings, "Shader Bindings");
			else
				m_ShowBindings = EditorGUILayout.Foldout(m_ShowBindings, "Shader Bindings");
			if (m_ShowBindings)
			{
				if (bindingHasTransparent)
				{
					SerializedProperty SourceBlendPropertyName = serializedObject.FindProperty("m_SourceBlendPropertyName");
					SourceBlendPropertyName.stringValue = EditorGUILayout.TextField("Source blend mode (Int - UnityEngine.Rendering.BlendMode): ", SourceBlendPropertyName.stringValue);
					SerializedProperty DestinationBlendPropertyName = serializedObject.FindProperty("m_DestinationBlendPropertyName");
					DestinationBlendPropertyName.stringValue = EditorGUILayout.TextField("Destination blend mode (Int - UnityEngine.Rendering.BlendMode): ", DestinationBlendPropertyName.stringValue);
				}
				if (bindingHasMasked)
				{
					SerializedProperty MaskThresholdPropertyName = serializedObject.FindProperty("m_MaskThresholdPropertyName");
					MaskThresholdPropertyName.stringValue = EditorGUILayout.TextField("Cutout alpha for opaque (Float): ", MaskThresholdPropertyName.stringValue);
				}
				SerializedProperty RotateUVsPropertyName = serializedObject.FindProperty("m_RotateUVsPropertyName");
				RotateUVsPropertyName.stringValue = EditorGUILayout.TextField("RotateUVs (Int 0 or 1): ", RotateUVsPropertyName.stringValue);
				if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_DiffuseMap))
				{
					SerializedProperty DiffuseMapPropertyName = serializedObject.FindProperty("m_DiffuseMapPropertyName");
					DiffuseMapPropertyName.stringValue = EditorGUILayout.TextField("Diffuse (Texture2D): ", DiffuseMapPropertyName.stringValue);
				}
				if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_DistortionMap))
				{
					SerializedProperty DistortionMapPropertyName = serializedObject.FindProperty("m_DistortionMapPropertyName");
					DistortionMapPropertyName.stringValue = EditorGUILayout.TextField("Distortion (Texture2D): ", DistortionMapPropertyName.stringValue);
				}
				if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_Emissive))
				{
					SerializedProperty EmissiveMapPropertyName = serializedObject.FindProperty("m_EmissiveMapPropertyName");
					EmissiveMapPropertyName.stringValue = EditorGUILayout.TextField("Emissive (Texture2D): ", EmissiveMapPropertyName.stringValue);
				}
				if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_EmissiveRamp))
				{
					SerializedProperty EmissiveRampMapPropertyName = serializedObject.FindProperty("m_EmissiveRampMapPropertyName");
					EmissiveRampMapPropertyName.stringValue = EditorGUILayout.TextField("Emissive ramp (Texture2D): ", EmissiveRampMapPropertyName.stringValue);
				}
				if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_AlphaRemap))
				{
					SerializedProperty AlphaRemapPropertyName = serializedObject.FindProperty("m_AlphaRemapPropertyName");
					AlphaRemapPropertyName.stringValue = EditorGUILayout.TextField("Alpha remap (Texture2D): ", AlphaRemapPropertyName.stringValue);
				}
				if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_DiffuseRamp))
				{
					SerializedProperty DiffuseRampMapPropertyName = serializedObject.FindProperty("m_DiffuseRampMapPropertyName");
					DiffuseRampMapPropertyName.stringValue = EditorGUILayout.TextField("Diffuse ramp (Texture2D): ", DiffuseRampMapPropertyName.stringValue);
				}
				if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_Soft))
				{
					SerializedProperty InvSoftnessDistancePropertyName = serializedObject.FindProperty("m_InvSoftnessDistancePropertyName");
					InvSoftnessDistancePropertyName.stringValue = EditorGUILayout.TextField("Inverse softness distance (Float): ", InvSoftnessDistancePropertyName.stringValue);
				}
				if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_Lighting) ||
					HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_LightingLegacy))
				{
					SerializedProperty NormalMapPropertyName = serializedObject.FindProperty("m_NormalMapPropertyName");
					NormalMapPropertyName.stringValue = EditorGUILayout.TextField("Normal map (Texture2D): ", NormalMapPropertyName.stringValue);
					SerializedProperty RoughMetalMapPropertyName = serializedObject.FindProperty("m_RoughMetalMapPropertyName");
					RoughMetalMapPropertyName.stringValue = EditorGUILayout.TextField("Roughness / Metalness map (Texture2D): ", RoughMetalMapPropertyName.stringValue);
					SerializedProperty RoughnessPropertyName = serializedObject.FindProperty("m_RoughnessPropertyName");
					RoughnessPropertyName.stringValue = EditorGUILayout.TextField("Roughness (Float): ", RoughnessPropertyName.stringValue);
					SerializedProperty MetalnessPropertyName = serializedObject.FindProperty("m_MetalnessPropertyName");
					MetalnessPropertyName.stringValue = EditorGUILayout.TextField("Metalness (Float): ", MetalnessPropertyName.stringValue);
				}
				if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_TransformUVs))
				{
					SerializedProperty TransformUVs_RGBOnlyName = serializedObject.FindProperty("m_TransformUVsRGBOnlyPropertyName");
					TransformUVs_RGBOnlyName.stringValue = EditorGUILayout.TextField("TransformUVs IsRGB Only (Bool): ", TransformUVs_RGBOnlyName.stringValue);
				}
				if (bindingHasMeshRenderer)
				{
					SerializedProperty MeshDiffuseColorPropertyName = serializedObject.FindProperty("m_MeshDiffuseColorPropertyName");
					MeshDiffuseColorPropertyName.stringValue = EditorGUILayout.TextField("Mesh diffuse color (Vec4): ", MeshDiffuseColorPropertyName.stringValue);
					SerializedProperty MeshEmissiveColorPropertyName = serializedObject.FindProperty("m_MeshEmissiveColorPropertyName");
					MeshEmissiveColorPropertyName.stringValue = EditorGUILayout.TextField("Mesh emissive color (Vec4): ", MeshEmissiveColorPropertyName.stringValue);
					SerializedProperty MeshAlphaCursorPropertyName = serializedObject.FindProperty("m_MeshAlphaCursorPropertyName");
					MeshAlphaCursorPropertyName.stringValue = EditorGUILayout.TextField("Mesh alpha cursor (Float): ", MeshAlphaCursorPropertyName.stringValue);
					SerializedProperty MeshCullingModePropertyName = serializedObject.FindProperty("m_MeshCullingModePropertyName");
					MeshCullingModePropertyName.stringValue = EditorGUILayout.TextField("Mesh culling mode (Int): ", MeshCullingModePropertyName.stringValue);

					if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_FluidVAT) ||
						HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_SoftVAT) ||
						HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_RigidVAT))
					{
						SerializedProperty MeshVATCursorPropertyName = serializedObject.FindProperty("m_MeshVATCursorPropertyName");
						MeshVATCursorPropertyName.stringValue = EditorGUILayout.TextField("Mesh VAT cursor (Float): ", MeshVATCursorPropertyName.stringValue);
						SerializedProperty VATPositionMapPropertyName = serializedObject.FindProperty("m_VATPositionMapPropertyName");
						VATPositionMapPropertyName.stringValue = EditorGUILayout.TextField("VAT positions (Texture2D): ", VATPositionMapPropertyName.stringValue);
						SerializedProperty VATNormalMapPropertyName = serializedObject.FindProperty("m_VATNormalMapPropertyName");
						VATNormalMapPropertyName.stringValue = EditorGUILayout.TextField("VAT normals (Texture2D): ", VATNormalMapPropertyName.stringValue);
						SerializedProperty VATColorMapPropertyName = serializedObject.FindProperty("m_VATColorMapPropertyName");
						VATColorMapPropertyName.stringValue = EditorGUILayout.TextField("VAT colors (Texture2D): ", VATColorMapPropertyName.stringValue);
						SerializedProperty VATRotationMapPropertyName = serializedObject.FindProperty("m_VATRotationMapPropertyName");
						VATRotationMapPropertyName.stringValue = EditorGUILayout.TextField("VAT rotations (Texture2D): ", VATRotationMapPropertyName.stringValue);
						SerializedProperty VATNumFramesPropertyName = serializedObject.FindProperty("m_VATNumFramesPropertyName");
						VATNumFramesPropertyName.stringValue = EditorGUILayout.TextField("VAT num frames (Int): ", VATNumFramesPropertyName.stringValue);
						SerializedProperty VATPackedDataPropertyName = serializedObject.FindProperty("m_VATPackedDataPropertyName");
						VATPackedDataPropertyName.stringValue = EditorGUILayout.TextField("VAT packed data (Int): ", VATPackedDataPropertyName.stringValue);
						SerializedProperty VATColorPropertyName = serializedObject.FindProperty("m_VATColorPropertyName");
						VATColorPropertyName.stringValue = EditorGUILayout.TextField("VAT color (Vec4): ", VATColorPropertyName.stringValue);
						SerializedProperty VATNormalizedDataPropertyName = serializedObject.FindProperty("m_VATNormalizedDataPropertyName");
						VATNormalizedDataPropertyName.stringValue = EditorGUILayout.TextField("VAT normalized data (Int 0 or 1): ", VATNormalizedDataPropertyName.stringValue);
						SerializedProperty VATBoundsPositionPropertyName = serializedObject.FindProperty("m_VATBoundsPositionPropertyName");
						VATBoundsPositionPropertyName.stringValue = EditorGUILayout.TextField("VAT bounds positions (Vec2): ", VATBoundsPositionPropertyName.stringValue);
						SerializedProperty VATPadToPowerOf2PropertyName = serializedObject.FindProperty("m_VATPadToPowerOf2PropertyName");
						VATPadToPowerOf2PropertyName.stringValue = EditorGUILayout.TextField("VAT pad to power of 2 (Int 0 or 1): ", VATPadToPowerOf2PropertyName.stringValue);
						SerializedProperty VATPaddedRatioPropertyName = serializedObject.FindProperty("m_VATPaddedRatioPropertyName");
						VATPaddedRatioPropertyName.stringValue = EditorGUILayout.TextField("VAT Padded ratio (Vec2): ", VATPaddedRatioPropertyName.stringValue);
						if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_RigidVAT))
						{
							SerializedProperty VATBoundsPivotPropertyName = serializedObject.FindProperty("m_VATBoundsPivotPropertyName");
							VATBoundsPivotPropertyName.stringValue = EditorGUILayout.TextField("VAT bounds pivot (Vec2): ", VATBoundsPivotPropertyName.stringValue);
						}
					}
					if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_SkeletalAnim))
					{
						SerializedProperty skeletalAnimTexturePropertyName = serializedObject.FindProperty("m_SkeletalAnimTexturePropertyName");
						skeletalAnimTexturePropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation texture (Texture2D): ", skeletalAnimTexturePropertyName.stringValue);
						SerializedProperty skeletalAnimTextureResolPropertyName = serializedObject.FindProperty("m_SkeletalAnimTextureResolPropertyName");
						skeletalAnimTextureResolPropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation texture resolutuion (Vec2): ", skeletalAnimTextureResolPropertyName.stringValue);
						SerializedProperty skeletalAnimCountPropertyName = serializedObject.FindProperty("m_SkeletalAnimCountPropertyName");
						skeletalAnimCountPropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation count (Int): ", skeletalAnimCountPropertyName.stringValue);
						SerializedProperty skeletalAnimUseBoneScalePropertyName = serializedObject.FindProperty("m_SkeletalAnimUseBoneScalePropertyName");
						skeletalAnimUseBoneScalePropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation uses bones scale (Int 0 or 1): ", skeletalAnimUseBoneScalePropertyName.stringValue);
						SerializedProperty skeletalAnimCursor0PropertyName = serializedObject.FindProperty("m_SkeletalAnimCursor0PropertyName");
						skeletalAnimCursor0PropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation cursor current (Float): ", skeletalAnimCursor0PropertyName.stringValue);
						SerializedProperty skeletalAnimIdx0PropertyName = serializedObject.FindProperty("m_SkeletalAnimIdx0PropertyName");
						skeletalAnimIdx0PropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation idx current (Int): ", skeletalAnimIdx0PropertyName.stringValue);

						SerializedProperty skeletalAnimTranslationBoundsMinPropertyName = serializedObject.FindProperty("m_SkeletalAnimTranslationBoundsMinPropertyName");
						skeletalAnimTranslationBoundsMinPropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation positions bounds min (Vec3): ", skeletalAnimTranslationBoundsMinPropertyName.stringValue);
						SerializedProperty skeletalAnimTranslationBoundsMaxPropertyName = serializedObject.FindProperty("m_SkeletalAnimTranslationBoundsMaxPropertyName");
						skeletalAnimTranslationBoundsMaxPropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation positions bounds max (Vec3): ", skeletalAnimTranslationBoundsMaxPropertyName.stringValue);
						SerializedProperty skeletalAnimScaleBoundsMinPropertyName = serializedObject.FindProperty("m_SkeletalAnimScaleBoundsMinPropertyName");
						skeletalAnimScaleBoundsMinPropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation scales bounds min (Vec3): ", skeletalAnimScaleBoundsMinPropertyName.stringValue);
						SerializedProperty skeletalAnimScaleBoundsMaxPropertyName = serializedObject.FindProperty("m_SkeletalAnimScaleBoundsMaxPropertyName");
						skeletalAnimScaleBoundsMaxPropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation scales bounds max (Vec3): ", skeletalAnimScaleBoundsMaxPropertyName.stringValue);


						SerializedProperty meshTransformRow0PropertyName = serializedObject.FindProperty("m_MeshTransformRow0PropertyName");
						meshTransformRow0PropertyName.stringValue = EditorGUILayout.TextField("Mesh import transform matrix row 0 (Vec4): ", meshTransformRow0PropertyName.stringValue);
						SerializedProperty meshTransformRow1PropertyName = serializedObject.FindProperty("m_MeshTransformRow1PropertyName");
						meshTransformRow1PropertyName.stringValue = EditorGUILayout.TextField("Mesh import transform matrix row 1 (Vec4): ", meshTransformRow1PropertyName.stringValue);
						SerializedProperty meshTransformRow2PropertyName = serializedObject.FindProperty("m_MeshTransformRow2PropertyName");
						meshTransformRow2PropertyName.stringValue = EditorGUILayout.TextField("Mesh import transform matrix row 2 (Vec4): ", meshTransformRow2PropertyName.stringValue);
						SerializedProperty meshTransformRow3PropertyName = serializedObject.FindProperty("m_MeshTransformRow3PropertyName");
						meshTransformRow3PropertyName.stringValue = EditorGUILayout.TextField("Mesh import transform matrix row 3 (Vec4): ", meshTransformRow3PropertyName.stringValue);
					}
					if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_SkeletalTrackInterpol))
					{
						SerializedProperty skeletalAnimIdx1PropertyName = serializedObject.FindProperty("m_SkeletalAnimIdx1PropertyName");
						skeletalAnimIdx1PropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation idx next (Int): ", skeletalAnimIdx1PropertyName.stringValue);
						SerializedProperty skeletalAnimCursor1PropertyName = serializedObject.FindProperty("m_SkeletalAnimCursor1PropertyName");
						skeletalAnimCursor1PropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation cursor next (Float): ", skeletalAnimCursor1PropertyName.stringValue);
						SerializedProperty skeletalAnimTransitionPropertyName = serializedObject.FindProperty("m_SkeletalAnimTransitionPropertyName");
						skeletalAnimTransitionPropertyName.stringValue = EditorGUILayout.TextField("Skeletal animation transition (Float): ", skeletalAnimTransitionPropertyName.stringValue);
					}

					SerializedProperty useVertexColorPropertyName = serializedObject.FindProperty("m_UseVertexColorPropertyName");
					useVertexColorPropertyName.stringValue = EditorGUILayout.TextField("Use Vertex Color (Bool): ", useVertexColorPropertyName.stringValue);
				}
				if (bindingHasDecalRenderer)
				{
					SerializedProperty decalDiffuseColorPropertyName = serializedObject.FindProperty("m_DecalDiffuseColorPropertyName");
					decalDiffuseColorPropertyName.stringValue = EditorGUILayout.TextField("Decal Diffuse Color (Vec4): ", decalDiffuseColorPropertyName.stringValue);
					SerializedProperty decalEmissiveColorPropertyName = serializedObject.FindProperty("m_DecalEmissiveColorPropertyName");
					decalEmissiveColorPropertyName.stringValue = EditorGUILayout.TextField("Decal Emissive Color (Vec4): ", decalEmissiveColorPropertyName.stringValue);

                    if (HasShaderVariationFlag(shaderVariationFlags, EShaderVariationFlags.Has_Atlas))
                    {
                        SerializedProperty decalHasAtlasPropertyName = serializedObject.FindProperty("m_DecalHasAtlasPropertyName");
                        decalHasAtlasPropertyName.stringValue = EditorGUILayout.TextField("Decal Has Atlas (Bool): ", decalHasAtlasPropertyName.stringValue);

                        SerializedProperty decalAtlasDefinitionPropertyName = serializedObject.FindProperty("m_DecalAtlasDefinitionPropertyName");
                        decalAtlasDefinitionPropertyName.stringValue = EditorGUILayout.TextField("Decal Atlas Definition (Texture2D): ", decalAtlasDefinitionPropertyName.stringValue);

                        SerializedProperty decalAtlasSubdivsPropertyName = serializedObject.FindProperty("m_DecalAtlasSubdivsPropertyName");
                        decalAtlasSubdivsPropertyName.stringValue = EditorGUILayout.TextField("Decal Atlas Subdivs (Vec2): ", decalAtlasSubdivsPropertyName.stringValue);

                        SerializedProperty decalAtlasIdPropertyName = serializedObject.FindProperty("m_DecalAtlasIdPropertyName");
                        decalAtlasIdPropertyName.stringValue = EditorGUILayout.TextField("Decal Atlas ID (Float): ", decalAtlasIdPropertyName.stringValue);
                    }
				}

			}
			if (headerGroup)
				EditorGUILayout.EndFoldoutHeaderGroup();
			if (serializedObject.hasModifiedProperties)
			{
				serializedObject.ApplyModifiedProperties();
				return true;
			}
			return false;
		}

		private bool HasShaderVariationFlag(int mask, EShaderVariationFlags flag)
		{
			return (mask & (int)flag) == (int)flag;
		}
#endif
	}
}