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

		[HideInInspector] public PKFxRenderFeatureBinding m_TransparentMeshUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueMeshUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_TransparentMeshLit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueMeshLit2020;

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
			m_RenderFeatureBindings.Add(m_CPUBillboardingDefault);
			m_RenderFeatureBindings.Add(m_CPUBillboardingOpaqueDefault);
			m_RenderFeatureBindings.Add(m_VertexBillboardingDefault);
			m_RenderFeatureBindings.Add(m_VertexBillboardingOpaqueDefault);
#if UNITY_2021_1_OR_NEWER
			m_RenderFeatureBindings.Add(m_TransparentMeshUnlitDefault);
			m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitDefault);
			m_RenderFeatureBindings.Add(m_TransparentMeshLitDefault);
			m_RenderFeatureBindings.Add(m_OpaqueMeshLitDefault);
#elif UNITY_2020_3_OR_NEWER
			m_RenderFeatureBindings.Add(m_TransparentMeshUnlit2020);
			m_RenderFeatureBindings.Add(m_OpaqueMeshUnlit2020);
			m_RenderFeatureBindings.Add(m_TransparentMeshLit2020);
			m_RenderFeatureBindings.Add(m_OpaqueMeshLit2020);
#endif
			ReplaceBindingsWithLegacy();
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

			PKFxRenderFeatureBinding binding = null;
			if (material == null)
			{
				binding = ResolveBatchBinding(batchDesc);
				if (binding)
					material = binding.GetMaterial();
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