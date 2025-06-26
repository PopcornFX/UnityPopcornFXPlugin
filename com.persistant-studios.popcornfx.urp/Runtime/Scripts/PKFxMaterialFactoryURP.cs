//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering.Universal;
using System;
using System.Runtime.InteropServices;
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
#if UNITY_EDITOR

	[InitializeOnLoad]
	public class PKFxMaterialFactoryURPUtils
	{
		internal static bool m_CreateFactory = false;
		static PKFxMaterialFactoryURPUtils()
		{
			if (PKFxSettings.CheckRenderPipeline() == PKFxSettings.ERenderPipeline.URP && PKFxSettings.MaterialFactory.GetType() != typeof(PKFxMaterialFactoryURP))
			{
				m_CreateFactory = true;
			}
		}

		static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
		{
			if (m_CreateFactory)
			{
				m_CreateFactory = false;
				PKFxSettings.MaterialFactory = PKFxMaterialFactoryURP.CreateMaterialFactoryURP();
			}
		}
	}

#endif

	[Serializable]
	public class PKFxMaterialFactoryURP : PKFxMaterialFactory
	{
		[HideInInspector] public PKFxRenderFeatureBinding m_TransparentMeshUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueMeshUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_TransparentMeshLit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueMeshLit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueMeshLitSkinned2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_TransparentMeshUnlitDoubleSided2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueMeshUnlitDoubleSided2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_TransparentMeshLitDoubleSided2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueMeshLitDoubleSided2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueMeshLitSkinnedDoubleSided2020;

		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueParticleUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_TransparentParticleUnlit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_AdditiveParticleUnlit2020;

		[HideInInspector] public PKFxRenderFeatureBinding m_OpaqueParticleLit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_TransparentParticleLit2020;
		[HideInInspector] public PKFxRenderFeatureBinding m_AdditiveParticleLit2020;

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

			PKFxShadergraphSettingSetter.SetVariantCount();

			PKFxMaterialFactoryURP materialFactoryURP = CreateInstance<PKFxMaterialFactoryURP>();
			materialFactoryURP.m_FactoryType = FactoryType.URP;
			materialFactoryURP.SetupFallBackFeatureBinding();

			AssetDatabase.CreateAsset(materialFactoryURP, path);
			AssetDatabase.SaveAssets();

			EditorUtility.FocusProjectWindow();

			Selection.activeObject = materialFactoryURP;

			return materialFactoryURP;
		}

#endif

		public override void SetupFallBackFeatureBinding()
		{
#if UNITY_2021_1_OR_NEWER
			// Meshes:
			if (m_TransparentMeshUnlitLegacy != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshUnlitLegacy);
			if (m_TransparentMeshUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshUnlitDefault);
			if (m_EmissiveMeshUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_EmissiveMeshUnlitDefault);
			if (m_OpaqueMeshUnlitLegacy != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitLegacy);
			if (m_OpaqueMeshUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitDefault);
			if (m_TransparentMeshLitLegacy != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshLitLegacy);
			if (m_TransparentMeshLitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshLitDefault);
			if (m_OpaqueMeshLitLegacy != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitLegacy);
			if (m_OpaqueMeshLitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitDefault);
			if (m_OpaqueMeshLitSkinnedLegacy != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitSkinnedLegacy);
			//Ribbons
			// Lit:
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
			// Unlit:
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
			if (m_AlphaBlendDecal != null)
				m_RenderFeatureBindings.Add(m_AlphaBlendDecal);
			if (m_CPUParticleDistortion != null)
				m_RenderFeatureBindings.Add(m_CPUParticleDistortion);

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
			material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}

		public override GameObject GetDecalTemplate()
		{
			GameObject template = new GameObject("PopcornFX URP Decal");
			PKFxURPDecalData data = template.AddComponent<PKFxURPDecalData>();
			DecalProjector decalProjector = template.AddComponent<DecalProjector>();
			decalProjector.pivot = Vector3.zero;
			data.m_DecalProjector = decalProjector;

			return template;
		}

		public override void SetDecalValue(PKFxDecalData data, PKFxDecalPool.SDecalInfo info)
		{
			base.SetDecalValue(data, info);

			PKFxURPDecalData URPData = data as PKFxURPDecalData;
			if (URPData == null)
				return;

			URPData.m_DecalProjector.transform.position = info.m_Position;
			URPData.m_DecalProjector.size = info.m_Scale;

			// Tweak orientation to match Unity's decal renderers' starting point
			info.m_Orientation *= Quaternion.Euler(Vector3.right * 90);

			URPData.m_DecalProjector.transform.rotation = info.m_Orientation;

			URPData.m_DecalProjector.material = data.m_Material;

			URPData.m_DecalProjector.material.SetVector("_BaseColor", info.m_DiffuseColor);
			URPData.m_DecalProjector.material.SetVector("_EmissiveColor", info.m_EmissiveColor);

			URPData.m_DecalProjector.material.SetTexture("_AtlasRects", data.m_AtlasRects);
			URPData.m_DecalProjector.material.SetFloat("_AtlasId", info.m_AtlasID);
		}
	}
}