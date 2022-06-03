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
			m_RenderFeatureBindings.Add(m_CPUBillboardingDefault);
			m_RenderFeatureBindings.Add(m_CPUBillboardingOpaqueDefault);
			m_RenderFeatureBindings.Add(m_VertexBillboardingDefault);
			m_RenderFeatureBindings.Add(m_VertexBillboardingOpaqueDefault);
			m_RenderFeatureBindings.Add(m_TransparentMeshUnlitDefault);
			m_RenderFeatureBindings.Add(m_OpaqueMeshUnlitDefault);
			m_RenderFeatureBindings.Add(m_TransparentMeshLitDefault);
			m_RenderFeatureBindings.Add(m_OpaqueMeshLitDefault);
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
			Material material = GetRuntimeMaterial(asset, batchDesc);
			if (material == null)
				return null;
			// Set the render queue:
			material.renderQueue = m_RenderQueue + batchDesc.m_DrawOrder;
			return material;
		}
	}
}