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
		public int			m_RenderQueue = 3050; // In HDRP, the objects > 3100 are not drawn

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
			m_RenderFeatureBindings.Add(m_CPUBillboardingDistortion);
			m_RenderFeatureBindings.Add(m_VertexBillboardingDistortion);
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
			Material material = asset.m_Materials[batchDesc.MaterialIdx];
			if (material == null)
				return null;
			if (batchDesc.m_Type != ERendererType.Mesh)
				material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}
	}
}