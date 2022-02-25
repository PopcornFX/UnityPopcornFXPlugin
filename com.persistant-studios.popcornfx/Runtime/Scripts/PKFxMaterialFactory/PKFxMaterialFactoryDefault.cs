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
	public class PKFxMaterialFactoryDefault : PKFxMaterialFactory
	{
		public int m_RenderQueue = 3500;
		public bool m_UseSortingLayers = false;

#if UNITY_EDITOR
		[MenuItem("Assets/Create/PopcornFX/Material Factory/Default")]
		static void CreateMaterialFactoryDefaultMenu()
		{
			string folderPath = GetSelectedPathOrFallback();
			string path = Path.Combine(folderPath, "DefaultMaterialFactory.asset");

			PKFxMaterialFactoryDefault materialFactoryDefault = CreateInstance<PKFxMaterialFactoryDefault>();
			materialFactoryDefault.SetupFallBackFeatureBinding();

			AssetDatabase.CreateAsset(materialFactoryDefault, path);
			AssetDatabase.SaveAssets();

			EditorUtility.FocusProjectWindow();

			Selection.activeObject = materialFactoryDefault;
		}
#endif

		public static PKFxMaterialFactoryDefault CreateMaterialFactoryDefaultAsset()
		{
			PKFxMaterialFactoryDefault materialFactoryDefault = CreateInstance<PKFxMaterialFactoryDefault>();

			materialFactoryDefault.SetupFallBackFeatureBinding();
#if UNITY_EDITOR
			AssetDatabase.CreateAsset(materialFactoryDefault, "Assets/Resources/DefaultMaterialFactory.asset");
			AssetDatabase.SaveAssets();
#endif
			return materialFactoryDefault;
		}


		public override void SetupFallBackFeatureBinding()
		{
			m_RenderFeatureBindings.Add(m_VertexBillboardingFallback);
			m_RenderFeatureBindings.Add(m_CPUBillboardingFallback);
			m_RenderFeatureBindings.Add(m_TransparentMeshFallback);
			m_RenderFeatureBindings.Add(m_OpaqueMeshFallback);
			m_RenderFeatureBindings.Add(m_MeshVATFluidFallback);
			m_RenderFeatureBindings.Add(m_MeshVATSoftFallback);
			m_RenderFeatureBindings.Add(m_MeshVATRigidFallback);
			m_RenderFeatureBindings.Add(m_VertexBillboardingOpaque);
			m_RenderFeatureBindings.Add(m_CPUBillboardingOpaque);
		}

		public override void SetupRenderer(SBatchDesc batchDesc, GameObject gameObject, MeshRenderer meshRenderer)
		{
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
				gameObject.layer = PKFxManagerImpl.m_DistortionLayer;
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
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
				gameObject.layer = PKFxManagerImpl.m_DistortionLayer;

			PKFxEffectAsset.DependencyDesc DepDesc = PKFxManager.GetBuiltAsset().m_Dependencies.Find(x => batchDesc.m_MeshAsset.Contains(x.m_Path));
			if (DepDesc != null)
			{
				GameObject			meshGO = DepDesc.m_Object as GameObject;
				List<Mesh>			meshes = new List<Mesh>();
				List<Matrix4x4>		trans = new List<Matrix4x4>();
				MeshFilter			meshFilter = meshGO.GetComponent<MeshFilter>();
				if (meshFilter != null)
				{
					meshes.Add(meshFilter.sharedMesh);
					trans.Add(meshGO.transform.localToWorldMatrix);
				}
				if (meshes.Count == 0)
				{
					MeshFilter[]	meshFilters = meshGO.GetComponentsInChildren<MeshFilter>();
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
				Debug.LogError("[PopcornFX] Trying to resolve material from null PKFxFxAsset");
				return null;
			}
			TextureWrapMode wrapMode = batchDesc.m_Type == ERendererType.Ribbon ? TextureWrapMode.Repeat : TextureWrapMode.Clamp;
			Material customMat = TryFindAndInstantiateCustomMaterial(asset, batchDesc, wrapMode);

			if (customMat != null)
				return customMat;

			Material					material = null;
			PKFxRenderFeatureBinding	binding = null;
			binding = ResolveBatchBinding(batchDesc);
			if (binding)
				material = binding.GetMaterial();
			if (material == null)
				return null;

			// Set the diffuse texture:
			bool	isDistortion = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap);
			Texture	diffuseTexture = GetTextureAsset(asset, batchDesc.m_DiffuseMap, isDistortion, wrapMode);
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
				material.SetInt("_PadToPowerOf2", Convert.ToInt32(batchDesc.m_VatFeature.m_PadToPowerOf2));

				if (batchDesc.m_VatFeature.m_PadToPowerOf2)
					material.SetVector("_PaddedRatio", batchDesc.m_VatFeature.m_PaddedRatio);

				if (hasRigidVAT)
					material.SetVector("_Pivot", batchDesc.m_VatFeature.m_BoundsPivot);

				if (!string.IsNullOrEmpty(batchDesc.m_VatFeature.m_PositionMap))
				{
					Texture positionTexture = GetTextureAsset(asset, batchDesc.m_VatFeature.m_PositionMap, false, wrapMode);
					if (positionTexture != null)
					{
						material.SetTexture("_PositionMap", positionTexture);
					}
				}
				if (!string.IsNullOrEmpty(batchDesc.m_VatFeature.m_NormalMap))
				{
					Texture normalTexture = GetTextureAsset(asset, batchDesc.m_VatFeature.m_NormalMap, false, wrapMode);
					if (normalTexture != null)
					{
						material.SetTexture("_NormalMap", normalTexture);
					}
				}
				if (hasRigidVAT && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_RotationMap))
				{
					Texture rotationTexture = GetTextureAsset(asset, batchDesc.m_VatFeature.m_RotationMap, false, wrapMode);
					if (rotationTexture != null)
					{
						material.SetTexture("_RotationMap", rotationTexture);
					}
				}
				if (hasFluidVAT && !string.IsNullOrEmpty(batchDesc.m_VatFeature.m_ColorMap))
				{
					Texture colorTexture = GetTextureAsset(asset, batchDesc.m_VatFeature.m_ColorMap, false, wrapMode);
					if (colorTexture != null)
					{
						material.SetTexture("_ColorMap", colorTexture);
					}
				}
			}

			// Set the alpha remap texture
			if (batchDesc.m_AlphaRemap != null && batchDesc.m_AlphaRemap.Length != 0)
			{
				Texture alphaRemapTexture = GetTextureAsset(asset, batchDesc.m_AlphaRemap, true, wrapMode);
				if (alphaRemapTexture != null)
				{
					alphaRemapTexture.wrapMode = TextureWrapMode.Clamp;
					material.SetTexture("_AlphaMap", alphaRemapTexture);
				}
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

			material.SetInt("_SrcBlendMode", srcMode);
			material.SetInt("_DstBlendMode", dstMode);

			material.SetInt("_ZTestMode", (int)UnityEngine.Rendering.CompareFunction.LessEqual);

			// We can stop here if the material is for a mesh particle:
			if (batchDesc.m_Type == ERendererType.Mesh)
				return material;

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
			// PK_HAS_COLOR is now always enabled to avoid having too many shader variations
#if false
		if (batchDesc.HasShaderVariationFlag(PKFxManagerImpl.EShaderVariationFlags.Has_Color))
			material.EnableKeyword("PK_HAS_COLOR");
#endif
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Soft) && PKFxSettings.EnableSoftParticles)
			{
				material.EnableKeyword("PK_HAS_SOFT");
				material.SetFloat("_InvSoftnessDistance", batchDesc.m_InvSoftnessDistance);
			}
			// Set the render queue:
			material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}
	}
}