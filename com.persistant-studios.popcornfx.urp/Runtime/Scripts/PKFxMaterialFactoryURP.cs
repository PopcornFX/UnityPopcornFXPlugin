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
	public class PKFxMaterialFactoryURP : PKFxMaterialFactory
	{
		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueLitMeshFallback;

		public enum SurfaceType
		{
			Opaque,
			Transparent
		}

		public int m_RenderQueue = 3500;
		public bool m_UseSortingLayers = false;

#if UNITY_EDITOR
		[MenuItem("Assets/Create/PopcornFX/Material Factory/URP")]
		static void CreateMaterialFactoryURP()
		{
			string folderPath = GetSelectedPathOrFallback();
			string path = Path.Combine(folderPath, "URPMaterialFactory.asset");

			PKFxMaterialFactoryURP materialFactoryURP = CreateInstance<PKFxMaterialFactoryURP>();
			materialFactoryURP.SetupFallBackFeatureBinding();

			AssetDatabase.CreateAsset(materialFactoryURP, path);
			AssetDatabase.SaveAssets();

			EditorUtility.FocusProjectWindow();

			Selection.activeObject = materialFactoryURP;
		}

#endif
		public override void SetupFallBackFeatureBinding()
		{
			m_RenderFeatureBindings.Add(m_VertexBillboardingFallback);
			m_RenderFeatureBindings.Add(m_CPUBillboardingFallback);
			m_RenderFeatureBindings.Add(m_TransparentMeshFallback);
			m_RenderFeatureBindings.Add(m_OpaqueMeshFallback);
			m_RenderFeatureBindings.Add(m_OpaqueLitMeshFallback);
			m_RenderFeatureBindings.Add(m_VertexBillboardingOpaque);
			m_RenderFeatureBindings.Add(m_CPUBillboardingOpaque);
		}

		public override void SetupRenderer(SBatchDesc batchDesc, GameObject gameObject, MeshRenderer meshRenderer)
		{
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
			{
				gameObject.layer = PKFxManager.DistortionLayer;
			}
			if (m_UseSortingLayers)
			{
				meshRenderer.sortingLayerName = "PopcornFX";
			}
		}

		public override void SetupMeshRenderer(SBatchDesc batchDesc, GameObject gameObject, PKFxMeshInstancesRenderer meshRenderer)
		{
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
			{
				gameObject.layer = PKFxManager.DistortionLayer;
			}

			PKFxEffectAsset buildAsset = PKFxManager.GetBuiltAsset();
			PKFxEffectAsset.DependencyDesc DepDesc = buildAsset.m_Dependencies.Find(x => batchDesc.m_MeshAsset.Contains(x.m_Path));
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
				PKFxRenderFeatureBinding binding = ResolveBatchBinding(batchDesc);
				meshRenderer.m_MeshesImportTransform = trans.ToArray();
				meshRenderer.Meshes = meshes.ToArray();
				meshRenderer.m_ColorPropertyName = binding.GetMeshColorPropertyName();
			}
			if (batchDesc.m_LitFeature != null)
			{
				meshRenderer.m_CastShadow = batchDesc.m_LitFeature.m_CastShadows;
				
			}
				
		}

		public override Material ResolveParticleMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null)
		{
			if (asset == null)
				asset = PKFxManager.GetBuiltAsset();
			if (asset == null)
			{
				Debug.LogError("Trying to resolve material from null PKFxFxAsset");
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
			bool isDistortion = batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap);
			Texture diffuseTexture = GetTextureAsset(asset, batchDesc.m_DiffuseMap, isDistortion, wrapMode);
			material.mainTexture = diffuseTexture;
			// Set the material uniforms:
			material.SetInt("_RotateUVs", batchDesc.m_RotateUVs ? 1 : 0);

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

			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_Lighting) &&
				batchDesc.m_LitFeature != null && batchDesc.m_LitFeature.m_NormalMap != null)
			{
				Texture normalTexture = GetTextureAsset(asset, batchDesc.m_LitFeature.m_NormalMap, true, wrapMode);
				string normalTextureName = binding.GetNormalMapPropertyName();
				if (normalTextureName != null && normalTexture != null)
					material.SetTexture(normalTextureName, normalTexture);
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
#if false
			if (batchDesc.m_BlendMode == EBlendMode.Additive ||
				batchDesc.m_BlendMode == EBlendMode.AdditiveNoAlpha)
			{
				bool alphaClip = material.GetFloat("_AlphaClip") == 1;
				if (alphaClip)
					material.EnableKeyword("_ALPHATEST_ON");
				else
					material.DisableKeyword("_ALPHATEST_ON");
				material.SetOverrideTag("RenderType", "Transparent");
				material.SetInt("_SrcBlend", (int)UnityEngine.Rendering.BlendMode.One);
				material.SetInt("_DstBlend", (int)UnityEngine.Rendering.BlendMode.One);
				material.SetInt("_ZWrite", 0);
				material.DisableKeyword("_ALPHAPREMULTIPLY_ON");
				material.renderQueue = (int)UnityEngine.Rendering.RenderQueue.Transparent;
				material.SetShaderPassEnabled("ShadowCaster", false);
			}
#endif
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