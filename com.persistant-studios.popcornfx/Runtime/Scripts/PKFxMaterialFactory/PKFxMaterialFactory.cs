//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
#if		UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
	[Serializable]
	public abstract class PKFxMaterialFactory : ScriptableObject
	{
		[Serializable]
		public class CustomMaterialInfo
		{
			public string		m_AssetVirtualPath;
			public string		m_BatchDescName;
			public int			m_InternalId;

			public Material					m_CustomMaterial;
			public PKFxShaderInputBindings	m_CustomMaterialBindings;
		}

		public abstract void		SetupRenderer(SBatchDesc batchDesc, GameObject gameObject, MeshRenderer meshRenderer);
		public abstract void		SetupMeshRenderer(SBatchDesc batchDesc, GameObject gameObject, PKFxMeshInstancesRenderer meshRenderer);
		public abstract Material	ResolveParticleMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null);


		public List<CustomMaterialInfo>			m_CustomMaterials = new List<CustomMaterialInfo>();

		public Shader DistortionShader;
		public Shader BlurShader;

		[HideInInspector]public PKFxRenderFeatureBinding			m_CPUBillboardingDefault;
		[HideInInspector]public PKFxRenderFeatureBinding			m_CPUBillboardingOpaqueDefault;
		[HideInInspector]public PKFxRenderFeatureBinding			m_VertexBillboardingDefault;
		[HideInInspector]public PKFxRenderFeatureBinding			m_VertexBillboardingOpaqueDefault;
		[HideInInspector]public PKFxRenderFeatureBinding			m_TransparentMeshUnlitDefault;
		[HideInInspector]public PKFxRenderFeatureBinding			m_OpaqueMeshUnlitDefault;
		[HideInInspector]public PKFxRenderFeatureBinding			m_TransparentMeshLitDefault;
		[HideInInspector]public PKFxRenderFeatureBinding			m_OpaqueMeshLitDefault;

		public List<PKFxRenderFeatureBinding>	m_RenderFeatureBindings = new List<PKFxRenderFeatureBinding>();

		public abstract void SetupFallBackFeatureBinding();

		public CustomMaterialInfo FindCustomMaterialInfo(SBatchDesc batchDesc, PKFxEffectAsset asset)
		{
			if (m_CustomMaterials == null || asset == null)
				return null;
			return m_CustomMaterials.Find(x => (asset.AssetVirtualPath == x.m_AssetVirtualPath &&
												batchDesc.m_GeneratedName == x.m_BatchDescName &&
												batchDesc.m_InternalId == x.m_InternalId));
		}

#if UNITY_EDITOR
		public static string GetSelectedPathOrFallback()
		{
			string path = "Assets";

			foreach (UnityEngine.Object obj in Selection.GetFiltered(typeof(UnityEngine.Object), SelectionMode.Assets))
			{
				path = AssetDatabase.GetAssetPath(obj);
				if (!string.IsNullOrEmpty(path) && File.Exists(path))
				{
					path = Path.GetDirectoryName(path);
					break;
				}
			}
			return path;
		}

		public void AddCustomMaterial(PKFxEffectAsset asset, SBatchDesc sBatchDesc, Material newMat, int id)
		{
			if (m_CustomMaterials == null)
				m_CustomMaterials = new List<CustomMaterialInfo>();

			CustomMaterialInfo customRule = FindCustomMaterialInfo(sBatchDesc, asset);

			if (customRule == null)
			{
				customRule = new CustomMaterialInfo();
				customRule.m_AssetVirtualPath = asset.AssetVirtualPath;
				customRule.m_BatchDescName = sBatchDesc.m_GeneratedName;
				customRule.m_InternalId = id;
				m_CustomMaterials.Add(customRule);
			}
			if (customRule != null)
			{
				customRule.m_CustomMaterial = newMat;
			}
			EditorUtility.SetDirty(this);
			AssetDatabase.SaveAssets();
		}

		public Material ResetParticleMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null)
		{
			if (batchDesc != null && asset != null)
			{
				CustomMaterialInfo customRule = FindCustomMaterialInfo(batchDesc, asset);
				if (customRule != null)
				{
					m_CustomMaterials.Remove(customRule);
				}
			}
			return ResolveParticleMaterial(batchDesc, asset);
		}
#endif

		public Material TryFindAndInstantiateCustomMaterial(PKFxEffectAsset asset, SBatchDesc batchDesc)
		{
			CustomMaterialInfo curMat = FindCustomMaterialInfo(batchDesc, asset);
			if (curMat != null)
			{
				if (curMat.m_CustomMaterial != null)
				{
					Material material = new Material(curMat.m_CustomMaterial);
					curMat.m_CustomMaterialBindings.SetMaterialKeywords(batchDesc, material);
					curMat.m_CustomMaterialBindings.BindMaterialProperties(batchDesc, material, asset);
				}
				else
				{
					Debug.LogWarning("[PopcornFX] Custom Material is missing, remember to set it in \"PKFXSettings > Material Factory > Custom Materials\"", asset);
				}
			}
			return null;
		}

		public static Texture GetTextureAsset(PKFxEffectAsset asset, string rawPath, bool isLinear, TextureWrapMode wrapMode)
		{
			Texture texture = null;

			if (rawPath != null)
			{
				rawPath = rawPath.Replace('\\', '/');
				rawPath = rawPath.Replace("//", "/");
				PKFxEffectAsset.DependencyDesc DepDesc = null;

				if (isLinear)
				{
					string path = Path.GetDirectoryName(rawPath) + "/" + Path.GetFileNameWithoutExtension(rawPath) + "_linear" + Path.GetExtension(rawPath);
					path = path.Replace('\\', '/');
					DepDesc = asset.m_Dependencies.Find(x => path.Contains(x.m_Path));
				}
				if (DepDesc == null)
				{
					DepDesc = asset.m_Dependencies.Find(x => rawPath.Contains(x.m_Path));
				}
				if (DepDesc != null)
					texture = DepDesc.m_Object as Texture;

				if (texture == null)
				{
					Debug.LogError("[PopcornFX] Error while trying to create texture. Try to reimport \"" + rawPath + "\" and \"" + asset.AssetVirtualPath + "\" check if its format is compatible with Unity.", asset);
					return null;
				}

				texture.wrapMode = wrapMode;
			}
			return texture;
		}
		protected PKFxRenderFeatureBinding ResolveBatchBinding(SBatchDesc batchDesc)
		{
			foreach (PKFxRenderFeatureBinding binding in m_RenderFeatureBindings)
			{
				if (binding != null)
				{
					if (binding.IsMatchingRendererDesc(batchDesc))
					{
						return binding;
					}
				}
			}
			Debug.LogError("[PopcornFX] Error No shader found for " + batchDesc.m_GeneratedName);
			return null;
		}

		// Replace bindings with legacy ones, if it exists.
		// Corresponding legacy bindings are resolved from path as follow:
		// "baseBindingDirectory/Legacy/version/baseBindingName"
		protected void ReplaceBindingsWithLegacy()
		{
#if UNITY_EDITOR

#if UNITY_2020 || UNITY_2019
			for (int i = 0; i < m_RenderFeatureBindings.Count; i++)
			{
				string	path = AssetDatabase.GetAssetPath(m_RenderFeatureBindings[i]);
				int		insertAt = path.LastIndexOf("/") + 1;

				{
					PKFxRenderFeatureBinding legacyBinding = AssetDatabase.LoadAssetAtPath<PKFxRenderFeatureBinding>(path.Insert(insertAt, "Legacy/2020/"));
					if (legacyBinding != null)
						m_RenderFeatureBindings[i] = legacyBinding;
				}
#if UNITY_2019
				{
					PKFxRenderFeatureBinding legacyBinding = AssetDatabase.LoadAssetAtPath<PKFxRenderFeatureBinding>(path.Insert(insertAt, "Legacy/2019/"));
					if (legacyBinding != null)
						m_RenderFeatureBindings[i] = legacyBinding;
				}
#endif
			}
#endif

#endif
		}

		protected void _SetupMeshRenderer(SBatchDesc batchDesc, GameObject gameObject, PKFxMeshInstancesRenderer meshRenderer)
		{
			if (batchDesc.m_LitFeature != null)
				meshRenderer.m_CastShadow = batchDesc.m_LitFeature.m_CastShadows;
			else
				meshRenderer.m_CastShadow = false;

			PKFxEffectAsset.DependencyDesc DepDesc = PKFxManager.GetBuiltAsset().m_Dependencies.Find(x => batchDesc.m_MeshAsset.Contains(x.m_Path));

			if (DepDesc == null && batchDesc.m_MeshAsset.EndsWith(".fbx"))
			{
				string majExtention = batchDesc.m_MeshAsset;
				majExtention = majExtention.Replace(".fbx", ".FBX");
				DepDesc = PKFxManager.GetBuiltAsset().m_Dependencies.Find(x => majExtention.Contains(x.m_Path));
			}
			if (DepDesc != null)
			{
				GameObject meshGO = DepDesc.m_Object as GameObject;
				List<Mesh> meshes = new List<Mesh>();
				List<Matrix4x4> trans = new List<Matrix4x4>();
				MeshFilter meshFilter = meshGO.GetComponent<MeshFilter>();
				if (meshFilter != null)
				{
					meshes.Add(meshFilter.sharedMesh);
					trans.Add(meshGO.transform.localToWorldMatrix);
				}
				if (meshes.Count == 0)
				{
					MeshFilter[] meshFilters = meshGO.GetComponentsInChildren<MeshFilter>();
					for (int i = 0; i < meshFilters.Length; ++i)
					{
						meshes.Add(meshFilters[i].sharedMesh);
						trans.Add(meshGO.transform.localToWorldMatrix);
					}
				}
				meshRenderer.m_MeshesImportTransform = trans.ToArray();
				meshRenderer.Meshes = meshes.ToArray();

				PKFxRenderFeatureBinding binding = ResolveBatchBinding(batchDesc);
				meshRenderer.m_ShaderVariation = batchDesc.m_ShaderVariationFlags;
				meshRenderer.m_DiffuseColorPropertyName = binding.m_MeshDiffuseColorPropertyName;
				meshRenderer.m_EmissiveColorPropertyName = binding.m_MeshEmissiveColorPropertyName;
				meshRenderer.m_AlphaRemapCursorPropertyName = binding.m_MeshAlphaCursorPropertyName;
				meshRenderer.m_VATCursorPropertyName = binding.m_MeshVATCursorPropertyName;
			}
			else
			{
				Debug.LogError("Could not find mesh '" + batchDesc.m_MeshAsset + "'");
			}
		}
	}
}