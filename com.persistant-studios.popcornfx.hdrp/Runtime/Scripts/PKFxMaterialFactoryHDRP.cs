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
using UnityEngine.Rendering.HighDefinition;

namespace PopcornFX
{

#if UNITY_EDITOR

	[InitializeOnLoad]
	public class PKFxMaterialFactoryHDRPUtils : AssetPostprocessor
	{
		internal static bool m_CreateFactory = false;
		static PKFxMaterialFactoryHDRPUtils()
		{
			if (PKFxSettings.CheckRenderPipeline() == PKFxSettings.ERenderPipeline.HDRP && PKFxSettings.MaterialFactory.GetType() != typeof(PKFxMaterialFactoryHDRP))
			{
				m_CreateFactory = true;
			}
		}

		static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
		{
			if (m_CreateFactory)
			{
				m_CreateFactory = false;
				PKFxSettings.MaterialFactory = PKFxMaterialFactoryHDRP.CreateMaterialFactoryHDRP();
			}
		}
	}

#endif

	[Serializable]
	public class PKFxMaterialFactoryHDRP : PKFxMaterialFactory
	{
		public int			m_RenderQueue = 3050; // In HDRP, the objects > 3100 are not drawn

		[HideInInspector] public PKFxRenderFeatureBinding			m_TransparentMeshUnlitDoubleSidedDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueMeshUnlitDoubleSidedDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_TransparentMeshLitDoubleSidedDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueMeshLitDoubleSidedDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueMeshLitSkinnedDoubleSidedDefault;

		public PKFxRenderFeatureBinding								m_DistortionBillboard_Default;
		public PKFxRenderFeatureBinding								m_DistortionBillboard;
#if UNITY_EDITOR
		[MenuItem("Assets/Create/PopcornFX/Material Factory/HDRP")]
		public static PKFxMaterialFactoryHDRP CreateMaterialFactoryHDRP()
		{
			string folderPath = GetSelectedPathOrFallback();
			string path = Path.Combine(folderPath, "HDRPMaterialFactory.asset");

			// Create the HDRP material factory asset:
			PKFxMaterialFactoryHDRP materialFactoryHDRP = CreateInstance<PKFxMaterialFactoryHDRP>();
			materialFactoryHDRP.m_FactoryType = FactoryType.HDRP;
			materialFactoryHDRP.SetupFallBackFeatureBinding();

			AssetDatabase.CreateAsset(materialFactoryHDRP, path);
			AssetDatabase.SaveAssets();

			EditorUtility.FocusProjectWindow();

			Selection.activeObject = materialFactoryHDRP;

			return materialFactoryHDRP;
		}

#endif
		public override void SetupFallBackFeatureBinding()
		{
			//Distortion
			if (m_DistortionBillboard != null)
				m_RenderFeatureBindings.Add(m_DistortionBillboard);
			if (m_DistortionBillboard_Default != null)
				m_RenderFeatureBindings.Add(m_DistortionBillboard_Default);
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
			if (batchDesc.m_Type != ERendererType.Mesh)
				material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}

		public override GameObject GetLightTemplate()
		{
			GameObject template = new GameObject("PopcornFX Light");
			HDAdditionalLightData light = template.AddHDLight(HDLightTypeAndShape.Point);
			light.EnableColorTemperature(false);
			light.shapeRadius = 0;
			return template;
		}

		public override void SetLightValue(Light light, PKFxLightPool.SLightInfo info)
		{
			HDAdditionalLightData data = light.gameObject.GetComponent<HDAdditionalLightData>();
			
			data.SetIntensity(info.m_Intensity, UnityEngine.Rendering.LightUnit.Lux);
			data.luxAtDistance = info.m_Range;
			data.SetRange(info.m_Range);
			data.SetColor(info.m_Color * info.m_Intensity);
			light.transform.position = info.m_Position;
		}
	}
}