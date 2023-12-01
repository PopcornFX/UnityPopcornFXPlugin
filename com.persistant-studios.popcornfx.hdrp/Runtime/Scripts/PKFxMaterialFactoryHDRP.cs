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
	public class PKFxMaterialFactoryHDRPUtils
	{
		static PKFxMaterialFactoryHDRPUtils()
		{
			if (PKFxSettings.CheckRenderPipeline() == PKFxSettings.ERenderPipeline.HDRP && PKFxSettings.MaterialFactory.GetType() != typeof(PKFxMaterialFactoryHDRP))
			{
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
			// Fallback default:
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
			
			data.SetIntensity(info.m_Intensity, LightUnit.Lux);
			data.luxAtDistance = info.m_Range;
			data.SetRange(info.m_Range);
			data.SetColor(info.m_Color * info.m_Intensity);
			light.transform.position = info.m_Position;
		}
	}
}