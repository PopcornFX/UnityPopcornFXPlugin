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
		public bool m_UseSortingLayers = false;

		public abstract void		SetupMeshRenderer(SBatchDesc batchDesc, GameObject gameObject, PKFxMeshInstancesRenderer meshRenderer);
		public abstract Material	ResolveParticleMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null);

		[HideInInspector]
		[SerializeField]
		private List<PKFxCustomMaterialInfo>			m_CustomMaterials = new List<PKFxCustomMaterialInfo>();

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
		[HideInInspector]public PKFxRenderFeatureBinding			m_CPUBillboardingDistortion;
		[HideInInspector]public PKFxRenderFeatureBinding			m_VertexBillboardingDistortion;

		public List<PKFxRenderFeatureBinding>	m_RenderFeatureBindings = new List<PKFxRenderFeatureBinding>();

		private PKFxRenderingPlugin				m_RenderingPlugin = null;

		public abstract void SetupFallBackFeatureBinding();

		public void SetupRenderer(SBatchDesc batchDesc, GameObject gameObject, MeshRenderer meshRenderer)
		{
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
				gameObject.layer = LayerMask.NameToLayer(PKFxManagerImpl.m_DistortionLayer);
			else
			{
				int layer = 0;

				PKFxSettings.Instance.GetRenderingLayerForBatchDesc(batchDesc, out layer);
				gameObject.layer = layer;
			}
			if (m_UseSortingLayers)
			{
				meshRenderer.sortingLayerName = "PopcornFX";
			}
		}

		public PKFxCustomMaterialInfo FindCustomMaterialInfo(SBatchDesc batchDesc, PKFxEffectAsset asset)
		{
			if (m_CustomMaterials == null || asset == null)
				return null;
			return m_CustomMaterials.Find(x =>	(x != null &&
												asset.AssetVirtualPath == x.m_AssetVirtualPath &&
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

		public void DrawEditorCustomMaterialList()
		{
			m_CustomMaterials.RemoveAll(delegate (PKFxCustomMaterialInfo a) { return a == null; });
			List<PKFxCustomMaterialInfo> materials = new List<PKFxCustomMaterialInfo>(m_CustomMaterials);
			materials.Sort(delegate (PKFxCustomMaterialInfo a, PKFxCustomMaterialInfo b)
			{
				int assetPathDiff = a.m_AssetVirtualPath.CompareTo(b.m_AssetVirtualPath);
				if (assetPathDiff == 0)
					return a.m_InternalId - b.m_InternalId;
				return assetPathDiff;
			});
			int materialIdx = 0;
			while (materialIdx < materials.Count)
			{
				bool showObjectFieldFX = true;
				string assetVirtualPath = materials[materialIdx].m_AssetVirtualPath;

				GUILayout.Space(15);
				Rect rect = EditorGUILayout.GetControlRect(false, 1);
				rect.height = 1;
				EditorGUI.DrawRect(rect, new Color(0.2f, 0.2f, 0.2f, 1.0f));
				GUILayout.Space(15);
				while (	materialIdx < materials.Count &&
						materials[materialIdx].m_AssetVirtualPath == assetVirtualPath)
				{
					PKFxCustomMaterialInfo info = materials[materialIdx];
					DrawEditorCustomMaterial(info, showObjectFieldFX);
					showObjectFieldFX = false;
					++materialIdx;
				}
			}
			EditorGUI.EndDisabledGroup();
		}

		public static void DrawEditorCustomMaterial(PKFxCustomMaterialInfo customMat, bool showFx)
		{
			EditorGUI.BeginDisabledGroup(true);
			string assetVirtualPath = customMat.m_AssetVirtualPath;
			string assetFullPath = "Assets" + PKFxSettings.UnityPackFxPath + "/" + assetVirtualPath + ".asset";
			PKFxEffectAsset curAsset = (PKFxEffectAsset)AssetDatabase.LoadAssetAtPath(assetFullPath, typeof(PKFxEffectAsset));
			if (showFx)
			{
				EditorGUILayout.BeginHorizontal();
				EditorGUILayout.LabelField("Effect Asset");
				EditorGUILayout.ObjectField(curAsset, typeof(PKFxEffectAsset), false);
				EditorGUILayout.EndHorizontal();
			}
			EditorGUILayout.BeginHorizontal();
			EditorGUILayout.LabelField("Renderer #" + customMat.m_InternalId + " Material");
			EditorGUILayout.ObjectField(customMat.m_CustomMaterial, typeof(Material), false);
			EditorGUILayout.EndHorizontal();
			EditorGUI.EndDisabledGroup();
			SBatchDesc customMatBatchDesc = curAsset.m_RendererDescs.Find(delegate (SBatchDesc desc)
			{
				return desc.m_GeneratedName == customMat.m_BatchDescName &&
						desc.m_InternalId == customMat.m_InternalId;
			});
			EditorGUI.indentLevel += 1;
			customMat.DrawEditorShaderInputBindings(customMatBatchDesc);
			EditorGUI.indentLevel -= 1;
		}

		public void AddCustomMaterial(PKFxEffectAsset asset, SBatchDesc sBatchDesc, Material newMat, int id)
		{
			if (m_CustomMaterials == null)
				m_CustomMaterials = new List<PKFxCustomMaterialInfo>();

			PKFxCustomMaterialInfo customRule = FindCustomMaterialInfo(sBatchDesc, asset);

			if (customRule == null)
			{
				customRule = ScriptableObject.CreateInstance<PKFxCustomMaterialInfo>();
				customRule.m_AssetVirtualPath = asset.AssetVirtualPath;
				customRule.m_BatchDescName = sBatchDesc.m_GeneratedName;
				customRule.m_InternalId = id;
				string assetName = asset.AssetVirtualPath + " [" + id + "]";
				customRule.name = assetName;
				AssetDatabase.AddObjectToAsset(customRule, this);
				m_CustomMaterials.Add(customRule);
				asset.m_Materials[id] = newMat;
				AssetDatabase.SaveAssets();
			}
			if (customRule != null)
			{
				customRule.m_CustomMaterial = newMat;
				asset.m_Materials[id] = newMat;
				AssetDatabase.SaveAssets();
			}
		}

		public void ResetParticleMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null)
		{
			if (batchDesc != null && asset != null)
			{
				PKFxCustomMaterialInfo customRule = FindCustomMaterialInfo(batchDesc, asset);
				if (customRule != null)
				{
					AssetDatabase.RemoveObjectFromAsset(customRule);
					m_CustomMaterials.Remove(customRule);
				}
			}
			asset.m_Materials[batchDesc.MaterialIdx] = EditorResolveMaterial(batchDesc, asset);
			AssetDatabase.SaveAssets();
		}

		public void ResetAllCustomMaterials()
		{
			foreach (PKFxCustomMaterialInfo info in m_CustomMaterials)
			{
				PKFxEffectAsset asset = AssetDatabase.LoadAssetAtPath("Assets/" + PKFxSettings.UnityPackFxPath + "/" + info.m_AssetVirtualPath, typeof(PKFxEffectAsset)) as PKFxEffectAsset;
				asset.m_Materials[info.m_InternalId] = EditorGetDefaultMaterial(asset.m_RendererDescs[info.m_InternalId], asset);
				AssetDatabase.RemoveObjectFromAsset(info);
			}
			AssetDatabase.SaveAssets();
			m_CustomMaterials.Clear();
		}

		public virtual Material EditorGetDefaultMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null)
		{
			PKFxRenderFeatureBinding binding = ResolveBatchBinding(batchDesc);
			Material material;
			if (binding == null)
			{
				Debug.LogError("[PopcornFX] Error No shader found for " + batchDesc.m_GeneratedName + "in effect: " + asset.name);
				return null;
			}

			Material assetMat = (Material)AssetDatabase.LoadAssetAtPath("Assets/" + PKFxSettings.UnityPackFxPath + "/UnityMaterials/" + batchDesc.PathGeneratedName + ".mat", typeof(Material));
			if (assetMat == null)
			{
				if (binding.m_UseShader)
					material = new Material(binding.m_Shader);
				else
					material = new Material(binding.m_Material);
				binding.SetMaterialKeywords(batchDesc, material);
				binding.BindMaterialProperties(batchDesc, material, asset);
				AssetDatabase.CreateAsset(material, "Assets" + PKFxSettings.UnityPackFxPath + "/UnityMaterials/" + batchDesc.PathGeneratedName + ".mat");
				AssetDatabase.SaveAssets();
			}
			else
				material = assetMat;
			return material;
		}
		public virtual Material EditorResolveMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null)
		{
			if (!AssetDatabase.IsValidFolder("Assets" + PKFxSettings.UnityPackFxPath + "/UnityMaterials"))
				AssetDatabase.CreateFolder("Assets" + PKFxSettings.UnityPackFxPath, "UnityMaterials");
			PKFxCustomMaterialInfo curMat = FindCustomMaterialInfo(batchDesc, asset);
			if (curMat != null && curMat.m_CustomMaterial != null)
				return curMat.m_CustomMaterial;
			else
			{
				return EditorGetDefaultMaterial(batchDesc, asset);
			}
		}

		public virtual PKFxShaderInputBindings EditorResolveBindings(SBatchDesc batchDesc, PKFxEffectAsset asset = null)
		{
			PKFxCustomMaterialInfo curMat = FindCustomMaterialInfo(batchDesc, asset);
			if (curMat != null && curMat.m_CustomMaterial != null)
			{
				return curMat;
			}
			else
			{
				PKFxRenderFeatureBinding binding = ResolveBatchBinding(batchDesc);
				return binding;
			}
		}

#endif
		
		public PKFxRenderFeatureBinding ResolveBatchBinding(SBatchDesc batchDesc)
		{
			if (m_RenderingPlugin == null)
			{
				PKFxRenderingPlugin[] rendering = FindObjectsOfType<PKFxRenderingPlugin>();
				if (rendering.Length != 0)
					m_RenderingPlugin = rendering[0];
			}
			bool noDistortion = false;
			if (m_RenderingPlugin != null)
				noDistortion = !m_RenderingPlugin.m_EnableBlur && !m_RenderingPlugin.m_EnableDistortion;
			foreach (PKFxRenderFeatureBinding binding in m_RenderFeatureBindings)
			{
				if (binding != null)
				{
					if (binding.IsMatchingRendererDesc(batchDesc, noDistortion))
						return binding;
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
				{
#if UNITY_EDITOR
					if (DepDesc.m_Object == null)
						DepDesc.m_Object = AssetDatabase.LoadAssetAtPath("Assets" + PKFxSettings.UnityPackFxPath + "/" + rawPath, typeof(Texture));
#endif
					texture = DepDesc.m_Object as Texture;
				}

				if (texture == null)
					return null;

				texture.wrapMode = wrapMode;
			}
			return texture;
		}

		protected PKFxShaderInputBindings GetRuntimeShaderInputBindings(SBatchDesc batchDesc, PKFxEffectAsset asset = null)
		{
			PKFxCustomMaterialInfo curMat = FindCustomMaterialInfo(batchDesc, asset);
			if (curMat != null && curMat.m_CustomMaterial != null)
			{
				return curMat;
			}
			else
			{
				return ResolveBatchBinding(batchDesc);
			}
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

				PKFxShaderInputBindings binding = GetRuntimeShaderInputBindings(batchDesc, PKFxManager.GetBuiltAsset());
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