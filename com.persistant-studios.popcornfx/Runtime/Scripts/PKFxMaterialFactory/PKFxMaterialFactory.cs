//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using Unity.Jobs;
using Unity.Collections;
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
	[Serializable]
	public abstract class PKFxMaterialFactory : ScriptableObject
	{
		public enum FactoryType : int
		{
			DEFAULT,
			URP,
			HDRP,
			SRP,
		}

		public bool m_UseSortingLayers = false;
		public FactoryType m_FactoryType = FactoryType.DEFAULT;
		public abstract void		SetupMeshRenderer(SBatchDesc batchDesc, GameObject gameObject, PKFxMeshInstancesRenderer meshRenderer);
		public abstract Material	ResolveParticleMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null);

		public Shader DistortionShader;
		public Shader BlurShader;

		[HideInInspector] public PKFxRenderFeatureBinding			m_TransparentMeshUnlitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueMeshUnlitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_TransparentMeshLitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueMeshLitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueMeshLitSkinnedDefault;

		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueParticleUnlitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_TransparentParticleUnlitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_AdditiveParticleUnlitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueCorrectDeformationUnlitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_TransparentCorrectDeformationUnlitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_AdditiveCorrectDeformationUnlitDefault;

		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueParticleLitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_TransparentParticleLitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_AdditiveParticleLitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_OpaqueCorrectDeformationLitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_TransparentCorrectDeformationLitDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_AdditiveCorrectDeformationLitDefault;

		[HideInInspector] public PKFxRenderFeatureBinding			m_CPUParticleDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_CPUParticleOpaqueDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_VertexBillboardingDefault;
		[HideInInspector] public PKFxRenderFeatureBinding			m_VertexBillboardingOpaqueDefault;

		[HideInInspector]public PKFxRenderFeatureBinding			m_CPUParticleDistortion;
		[HideInInspector]public PKFxRenderFeatureBinding			m_VertexBillboardingDistortion;

		public List<PKFxRenderFeatureBinding>	m_RenderFeatureBindings = new List<PKFxRenderFeatureBinding>();

		public abstract void SetupFallBackFeatureBinding();

		public void SetupRenderer(SBatchDesc batchDesc, GameObject gameObject, MeshRenderer meshRenderer)
		{
			if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
				gameObject.layer = LayerMask.NameToLayer(PKFxManagerImpl.m_DistortionLayer);
			else
			{
				int layer;

				PKFxSettings.Instance.GetRenderingLayerForBatchDesc(batchDesc, out layer);
				if (layer != -1)
					gameObject.layer = layer;
			}
			if (batchDesc.m_Type != ERendererType.Mesh)
				meshRenderer.shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.Off; // Disable shadow casting on particles for now
			if (m_UseSortingLayers)
			{
				meshRenderer.sortingLayerName = "PopcornFX";
			}
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

		public string EditorResolveMaterialName(SBatchDesc batchDesc)
		{
			if (PKFxSettings.UseHashesAsMaterialName)
			{
				Hash128 hash = new Hash128();
				hash.Append(batchDesc.m_GeneratedName);
				return hash.ToString();
			}
			else
				return batchDesc.GenerateShortNameFromDescription();
		}

		public Material EditorResolveMaterial(SBatchDesc batchDesc, PKFxEffectAsset asset = null, bool reimport = false, bool resolveCustomMat = false, bool logError = true)
		{
			if (batchDesc.m_Type == ERendererType.Light || batchDesc.m_Type == ERendererType.Sound)
				return null;
			
			if (!AssetDatabase.IsValidFolder("Assets" + PKFxSettings.UnityPackFxPath))
				AssetDatabase.CreateFolder("Assets", PKFxSettings.UnityPackFxPath.Substring(1));
			if (!AssetDatabase.IsValidFolder("Assets" + PKFxSettings.UnityPackFxPath + "/UnityMaterials"))
				AssetDatabase.CreateFolder("Assets" + PKFxSettings.UnityPackFxPath, "UnityMaterials");

			PKFxRenderFeatureBinding binding = ResolveBatchBinding(batchDesc);
			Material material = null;

			if (binding == null)
			{
				Debug.LogError("[PopcornFX] Error No shader found for " + batchDesc.m_GeneratedName + "in effect: " + asset.name);
				return null;
			}

			string matName = EditorResolveMaterialName(batchDesc);
			string materialPath = "Assets/" + PKFxSettings.UnityPackFxPath + "/UnityMaterials/" + matName + ".mat";
			Material assetMat = (Material)AssetDatabase.LoadAssetAtPath(materialPath, typeof(Material));

			if (reimport && assetMat != null)
			{
				assetMat = null;
				AssetDatabase.DeleteAsset(materialPath);
			}
			if (assetMat == null)
			{
				material = new Material(binding.m_Shader);
				if (batchDesc.m_Type == ERendererType.Mesh)
					material.enableInstancing = true;
				binding.SetMaterialKeywords(batchDesc, material);
				binding.BindMaterialProperties(batchDesc, material, asset, logError);
				AssetDatabase.CreateAsset(material, "Assets" + PKFxSettings.UnityPackFxPath + "/UnityMaterials/" + matName + ".mat");
			}
			if (resolveCustomMat)
			{
				PKFxCustomMaterialInfo customMatInfo = asset.FindCustomMaterialInfo(batchDesc);
				if (customMatInfo != null)
					material = customMatInfo.m_CustomMaterial;
			}
			if (material == null)
			{
				material = assetMat;
				binding.SetMaterialKeywords(batchDesc, material);
				binding.BindMaterialProperties(batchDesc, material, asset);
			}
			AssetDatabase.SaveAssets();
			return material;
		}
#endif
		
		public PKFxRenderFeatureBinding ResolveBatchBinding(SBatchDesc batchDesc)
		{
			foreach (PKFxRenderFeatureBinding binding in m_RenderFeatureBindings)
			{
				if (binding != null)
				{
					if (binding.IsMatchingRendererDesc(batchDesc))
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
						DepDesc.m_Object = AssetDatabase.LoadAssetAtPath("Assets" + PKFxSettings.UnityPackFxPath + "/" + DepDesc.m_Path, typeof(Texture));
#endif
					texture = DepDesc.m_Object as Texture;
				}

				if (texture == null)
					return null;

				texture.wrapMode = wrapMode;
			}
			return texture;
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

		private struct SetupMeshBoneInfluences : IJobParallelFor
		{
			public NativeArray<Vector4>		m_OutBoneWeights;
			public NativeArray<Vector4>		m_OutBoneIndices;

			[ReadOnly]
			public NativeArray<BoneWeight>	m_InBoneWeights;
			[ReadOnly]
			public NativeArray<int>			m_ReorderBones;

			public void Execute(int h)
			{
				m_OutBoneWeights[h] = new Vector4(	m_InBoneWeights[h].weight0,
													m_InBoneWeights[h].weight1,
													m_InBoneWeights[h].weight2,
													m_InBoneWeights[h].weight3);
				m_OutBoneIndices[h] = new Vector4(	m_ReorderBones[m_InBoneWeights[h].boneIndex0],
													m_ReorderBones[m_InBoneWeights[h].boneIndex1],
													m_ReorderBones[m_InBoneWeights[h].boneIndex2],
													m_ReorderBones[m_InBoneWeights[h].boneIndex3]);
			}
		}

		private bool	_RetrieveBoneWeights(Mesh mesh, int[] reorderBones, out Vector4[] weights, out Vector4[] indices)
		{
			// Get the number of bone weights per vertex
			var bonesPerVertex = mesh.GetBonesPerVertex();
			if (bonesPerVertex.Length == 0)
			{
				weights = null;
				indices = null;
				return false;
			}

			Vector4[]		vBoneWeights = new Vector4[mesh.vertexCount];
			Vector4[]		vBoneIndices = new Vector4[mesh.vertexCount];

			BoneWeight[]	boneWeights = mesh.boneWeights;

			SetupMeshBoneInfluences job = new SetupMeshBoneInfluences();

			job.m_OutBoneIndices = new NativeArray<Vector4>(mesh.boneWeights.Length, Allocator.Persistent);
			job.m_OutBoneWeights = new NativeArray<Vector4>(mesh.boneWeights.Length, Allocator.Persistent);
			job.m_InBoneWeights = new NativeArray<BoneWeight>(mesh.boneWeights.Length, Allocator.TempJob);
			job.m_InBoneWeights.CopyFrom(mesh.boneWeights);
			job.m_ReorderBones = new NativeArray<int>(reorderBones.Length, Allocator.TempJob);
			job.m_ReorderBones.CopyFrom(reorderBones);
			
			Unity.Jobs.JobHandle handle = job.Schedule(mesh.boneWeights.Length, 64);
			handle.Complete();

			weights = job.m_OutBoneWeights.ToArray();
			indices = job.m_OutBoneIndices.ToArray();

			job.m_OutBoneWeights.Dispose();
			job.m_OutBoneIndices.Dispose();
			job.m_InBoneWeights.Dispose();
			job.m_ReorderBones.Dispose();
			return true;
		}

		protected int	_RecursiveGetBoneIdx(Transform boneToFind, Transform currentBoneLevel, Transform[] allBones, ref int currentIdx)
		{
			if (currentBoneLevel == boneToFind)
				return currentIdx;

			foreach (Transform cmp in allBones)
			{
				if (cmp == currentBoneLevel)
				{
					++currentIdx;
					break;
				}
			}

			List<Transform>		childs = new List<Transform>(currentBoneLevel.childCount);

			for (int childIdx = 0; childIdx < currentBoneLevel.childCount; ++childIdx)
			{
				Transform	currentChild = currentBoneLevel.GetChild(childIdx);
				bool		hasBeenInserted = false;

				for (int i = 0; i < childs.Count && !hasBeenInserted; ++i)
				{
					if (childs[i].name.CompareTo(currentChild.name) > 0)
					{
						childs.Insert(i, currentChild);
						hasBeenInserted = true;
					}
				}
				if (!hasBeenInserted)
					childs.Add(currentChild);
			}
			Debug.Assert(currentBoneLevel.childCount == childs.Count);
			for (int childIdx = 0; childIdx < childs.Count; ++childIdx)
			{
				Transform	currentChild = childs[childIdx];
				int		foundBone = _RecursiveGetBoneIdx(boneToFind, currentChild, allBones, ref currentIdx);
				if (foundBone != -1)
					return foundBone;
			}
			return -1;
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
			if (DepDesc != null && DepDesc.m_Object != null)
			{
				GameObject									meshGO = DepDesc.m_Object as GameObject;

				if (meshGO == null)
					return;

				LODGroup									lodGroup = meshGO.GetComponent<LODGroup>();
				List<Renderer>								renderers = new List<Renderer>();
				List<PKFxMeshInstancesRenderer.MeshToDraw>	meshes = new List<PKFxMeshInstancesRenderer.MeshToDraw>();
				List<SkinnedMeshRenderer>					skinnedMeshes = new List<SkinnedMeshRenderer>();
				List<Matrix4x4>								trans = new List<Matrix4x4>();

				if (lodGroup != null)
				{
					int		lodIdx = 0;
					meshRenderer.m_PerLODsSubmeshCount = new int[lodGroup.GetLODs().Length];
					foreach (LOD lod in lodGroup.GetLODs())
					{
						meshRenderer.m_PerLODsSubmeshCount[lodIdx] = lod.renderers.Length;
						foreach (Renderer renderer in lod.renderers)
						{
							if (renderers != null)
								renderers.Add(renderer);
						}
						++lodIdx;
					}
				}
				else
				{
					meshRenderer.m_PerLODsSubmeshCount = new int[1];
					Renderer[]		foundRenderers = meshGO.GetComponentsInChildren<Renderer>();
					meshRenderer.m_PerLODsSubmeshCount[0] = foundRenderers.Length;
					foreach (Renderer rdr in foundRenderers)
						renderers.Add(rdr);
				}

				int lodCurCount = 0;
				int subMeshIdx = 0;
				int lodCurIdx = 0;

				Transform rootTr = null;
				Transform[] bones = null;

				foreach (Renderer rdr in renderers)
				{
					if (lodCurCount == meshRenderer.m_PerLODsSubmeshCount[lodCurIdx])
					{
						lodCurCount = 0;
						// Update the submesh count to take into account the sharedMesh.subMeshCount:
						meshRenderer.m_PerLODsSubmeshCount[lodCurIdx] = subMeshIdx;
						subMeshIdx = 0;
						++lodCurIdx;
					}
					MeshRenderer		unityMeshRdr = rdr as MeshRenderer;
					SkinnedMeshRenderer	unitySkinnedMeshRdr = rdr as SkinnedMeshRenderer;
					Mesh				sharedMesh = null;

					if (unityMeshRdr != null)
					{
						MeshFilter	meshFilter = unityMeshRdr.gameObject.GetComponent<MeshFilter>();
						sharedMesh = meshFilter.sharedMesh;
					}
					else if (unitySkinnedMeshRdr)
					{
						sharedMesh = unitySkinnedMeshRdr.sharedMesh;
						if (batchDesc.HasShaderVariationFlag(EShaderVariationFlags.Has_SkeletalAnim))
						{
							Mesh skinnedMesh = new Mesh();

							if (bones == null)
							{
								bones = unitySkinnedMeshRdr.bones;
								rootTr = unitySkinnedMeshRdr.rootBone;
								while (rootTr.parent != null)
									rootTr = rootTr.parent;
							}

							int[]	reorderBones = new int[unitySkinnedMeshRdr.bones.Length];

							for (int boneIdx = 0; boneIdx < unitySkinnedMeshRdr.bones.Length; ++boneIdx)
							{
								int currentIdx = 0;
								reorderBones[boneIdx] = _RecursiveGetBoneIdx(unitySkinnedMeshRdr.bones[boneIdx], rootTr, bones, ref currentIdx);
							}
							for (int boneIdx = 0; boneIdx < unitySkinnedMeshRdr.bones.Length; ++boneIdx)
							{
								for (int boneIdx2 = 0; boneIdx2 < unitySkinnedMeshRdr.bones.Length; ++boneIdx2)
								{
									int realIdx = reorderBones[boneIdx2];
									if (realIdx == boneIdx)
									{
										Debug.Log("Bone " + realIdx + " is " + unitySkinnedMeshRdr.bones[boneIdx2].name);
										break;
									}
								}
							}

							skinnedMesh.vertices = sharedMesh.vertices;
							skinnedMesh.triangles = sharedMesh.triangles;
							skinnedMesh.bounds = sharedMesh.bounds;
							skinnedMesh.colors = sharedMesh.colors;
							skinnedMesh.normals = sharedMesh.normals;
							skinnedMesh.tangents = sharedMesh.tangents;
							skinnedMesh.uv = sharedMesh.uv;
							skinnedMesh.uv2 = sharedMesh.uv2;
							Vector4[] boneWeights;
							Vector4[] boneIndices;
							_RetrieveBoneWeights(sharedMesh, reorderBones, out boneWeights, out boneIndices);
							skinnedMesh.SetUVs(2, boneWeights);
							skinnedMesh.SetUVs(3, boneIndices);
							// For skinned meshes, the import transforms are applied in the shader
							// so we need to pre-scale the bounds:
							Bounds bounds = skinnedMesh.bounds;
							bounds.Expand(rdr.gameObject.transform.localToWorldMatrix.lossyScale);
							skinnedMesh.bounds = bounds;
							sharedMesh = skinnedMesh;
						}
					}

					for (int subMesh = 0; subMesh < sharedMesh.subMeshCount; ++subMesh)
					{
						meshes.Add(new PKFxMeshInstancesRenderer.MeshToDraw(sharedMesh,
																			subMesh,
																			rdr.gameObject.transform.localToWorldMatrix));
						++subMeshIdx;
					}
					++lodCurCount;
				}

				meshRenderer.m_Material.SetTexture(meshRenderer.m_TextureAtlasPropertyName, batchDesc.m_AtlasSubRects);

#if false
				if (meshFilter != null)
				{
					// Has mesh filter at root:
					meshes.Add(meshFilter.sharedMesh);
					trans.Add(meshGO.transform.localToWorldMatrix);
				}
				if (meshes.Count == 0)
				{
					// Has mesh filter in childrens:
					MeshFilter[] meshFilters = meshGO.GetComponentsInChildren<MeshFilter>();
					for (int i = 0; i < meshFilters.Length; ++i)
					{
						meshes.Add(meshFilters[i].sharedMesh);
						trans.Add(meshGO.transform.localToWorldMatrix);
					}
				}
				if (meshes.Count == 0)
				{
					// Has skinned mesh renderer in childrens:
					SkinnedMeshRenderer[] skinnedMeshRenderers = meshGO.GetComponentsInChildren<SkinnedMeshRenderer>();
					for (int i = 0; i < skinnedMeshRenderers.Length; ++i)
					{
						meshes.Add(skinnedMeshRenderers[i].sharedMesh);
						trans.Add(skinnedMeshRenderers[i].transform.localToWorldMatrix);
						skinnedMeshes.Add(skinnedMeshRenderers[i]);

						int bonesOffset = 1;
						for (GameObject curGo = skinnedMeshRenderers[i].gameObject; curGo != meshGO; curGo = curGo.transform.parent.gameObject)
							++bonesOffset;
					}
				}
				meshRenderer.m_MeshesImportTransform = trans.ToArray();
#endif
				meshRenderer.Meshes = meshes.ToArray();

				PKFxShaderInputBindings binding = PKFxManager.GetBuiltAsset().GetRuntimeShaderInputBindings(batchDesc);
				meshRenderer.m_ShaderVariation = batchDesc.m_ShaderVariationFlags;
				meshRenderer.m_DiffuseColorPropertyName = binding.m_MeshDiffuseColorPropertyName;
				meshRenderer.m_EmissiveColorPropertyName = binding.m_MeshEmissiveColorPropertyName;
				meshRenderer.m_AlphaRemapCursorPropertyName = binding.m_MeshAlphaCursorPropertyName;
				meshRenderer.m_VATCursorPropertyName = binding.m_MeshVATCursorPropertyName;
				meshRenderer.m_AtlasIdPropertyName = binding.m_AtlasIdPropertyName;
				meshRenderer.m_TextureAtlasPropertyName = binding.m_TextureAtlasPropertyName;
				meshRenderer.m_SkeletalAnimCursor0PropertyName = binding.m_SkeletalAnimCursor0PropertyName;
				meshRenderer.m_SkeletalAnimCursor1PropertyName = binding.m_SkeletalAnimCursor1PropertyName;
				meshRenderer.m_SkeletalAnimIdx0PropertyName = binding.m_SkeletalAnimIdx0PropertyName;
				meshRenderer.m_SkeletalAnimIdx1PropertyName = binding.m_SkeletalAnimIdx1PropertyName;
				meshRenderer.m_SkeletalAnimTransitionPropertyName = binding.m_SkeletalAnimTransitionPropertyName;
				meshRenderer.m_SkeletalMeshTransformRow0 = binding.m_MeshTransformRow0PropertyName;
				meshRenderer.m_SkeletalMeshTransformRow1 = binding.m_MeshTransformRow1PropertyName;
				meshRenderer.m_SkeletalMeshTransformRow2 = binding.m_MeshTransformRow2PropertyName;
				meshRenderer.m_SkeletalMeshTransformRow3 = binding.m_MeshTransformRow3PropertyName;
			}
			else
			{
				Debug.LogError("Could not find mesh '" + batchDesc.m_MeshAsset + "'");
			}
		}

		public virtual GameObject GetLightTemplate()
		{
			GameObject template = new GameObject("PopcornFX Light");
			Light light = template.AddComponent<Light>();

			light.bounceIntensity = 0;
			return template;
		}
		public virtual void SetLightValue(Light light, PKFxLightPool.SLightInfo info)
		{
			light.range = info.m_Range;
			light.color = info.m_Color;
			light.intensity = info.m_Intensity;
			light.transform.position = info.m_Position;
		}
	}
}