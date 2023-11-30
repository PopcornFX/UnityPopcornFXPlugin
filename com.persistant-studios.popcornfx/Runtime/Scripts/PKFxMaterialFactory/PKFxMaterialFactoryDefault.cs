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
			if (m_TransparentMeshUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshUnlitDefault);
			if (m_OpaqueMeshUnlitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitDefault);
			if (m_TransparentMeshLitDefault != null)
				m_RenderFeatureBindings.Add(m_TransparentMeshLitDefault);
			if (m_OpaqueMeshLitDefault != null)
				m_RenderFeatureBindings.Add(m_OpaqueMeshLitDefault);
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
			// Default shader fallback:
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
			Material material = asset.m_Materials[batchDesc.MaterialIdx];
			if (material == null)
				return null;
			// Set the render queue:
			material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}
	}
}