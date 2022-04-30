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
			m_RenderFeatureBindings.Add(m_CPUBillboardingDefault);
			m_RenderFeatureBindings.Add(m_CPUBillboardingOpaqueDefault);
			m_RenderFeatureBindings.Add(m_VertexBillboardingDefault);
			m_RenderFeatureBindings.Add(m_VertexBillboardingOpaqueDefault);
			m_RenderFeatureBindings.Add(m_TransparentMeshUnlitDefault);
			m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitDefault);
			m_RenderFeatureBindings.Add(m_TransparentMeshLitDefault);
			m_RenderFeatureBindings.Add(m_OpaqueMeshLitDefault);
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
			_SetupMeshRenderer(batchDesc, gameObject, meshRenderer);
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
			Material material = TryFindAndInstantiateCustomMaterial(asset, batchDesc);
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

			binding.SetMaterialKeywords(batchDesc, material);
			binding.BindMaterialProperties(batchDesc, material, asset);

			if (batchDesc.m_Type != ERendererType.Mesh)
				material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}
	}
}