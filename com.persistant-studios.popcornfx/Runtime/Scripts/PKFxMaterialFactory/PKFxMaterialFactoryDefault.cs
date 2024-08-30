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

#if UNITY_EDITOR

	[InitializeOnLoad]
	public class PKFxMaterialFactoryDefaultUtils : AssetPostprocessor
	{
		internal static bool m_CreateFactory = false;

		static PKFxMaterialFactoryDefaultUtils()
		{
			if (PKFxSettings.CheckRenderPipeline() == PKFxSettings.ERenderPipeline.Legacy && PKFxSettings.MaterialFactory.GetType() != typeof(PKFxMaterialFactoryDefault))
			{
				m_CreateFactory = true;
			}
		}
		
		static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
		{
			if (m_CreateFactory)
			{
				m_CreateFactory = false;
				PKFxSettings.MaterialFactory = PKFxMaterialFactoryDefault.CreateMaterialFactoryDefaultAsset();
			}
		}
	}

#endif

	[Serializable]
	public class PKFxMaterialFactoryDefault : PKFxMaterialFactory
	{
		public int m_RenderQueue = 3500;

#if UNITY_EDITOR
		[MenuItem("Assets/Create/PopcornFX/Material Factory/Default")]
		static void CreateMaterialFactoryDefaultMenu()
		{
			string folderPath = GetSelectedPathOrFallback();
			string path = Path.Combine(folderPath, "DefaultMaterialFactory.asset");

			PKFxMaterialFactoryDefault materialFactoryDefault = CreateInstance<PKFxMaterialFactoryDefault>();
			materialFactoryDefault.m_FactoryType = FactoryType.DEFAULT;
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
			// Meshes:
			if (m_TransparentMeshUnlitLegacy != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshUnlitLegacy);
            if (m_EmissiveMeshUnlitDefault != null)
                m_RenderFeatureBindings.Add(m_EmissiveMeshUnlitDefault);
            if (m_OpaqueMeshUnlitLegacy != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitLegacy);
            if (m_OpaqueMeshUnlitDefault != null)
                m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitDefault);
            if (m_TransparentMeshLitLegacy != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshLitLegacy);
            if (m_OpaqueMeshLitLegacy != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitLegacy);
            if (m_OpaqueMeshLitDefault != null)
                m_RenderFeatureBindings.Add(m_OpaqueMeshLitDefault);
            // Particles Unlit:
            if (m_OpaqueParticleUnlitLegacy != null)
				m_RenderFeatureBindings.Add(m_OpaqueParticleUnlitLegacy);
            if (m_OpaqueParticleUnlitDefault != null)
                m_RenderFeatureBindings.Add(m_OpaqueParticleUnlitDefault);
            if (m_TransparentParticleUnlitLegacy != null)
				m_RenderFeatureBindings.Add(m_TransparentParticleUnlitLegacy);
            if (m_TransparentParticleUnlitDefault != null)
                m_RenderFeatureBindings.Add(m_TransparentParticleUnlitDefault);
            if (m_AdditiveParticleUnlitLegacy != null)
                m_RenderFeatureBindings.Add(m_AdditiveParticleUnlitLegacy);
			if (m_OpaqueCorrectDeformationUnlitLegacy != null)
				m_RenderFeatureBindings.Add(m_OpaqueCorrectDeformationUnlitLegacy);
            if (m_OpaqueCorrectDeformationUnlitDefault != null)
                m_RenderFeatureBindings.Add(m_OpaqueCorrectDeformationUnlitDefault);
            if (m_TransparentCorrectDeformationUnlitLegacy != null)
				m_RenderFeatureBindings.Add(m_TransparentCorrectDeformationUnlitLegacy);
            if (m_TransparentCorrectDeformationUnlitDefault != null)
                m_RenderFeatureBindings.Add(m_TransparentCorrectDeformationUnlitDefault);
            if (m_AdditiveCorrectDeformationUnlitLegacy != null)
				m_RenderFeatureBindings.Add(m_AdditiveCorrectDeformationUnlitLegacy);
			// Particles Lit:
			if (m_OpaqueParticleLitLegacy != null)
				m_RenderFeatureBindings.Add(m_OpaqueParticleLitLegacy);
            if (m_OpaqueParticleLitDefault != null)
                m_RenderFeatureBindings.Add(m_OpaqueParticleLitDefault);
            if (m_TransparentParticleLitLegacy != null)
				m_RenderFeatureBindings.Add(m_TransparentParticleLitLegacy);
            if (m_TransparentParticleLitDefault != null)
                m_RenderFeatureBindings.Add(m_TransparentParticleLitDefault);
            if (m_AdditiveParticleLitLegacy != null)
				m_RenderFeatureBindings.Add(m_AdditiveParticleLitLegacy);
			if (m_OpaqueCorrectDeformationLitLegacy != null)
				m_RenderFeatureBindings.Add(m_OpaqueCorrectDeformationLitLegacy);
            if (m_OpaqueCorrectDeformationLitDefault != null)
                m_RenderFeatureBindings.Add(m_OpaqueCorrectDeformationLitDefault);
            if (m_TransparentCorrectDeformationLitLegacy != null)
				m_RenderFeatureBindings.Add(m_TransparentCorrectDeformationLitLegacy);
            if (m_TransparentCorrectDeformationLitDefault != null)
                m_RenderFeatureBindings.Add(m_TransparentCorrectDeformationLitDefault);
            if (m_AdditiveCorrectDeformationLitLegacy != null)
				m_RenderFeatureBindings.Add(m_AdditiveCorrectDeformationLitLegacy);
			// Default shader fallback:
			if (m_CPUParticleLegacy != null)
				m_RenderFeatureBindings.Add(m_CPUParticleLegacy);
			if (m_CPUParticleOpaqueLegacy != null)
				m_RenderFeatureBindings.Add(m_CPUParticleOpaqueLegacy);
			if (m_VertexBillboardingLegacy != null)
				m_RenderFeatureBindings.Add(m_VertexBillboardingLegacy);
			if (m_VertexBillboardingOpaqueLegacy != null)
				m_RenderFeatureBindings.Add(m_VertexBillboardingOpaqueLegacy);
		}

		public override void SetupMeshRenderer(SBatchDesc batchDesc, GameObject gameObject, PKFxMeshInstancesRenderer meshRenderer)
		{
			_SetupMeshRenderer(batchDesc, gameObject, meshRenderer);
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
				gameObject.layer = LayerMask.NameToLayer(PKFxManagerImpl.m_DistortionLayer);
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
			PKFxEffectAsset.MaterialUIDToIndex index = asset.m_MaterialIndexes.Find(item => item.m_UID == batchDesc.m_UID && item.m_Quality == PKFxManager.StoredQualityLevel);
			if (index == null)
				return null;
			if (index.m_Idx == -1)
				return null;
			Material material = asset.m_Materials[index.m_Idx];
			if (material == null)
				return null;
			// Set the render queue:
			material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}
	}
}