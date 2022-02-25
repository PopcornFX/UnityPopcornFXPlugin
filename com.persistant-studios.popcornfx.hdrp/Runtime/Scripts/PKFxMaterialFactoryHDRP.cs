//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
	[Serializable]
	public class PKFxMaterialFactoryHDRP : PKFxMaterialFactory
	{
		public Material		m_DoubleSidedDistortionMaterial;
		public Shader		m_ParticleProceduralShader_Distortion;
		public int			m_RenderQueue = 3050; // In HDRP, the objects > 3100 are not drawn
		public bool			m_UseSortingLayers = false;


		[HideInInspector] public PKFxRenderFeatureBinding m_UnlitOpaqueMeshFallback;

#if UNITY_EDITOR
		[MenuItem("Assets/Create/PopcornFX/Material Factory/HDRP")]
		static void CreateMaterialFactoryHDRP()
		{
			string folderPath = GetSelectedPathOrFallback();
			string path = Path.Combine(folderPath, "HDRPMaterialFactory.asset");

			// Create the HDRP material factory asset:
			PKFxMaterialFactoryHDRP materialFactoryHDRP = CreateInstance<PKFxMaterialFactoryHDRP>();

			materialFactoryHDRP.SetupFallBackFeatureBinding();

			AssetDatabase.CreateAsset(materialFactoryHDRP, path);
			AssetDatabase.SaveAssets();

			EditorUtility.FocusProjectWindow();

			Selection.activeObject = materialFactoryHDRP;
		}

#endif
		public override void SetupFallBackFeatureBinding()
		{
			m_RenderFeatureBindings.Add(m_UnlitOpaqueMeshFallback);
			m_RenderFeatureBindings.Add(m_OpaqueMeshFallback);

			m_RenderFeatureBindings.Add(m_VertexBillboardingFallback);
			m_RenderFeatureBindings.Add(m_CPUBillboardingFallback);
			m_RenderFeatureBindings.Add(m_MeshVATFluidFallback);
			m_RenderFeatureBindings.Add(m_MeshVATSoftFallback);
			m_RenderFeatureBindings.Add(m_MeshVATRigidFallback);

			m_RenderFeatureBindings.Add(m_VertexBillboardingOpaque);
			m_RenderFeatureBindings.Add(m_CPUBillboardingOpaque);

			//m_RenderFeatureBindings.Add(m_TransparentMeshFallback);
		}

		public override void SetupRenderer(SBatchDesc batchDesc, GameObject gameObject, MeshRenderer meshRenderer)
		{
			if (m_UseSortingLayers)
			{
				meshRenderer.sortingLayerName = "PopcornFX";
			}
			int layer = 0;
			PKFxSettings.Instance.GetRenderingLayerForBatchDesc(batchDesc, out layer);
			gameObject.layer = layer;
		}

		public override void SetupMeshRenderer(SBatchDesc batchDesc, GameObject gameObject, PKFxMeshInstancesRenderer meshRenderer)
		{
			if (batchDesc.m_LitFeature != null)
				meshRenderer.m_CastShadow = batchDesc.m_LitFeature.m_CastShadows;
			else
				meshRenderer.m_CastShadow = false;

			PKFxEffectAsset.DependencyDesc DepDesc = PKFxManager.GetBuiltAsset().m_Dependencies.Find(x => batchDesc.m_MeshAsset.Contains(x.m_Path));
			if (DepDesc != null)
			{
				GameObject meshGO = DepDesc.m_Object as GameObject;
				List<Mesh> meshes = new List<Mesh>();
				List<Matrix4x4> trans = new List<Matrix4x4>();
				MeshFilter meshFilter = meshGO.GetComponent<MeshFilter>();
				if (meshFilter != null)
				{
					meshes.Add(meshFilter.sharedMesh);
					trans.Add(meshGO.transform.localToWorldMatrix);
				}
				if (meshes.Count == 0)
				{
					MeshFilter[] meshFilters = meshGO.GetComponentsInChildren<MeshFilter>();
					for (int i = 0; i < meshFilters.Length; ++i)
					{
						meshes.Add(meshFilters[i].sharedMesh);
						trans.Add(meshGO.transform.localToWorldMatrix);
					}
				}
				meshRenderer.m_MeshesImportTransform = trans.ToArray();
				meshRenderer.Meshes = meshes.ToArray();

				PKFxRenderFeatureBinding binding = ResolveBatchBinding(batchDesc);
				meshRenderer.m_ColorPropertyName = binding.GetMeshColorPropertyName();
				meshRenderer.m_CursorPropertyName = binding.GetMeshVATCursorPropertyName();
			}
			if (batchDesc.m_LitFeature != null)
				meshRenderer.m_CastShadow = batchDesc.m_LitFeature.m_CastShadows;
		}

		public override Material ResolveParticleMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null)
		{
			if (asset == null)
				asset = PKFxManager.GetBuiltAsset();
			if (asset == null)
			{
				Debug.LogError("[PopcornFX] Trying to resolve material from null PKFxEffectAsset");
				return null;
			}
			TextureWrapMode wrapMode = batchDesc.m_Type == ERendererType.Ribbon ? TextureWrapMode.Repeat : TextureWrapMode.Clamp;
			Material material = TryFindAndInstantiateCustomMaterial(asset, batchDesc, wrapMode);
			bool isCustom = true;

			// First we handle the distortion with the material directly:
			if (material == null && batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
			{
				Material distoMat = null;
				Texture distoTexture = GetTextureAsset(asset, batchDesc.m_DiffuseMap, true, wrapMode);

				//distoMat.CopyPropertiesFromMaterial(m_DistortionMaterial);

				if (PKFxSettings.UseGPUBillboarding)
				{
					distoMat = new Material(m_ParticleProceduralShader_Distortion);
					distoMat.SetTexture("_MainTex", distoTexture);

					if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Soft) && PKFxSettings.EnableSoftParticles)
					{
						distoMat.EnableKeyword("PK_HAS_SOFT");
						distoMat.SetFloat("_InvSoftnessDistance", batchDesc.m_InvSoftnessDistance);
					}

					distoMat.SetInt("_DistortionSrcBlend", (int)UnityEngine.Rendering.BlendMode.One);
					distoMat.SetInt("_DistortionDstBlend", (int)UnityEngine.Rendering.BlendMode.One);
					distoMat.SetInt("_DistortionBlurSrcBlend", (int)UnityEngine.Rendering.BlendMode.One);
					distoMat.SetInt("_DistortionBlurDstBlend", (int)UnityEngine.Rendering.BlendMode.One);
					distoMat.SetInt("_DistortionBlurBlendOp", (int)UnityEngine.Rendering.BlendOp.Add);

					distoMat.SetInt("_ZTestMode", (int)UnityEngine.Rendering.CompareFunction.LessEqual);
				}
				else
				{
					distoMat = new Material(m_DoubleSidedDistortionMaterial);
					distoMat.SetTexture("_DistortionVectorMap", distoTexture);
				}

				return distoMat;
			}
			PKFxRenderFeatureBinding binding = null;
			if (material == null)
			{
				binding = ResolveBatchBinding(batchDesc);
				if (binding)
					material = binding.GetMaterial();
				isCustom = false;
			}
			if (material == null)
				return null;

			// Set VAT textures
			bool hasFluidVAT = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_FluidVAT);
			bool hasSoftVAT = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SoftVAT);
			bool hasRigidVAT = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_RigidVAT);
			bool hasVAT = hasFluidVAT || hasSoftVAT || hasRigidVAT;

			if (batchDesc.m_Type == ERendererType.Mesh && hasVAT && batchDesc.m_VatFeature != null)
			{
				material.SetInt("_NumFrames", batchDesc.m_VatFeature.m_NumFrames);
				material.SetVector("_Bounds", batchDesc.m_VatFeature.m_BoundsPosition);
				material.SetInt("_PackedData", Convert.ToInt32(batchDesc.m_VatFeature.m_PackedData));
				material.SetInt("_NormalizedData", Convert.ToInt32(batchDesc.m_VatFeature.m_NormalizedData));

				if (hasRigidVAT)
					material.SetVector("_Pivot", batchDesc.m_VatFeature.m_BoundsPivot);

				if (!string.IsNullOrEmpty(batchDesc.m_VatFeature.m_PositionMap))
				{
					Texture positionTexture = GetTextureAsset(asset, batchDesc.m_VatFeature.m_PositionMap, true, wrapMode);
					if (positionTexture != null)
					{
						material.SetTexture("_PositionMap", positionTexture);
					}
				}
				if (!string.IsNullOrEmpty(batchDesc.m_VatFeature.m_NormalMap))
				{
					Texture normalTexture = GetTextureAsset(asset, batchDesc.m_VatFeature.m_NormalMap, true, wrapMode);
					if (normalTexture != null)
					{
						material.SetTexture("_NormalMap", normalTexture);
					}
				}
				if (hasRigidVAT && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_RotationMap))
				{
					Texture rotationTexture = GetTextureAsset(asset, batchDesc.m_VatFeature.m_RotationMap, true, wrapMode);
					if (rotationTexture != null)
					{
						material.SetTexture("_RotationMap", rotationTexture);
					}
				}
				if (hasFluidVAT && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_ColorMap))
				{
					Texture colorTexture = GetTextureAsset(asset, batchDesc.m_VatFeature.m_ColorMap, true, wrapMode);
					if (colorTexture != null)
					{
						material.SetTexture("_ColorMap", colorTexture);
					}
				}
			}

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
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Soft) && PKFxSettings.EnableSoftParticles)
			{
				material.EnableKeyword("PK_HAS_SOFT");
				material.SetFloat("_InvSoftnessDistance", batchDesc.m_InvSoftnessDistance);
			}
			if (isCustom)
			{
				int mainTextureId = Shader.PropertyToID("_MainTex");
				Texture mainTexture = material.GetTexture(mainTextureId);
				bool isLinear = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap) ? true : false;
				if (mainTexture == null)
					material.SetTexture(mainTextureId, GetTextureAsset(asset, batchDesc.m_DiffuseMap, isLinear, wrapMode));

				if (batchDesc.m_Type == ERendererType.Billboard)
				{
					int alphaTextureId = Shader.PropertyToID("_AlphaMap");
					Texture alphaTexture = material.GetTexture(alphaTextureId);
					if (alphaTexture == null &&
						batchDesc.m_AlphaRemap != null &&
						batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaRemap))
					{
						material.SetTexture(alphaTextureId, GetTextureAsset(asset, batchDesc.m_AlphaRemap, false, wrapMode));
					}
				}
				else if (batchDesc.m_Type == ERendererType.Mesh)
				{

				}

				//if (batchDesc.HasShaderVariationFlag(PKFxManagerImpl.EShaderVariationFlags.Has_DistortionMap))
				//{
				//	int distortionTextureId = Shader.PropertyToID("Distortion");
				//	Texture distortionTexture = material.GetTexture(distortionTextureId);
				//	if (distortionTexture == null)
				//	{
				//		material.SetTexture(distortionTextureId, GetTextureAsset(asset, batchDesc.m_DiffuseMap, false, wrapMode));
				//	}
				//}

				if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Lighting))
				{
					// Set the Normal texture:
					int normalTextureId = Shader.PropertyToID("_NormalMap");
					if (normalTextureId >= 0)
					{
						Texture normalTexture = material.GetTexture(normalTextureId);
						if (normalTexture == null)
							material.SetTexture(normalTextureId, GetTextureAsset(asset, batchDesc.m_LitFeature.m_NormalMap, false, wrapMode));
					}
					// Set the RMMask texture (disabled until editor fixes):
					//int roughMetalMaskId = Shader.PropertyToID("_RoughMetalMask");
					//if (roughMetalMaskId >= 0)
					//{
					//	Texture roughMetalMaskTexture = material.GetTexture(roughMetalMaskId);
					//	if (roughMetalMaskTexture == null)
					//	{
					//		material.SetTexture(roughMetalMaskId, GetTextureAsset(asset, batchDesc.m_LitFeature.m_RoughMetalMap, false, wrapMode));
					//	}
					//}
					int smoothId = Shader.PropertyToID("_Smoothness");
					int metalId = Shader.PropertyToID("_Metallic");

					if (smoothId >= 0 && metalId >= 0)
					{
						if (material.GetFloat(smoothId) < 0)
							material.SetFloat(smoothId, 1.0f - batchDesc.m_LitFeature.m_Roughness);
						if (material.GetFloat(metalId) < 0)
							material.SetFloat(metalId, batchDesc.m_LitFeature.m_Metalness);
					}
				}
			}

			int srcMode = 0;
			int dstMode = 0;
			// Additive and distortion
			if (batchDesc.m_BlendMode == EBlendMode.Additive)
			{
				srcMode = (int)UnityEngine.Rendering.BlendMode.SrcAlpha;
				dstMode = (int)UnityEngine.Rendering.BlendMode.One;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.AdditiveNoAlpha)
			{
				srcMode = (int)UnityEngine.Rendering.BlendMode.One;
				dstMode = (int)UnityEngine.Rendering.BlendMode.One;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.AlphaBlend)
			{
				srcMode = (int)UnityEngine.Rendering.BlendMode.SrcAlpha;
				dstMode = (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.PremultipliedAlpha)
			{
				srcMode = (int)UnityEngine.Rendering.BlendMode.One;
				dstMode = (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha;
			}
			else if (batchDesc.m_BlendMode == EBlendMode.Solid)
			{

			}
			else if (batchDesc.m_BlendMode == EBlendMode.Masked)
			{

			}

			if (!isCustom)
			{
				material.SetInt("_SrcBlendMode", srcMode);
				material.SetInt("_DstBlendMode", dstMode);
			}
			else
			{
				material.SetFloat("_SrcBlend", srcMode);
				material.SetFloat("_DstBlend", dstMode);
			}

			if (isCustom)
				return material;

			// Set the diffuse texture:
			Texture diffuseTexture = GetTextureAsset(asset, batchDesc.m_DiffuseMap, false, wrapMode);
			material.mainTexture = diffuseTexture;

			// Set the material uniforms:
			material.SetInt("_RotateUVs", batchDesc.m_RotateUVs ? 1 : 0);

			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Lighting) &&
				batchDesc.m_LitFeature != null && batchDesc.m_LitFeature.m_NormalMap != null)
			{
				Texture normalTexture = GetTextureAsset(asset, batchDesc.m_LitFeature.m_NormalMap, true, wrapMode);
				string normalTextureName = binding.GetNormalMapPropertyName();
				if (normalTextureName != null && normalTexture != null)
					material.SetTexture(normalTextureName, normalTexture);
			}

			if (batchDesc.m_Type == ERendererType.Mesh)
				return material;

			// Set the alpha remap texture
			if (batchDesc.m_AlphaRemap != null &&
				batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaRemap))
			{
				Texture alphaRemapTexture = GetTextureAsset(asset, batchDesc.m_AlphaRemap, true, wrapMode);
				Debug.Assert(alphaRemapTexture != null);
				alphaRemapTexture.wrapMode = TextureWrapMode.Clamp;
				material.SetTexture("_AlphaMap", alphaRemapTexture);
			}

			// Set the Diffuse ramp texture
			if (batchDesc.m_DiffuseRampMap != null &&
				batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DiffuseRamp))
			{
				Texture diffuseRampTexture = GetTextureAsset(asset, batchDesc.m_DiffuseRampMap, true, wrapMode);
				Debug.Assert(diffuseRampTexture != null);
				diffuseRampTexture.wrapMode = TextureWrapMode.Clamp;
				material.SetTexture("_DiffuseRampMap", diffuseRampTexture);
			}


			material.SetInt("_ZTestMode", (int)UnityEngine.Rendering.CompareFunction.LessEqual);
			material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}
	}
}