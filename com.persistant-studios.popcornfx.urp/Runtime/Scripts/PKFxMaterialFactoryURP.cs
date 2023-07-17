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
	public class PKFxMaterialFactoryURPUtils
	{
		static PKFxMaterialFactoryURPUtils()
		{
			if (PKFxSettings.CheckRenderPipeline() == PKFxSettings.ERenderPipeline.URP && PKFxSettings.MaterialFactory.GetType() != typeof(PKFxMaterialFactoryURP))
			{
				PKFxSettings.MaterialFactory = PKFxMaterialFactoryURP.CreateMaterialFactoryURP();
			}
		}
	}

#endif

	[Serializable]
	public class PKFxMaterialFactoryURP : PKFxMaterialFactory
	{
		[HideInInspector] public PKFxRenderFeatureBinding	m_TransparentMeshUnlitDoubleSidedDefault;
		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueMeshUnlitDoubleSidedDefault;
		[HideInInspector] public PKFxRenderFeatureBinding	m_TransparentMeshLitDoubleSidedDefault;
		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueMeshLitDoubleSidedDefault;
		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueMeshLitSkinnedDoubleSidedDefault;

		[HideInInspector] public PKFxRenderFeatureBinding	m_TransparentMeshUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueMeshUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_TransparentMeshLit2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueMeshLit2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueMeshLitSkinned2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_TransparentMeshUnlitDoubleSided2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueMeshUnlitDoubleSided2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_TransparentMeshLitDoubleSided2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueMeshLitDoubleSided2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueMeshLitSkinnedDoubleSided2020;

		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueParticleUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_TransparentParticleUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_AdditiveParticleUnlit2020;

		[HideInInspector] public PKFxRenderFeatureBinding	m_OpaqueParticleLit2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_TransparentParticleLit2020;
		[HideInInspector] public PKFxRenderFeatureBinding	m_AdditiveParticleLit2020;

		public enum SurfaceType
		{
			Opaque,
			Transparent
		}

		public int m_RenderQueue = 3500;

#if UNITY_EDITOR
		[MenuItem("Assets/Create/PopcornFX/Material Factory/URP")]
		public static PKFxMaterialFactoryURP CreateMaterialFactoryURP()
		{
			string folderPath = GetSelectedPathOrFallback();
			string path = Path.Combine(folderPath, "URPMaterialFactory.asset");

			PKFxMaterialFactoryURP materialFactoryURP = CreateInstance<PKFxMaterialFactoryURP>();
			materialFactoryURP.SetupFallBackFeatureBinding();

			AssetDatabase.CreateAsset(materialFactoryURP, path);
			AssetDatabase.SaveAssets();

			EditorUtility.FocusProjectWindow();

			Selection.activeObject = materialFactoryURP;

			return materialFactoryURP;
		}

		public void CreateMaterialFactoryIFN()
		{
			
		}

#endif

		public override void SetupFallBackFeatureBinding()
		{
#if UNITY_2021_1_OR_NEWER
			// Meshes:
			if (m_TransparentMeshUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshUnlitDefault);
			if (m_OpaqueMeshUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitDefault);
			if (m_TransparentMeshLitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshLitDefault);
			if (m_OpaqueMeshLitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitDefault);
			if (m_OpaqueMeshLitSkinnedDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitSkinnedDefault);
			if (m_TransparentMeshUnlitDoubleSidedDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshUnlitDoubleSidedDefault);
			if (m_OpaqueMeshUnlitDoubleSidedDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitDoubleSidedDefault);
			if (m_TransparentMeshLitDoubleSidedDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshLitDoubleSidedDefault);
			if (m_OpaqueMeshLitDoubleSidedDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitDoubleSidedDefault);
			if (m_OpaqueMeshLitSkinnedDoubleSidedDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitSkinnedDoubleSidedDefault);
			// Particles Unlit:
			if (m_OpaqueParticleUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueParticleUnlitDefault);
			if (m_TransparentParticleUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentParticleUnlitDefault);
			if (m_AdditiveParticleUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_AdditiveParticleUnlitDefault);
			if (m_OpaqueCorrectDeformationUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueCorrectDeformationUnlitDefault);
			if (m_TransparentCorrectDeformationUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentCorrectDeformationUnlitDefault);
			if (m_AdditiveCorrectDeformationUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_AdditiveCorrectDeformationUnlitDefault);
			// Particles Lit:
			if (m_OpaqueParticleLitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueParticleLitDefault);
			if (m_TransparentParticleLitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentParticleLitDefault);
			if (m_AdditiveParticleLitDefault != null)
				m_RenderFeatureBindings.Add(m_AdditiveParticleLitDefault);
			if (m_OpaqueCorrectDeformationLitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueCorrectDeformationLitDefault);
			if (m_TransparentCorrectDeformationLitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentCorrectDeformationLitDefault);
			if (m_AdditiveCorrectDeformationLitDefault != null)
				m_RenderFeatureBindings.Add(m_AdditiveCorrectDeformationLitDefault);
#elif UNITY_2020_3_OR_NEWER
			// Meshes:
			if (m_TransparentMeshUnlit2020 != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshUnlit2020);
			if (m_OpaqueMeshUnlit2020 != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshUnlit2020);
			if (m_TransparentMeshLit2020 != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshLit2020);
			if (m_OpaqueMeshLit2020 != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLit2020);
			if (m_OpaqueMeshLitSkinned2020 != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitSkinned2020);
			if (m_TransparentMeshUnlitDoubleSided2020 != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshUnlitDoubleSided2020);
			if (m_OpaqueMeshUnlitDoubleSided2020 != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitDoubleSided2020);
			if (m_TransparentMeshLitDoubleSided2020 != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshLitDoubleSided2020);
			if (m_OpaqueMeshLitDoubleSided2020 != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitDoubleSided2020);
			if (m_OpaqueMeshLitSkinnedDoubleSided2020 != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitSkinnedDoubleSided2020);
			// Particles Unlit:
			if (m_OpaqueParticleUnlit2020 != null)
				m_RenderFeatureBindings.Add(m_OpaqueParticleUnlit2020);
			if (m_TransparentParticleUnlit2020 != null)
				m_RenderFeatureBindings.Add(m_TransparentParticleUnlit2020);
			if (m_AdditiveParticleUnlit2020 != null)
				m_RenderFeatureBindings.Add(m_AdditiveParticleUnlit2020);
			// Particles Lit:
			if (m_OpaqueParticleLit2020 != null)
				m_RenderFeatureBindings.Add(m_OpaqueParticleLit2020);
			if (m_TransparentParticleLit2020 != null)
				m_RenderFeatureBindings.Add(m_TransparentParticleLit2020);
			if (m_AdditiveParticleLit2020 != null)
				m_RenderFeatureBindings.Add(m_AdditiveParticleLit2020);
#endif
			if (m_CPUParticleDefault != null)
				m_RenderFeatureBindings.Add(m_CPUParticleDefault);
			if (m_CPUParticleOpaqueDefault != null)
				m_RenderFeatureBindings.Add(m_CPUParticleOpaqueDefault);
			if (m_VertexBillboardingDefault != null)
				m_RenderFeatureBindings.Add(m_VertexBillboardingDefault);
			if (m_VertexBillboardingOpaqueDefault != null)
				m_RenderFeatureBindings.Add(m_VertexBillboardingOpaqueDefault);
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
			Material material = null;
			PKFxEffectAsset.MaterialUIDToIndex index = asset.m_MaterialIndexes.Find(item => item.m_UID == batchDesc.m_UID && item.m_Quality == PKFxManager.StoredQualityLevel);
			if (index.m_Idx != -1)
				material = asset.m_Materials[index.m_Idx];
			else
				Debug.LogError("[PopcornFX] Trying to resolve material from PKFxEffectAsset that is not holding materials");
			if (material == null)
				return null;
			if (batchDesc.m_Type != ERendererType.Mesh)
				material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}
	}
}