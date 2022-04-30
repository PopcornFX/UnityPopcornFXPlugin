//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace PopcornFX
{
	public partial class PKFxEmitter : MonoBehaviour
	{
		private void SetPlayingState(bool isPlaying)
		{
			if (!m_TriggerAndForget && isPlaying != m_IsPlaying)
			{
				if (isPlaying)
				{
					if (!g_PlayingEffectsToUpdate.Contains(this))
					{
						g_PlayingEffectsToUpdate.Add(this);
					}
				}
				else
				{
					g_PlayingEffectsToUpdate.Remove(this);
				}
			}
			m_IsPlaying = isPlaying;
			m_IsStopped = !isPlaying;
		}

		private void LoadEvents(List<SEventDesc> eventFromPkfxFile, bool resetAllToDefault)
		{
			if (resetAllToDefault)
				m_Events.Clear();
			List<PkFxUserEventCallback> newEvents = new List<PkFxUserEventCallback>();
			foreach (SEventDesc desc in eventFromPkfxFile)
			{
				PkFxUserEventCallback existingEvent = m_Events.Find(
					x => x.m_Desc.ToString().Equals(desc.ToString())
					);

				if (existingEvent != null)
					newEvents.Add(existingEvent);
				else
					newEvents.Add(new PkFxUserEventCallback(desc));
			}
			m_Events = newEvents;
		}

		private void LoadAttributes(List<PKFxEffectAsset.AttributeDesc> attributesFromPkfxFile, bool resetAllToDefault)
		{
			SGenericTypedValueContainer[] attributesValues = new SGenericTypedValueContainer[attributesFromPkfxFile.Count * 4]; // 4 values per attribute
			int currentAttributeId = 0;

			foreach (PKFxEffectAsset.AttributeDesc desc in attributesFromPkfxFile)
			{
				bool attribIsSet = false;

				if (!resetAllToDefault && m_FxAttributesStartValues != null && m_FxAttributesDesc != null)
				{
					int attribId = GetAttributeIdFromDesc(desc);

					if (attribId != -1)
					{
						attributesValues[currentAttributeId * 4 + 0] = m_FxAttributesStartValues[attribId * 4 + 0];
						attributesValues[currentAttributeId * 4 + 1] = m_FxAttributesStartValues[attribId * 4 + 1];
						attributesValues[currentAttributeId * 4 + 2] = m_FxAttributesStartValues[attribId * 4 + 2];
						attributesValues[currentAttributeId * 4 + 3] = m_FxAttributesStartValues[attribId * 4 + 3];
						attribIsSet = true;
					}
				}
				if (!attribIsSet)
				{
					SAttribContainer_Vector4 clampedAttribute = desc.GetDefaultAttributeValueClamped();

					attributesValues[currentAttributeId * 4 + 0].f1 = clampedAttribute.x.f1;
					attributesValues[currentAttributeId * 4 + 1].f1 = clampedAttribute.y.f1;
					attributesValues[currentAttributeId * 4 + 2].f1 = clampedAttribute.z.f1;
					attributesValues[currentAttributeId * 4 + 3].f1 = clampedAttribute.w.f1;
				}
				++currentAttributeId;
			}
			m_FxAttributesDesc = attributesFromPkfxFile;
			m_FxAttributesStartValues = attributesValues;
		}
		
		private int GetAttributeIdFromDesc(PKFxEffectAsset.AttributeDesc desc)
		{
			int attribId = 0;

			foreach (PKFxEffectAsset.AttributeDesc curDesc in m_FxAttributesDesc)
			{
				if (curDesc.m_Name == desc.m_Name && curDesc.m_Type == desc.m_Type)
					return attribId;
				++attribId;
			}
			return -1;
		}

		// Mesh sampler:
		//----------------------------------------------------------------------------

		static public IntPtr GetMeshToFill(Mesh mesh, int samplingChannels, bool skinning)
		{
			int vertexCount = mesh.vertexCount;
			int indexCount = mesh.triangles.Length;
			int bonesCount = skinning ? mesh.bindposes.Length : 0;

			if (mesh.vertices.Length != vertexCount)
			{
				Debug.LogError("[PopcornFX] Check on the mesh import settings that the 'Read/Write Enabled' option is checked");
				return IntPtr.Zero;
			}
			if ((samplingChannels & (int)SamplerDesc.ESamplerUsageFlags.UsageFlags_Mesh_Normal) != 0)
			{
				if (mesh.normals.Length != vertexCount)
				{
					samplingChannels ^= (int)SamplerDesc.ESamplerUsageFlags.UsageFlags_Mesh_Normal;
					Debug.LogWarning("[PopcornFX] asks for EMeshSamplingInfo.Info_ChannelNormal when the mesh doesn't have normals");
				}
			}
			if ((samplingChannels & (int)SamplerDesc.ESamplerUsageFlags.UsageFlags_Mesh_Tangent) != 0)
			{
				if (mesh.tangents.Length != vertexCount)
				{
					samplingChannels ^= (int)SamplerDesc.ESamplerUsageFlags.UsageFlags_Mesh_Tangent;
					Debug.LogWarning("[PopcornFX] asks for EMeshSamplingInfo.Info_ChannelTangent when the mesh doesn't have tangents");
				}
			}
			if ((samplingChannels & (int)SamplerDesc.ESamplerUsageFlags.UsageFlags_Mesh_UV0) != 0)
			{
				if (mesh.uv.Length != vertexCount)
				{
					samplingChannels ^= (int)SamplerDesc.ESamplerUsageFlags.UsageFlags_Mesh_UV0;
					Debug.LogWarning("[PopcornFX] asks for EMeshSamplingInfo.Info_ChannelUV when the mesh doesn't have UVs");
				}
			}
			if ((samplingChannels & (int)SamplerDesc.ESamplerUsageFlags.UsageFlags_Mesh_Color0) != 0)
			{
				if (mesh.colors.Length != vertexCount)
				{
					samplingChannels ^= (int)SamplerDesc.ESamplerUsageFlags.UsageFlags_Mesh_Color0;
					Debug.LogWarning("[PopcornFX] asks for EMeshSamplingInfo.Info_ChannelVertexColor when the mesh doesn't have vertex color");
				}
			}
			return PKFxManager.GetMeshSamplerToFill(vertexCount, indexCount, bonesCount, samplingChannels);
		}

		//----------------------------------------------------------------------------

		static public bool UpdateMeshToFill(IntPtr meshToFillPtr, Mesh mesh)
		{
			int[] trianglesSrc = mesh.triangles;
			int vertexCount = mesh.vertexCount;

			unsafe
			{
				SMeshDataToFill* meshToFill = (SMeshDataToFill*)meshToFillPtr.ToPointer();

				if (meshToFill == null)
				{
					Debug.LogError("[PopcornFX] Could not create the mesh data");
					return false;
				}
				else if (meshToFill->m_IdxCount != trianglesSrc.Length ||
							meshToFill->m_VtxCount != vertexCount)
				{
					Debug.LogError("[PopcornFX] Index count and vertex count does not match with the allocated mesh");
					return false;
				}

				Vector3	*positions = (Vector3*)meshToFill->m_Positions.ToPointer();
				Vector3	*normals = (Vector3*)meshToFill->m_Normals.ToPointer();
				Vector4	*tangents = (Vector4*)meshToFill->m_Tangents.ToPointer();
				Vector2	*uvs = (Vector2*)meshToFill->m_UVs.ToPointer();
				Vector4	*colors = (Vector4*)meshToFill->m_Colors.ToPointer();
				float	*bonesWeights = (float*)meshToFill->m_VtxBonesWeights.ToPointer();
				int		*bonesIndices = (int*)meshToFill->m_VtxBonesIndices.ToPointer();

				// We use Marshal.Copy as often as possible
				// Indices:
				Marshal.Copy(trianglesSrc, 0, meshToFill->m_Indices, trianglesSrc.Length);
				// Positions:
				if (positions != null)
				{
					Vector3[] srcPositions = mesh.vertices;
					for (int i = 0; i < vertexCount; ++i)
						positions[i] = srcPositions[i]; // Vector3 to Vector4
				}
				else
				{
					Debug.LogError("[PopcornFX] Could not copy the mesh positions");
					return false;
				}
				// Normals:
				if (normals != null)
				{
					Vector3[] srcNormals = mesh.normals;
					for (int i = 0; i < vertexCount; ++i)
						normals[i] = srcNormals[i]; // Vector3 to Vector4
				}
				// Tangents (could be copied using Marshal.Copy but does not handle Vector4[]):
				if (tangents != null)
				{
					Vector4[] srcTangents = mesh.tangents;
					for (int i = 0; i < vertexCount; ++i)
						tangents[i] = srcTangents[i];
				}
				// UVs (could be copied using Marshal.Copy but does not handle Vector2[]):
				if (uvs != null)
				{
					Vector2[] srcUvs = mesh.uv;
					for (int i = 0; i < vertexCount; ++i)
						uvs[i] = srcUvs[i];
				}
				if (colors != null)// Colors (could be copied using Marshal.Copy but does not handle Vector4[]):
				{
					Color[] srcColors = mesh.colors;
					for (int i = 0; i < vertexCount; ++i)
						colors[i] = srcColors[i];
				}

				// Bones
				if (bonesWeights != null && bonesIndices != null)
				{
					BoneWeight[] boneWeightsSrc = mesh.boneWeights;

					// Test native code:
					for (int i = 0; i < vertexCount; ++i)
					{
						BoneWeight boneWeight = boneWeightsSrc[i];

						bonesWeights[i * 4 + 0] = boneWeight.weight0;
						bonesWeights[i * 4 + 1] = boneWeight.weight1;
						bonesWeights[i * 4 + 2] = boneWeight.weight2;
						bonesWeights[i * 4 + 3] = boneWeight.weight3;

						bonesIndices[i * 4 + 0] = boneWeight.boneIndex0;
						bonesIndices[i * 4 + 1] = boneWeight.boneIndex1;
						bonesIndices[i * 4 + 2] = boneWeight.boneIndex2;
						bonesIndices[i * 4 + 3] = boneWeight.boneIndex3;
					}
				}
			}
			return true;
		}

		//----------------------------------------------------------------------------

		public static bool UpdateMeshBones(int effectID, int samplerID, SkinnedMeshRenderer skinnedMesh)
		{
			if (skinnedMesh.bones.Length == 0)
			{
				Debug.Log("[PopcornFX] The skinned mesh does not have bones");
				return false;
			}

			Matrix4x4 rootMat = skinnedMesh.transform.parent.worldToLocalMatrix;
			IntPtr matricesPtr = PKFxManager.UpdateSamplerSkinningSetMatrices(effectID, samplerID);

			if (matricesPtr == IntPtr.Zero)
				return false;

			unsafe
			{
				Matrix4x4* matrices = (Matrix4x4*)matricesPtr.ToPointer();
				Transform[] curBonesTransform = skinnedMesh.bones;
				Matrix4x4[] bindPos = skinnedMesh.sharedMesh.bindposes;

				for (int i = 0; i < skinnedMesh.bones.Length; ++i)
				{
					Matrix4x4 boneLocalToWorld = curBonesTransform[i].localToWorldMatrix;
					matrices[i] = rootMat * boneLocalToWorld * bindPos[i];
				}
			}
			return true;
		}

		//----------------------------------------------------------------------------
		// Curve sampler:
		//----------------------------------------------------------------------------

		public static bool UpdateCurveToFill(IntPtr curveToFillPtr, AnimationCurve[] curvesArray, float[] curvesTimeKeys)
		{
			if (curvesTimeKeys == null || curvesTimeKeys.Length == 0 ||
				curvesArray == null || curvesArray.Length == 0)
			{
				Debug.LogError("[PopcornFX] No keypoints / curves found");
				return false;
			}

			// Copy values:
			int keyPointsCount = curvesTimeKeys.Length;
			int curveDimension = curvesArray.Length;
			int keyDataOffset = (1 + curveDimension * 3);

			unsafe
			{
				SCurveSamplerToFill* toFill = (SCurveSamplerToFill*)curveToFillPtr.ToPointer();
				float* dataArray = (float*)toFill->m_KeyPoints.ToPointer();

				if (toFill->m_KeyPointsCount != keyPointsCount ||
					toFill->m_CurveDimension != curveDimension)
				{
					Debug.LogError("[PopcornFX] Curve dimension and key points count does not match with the allocated curve");
					return false;
				}

				for (int keyId = 0; keyId < curvesTimeKeys.Length; ++keyId)
				{
					int realId = keyId * keyDataOffset;

					dataArray[realId] = curvesTimeKeys[keyId];
					for (int curveId = 0; curveId < curvesArray.Length; ++curveId)
					{
						AnimationCurve curve = curvesArray[curveId];
						int curveRealId = realId + 1 + curveId * 3;
						Keyframe key = curve.keys[keyId];

						dataArray[curveRealId + 0] = key.value;
						dataArray[curveRealId + 1] = key.inTangent;
						dataArray[curveRealId + 2] = key.outTangent;
					}
				}
			}
			return true;
		}

		//----------------------------------------------------------------------------

		public static void PreloadEffectFromAsset(PKFxEffectAsset fxAsset)
		{
			if (fxAsset.AssetVirtualPath == null || fxAsset.AssetVirtualPath.Length == 0)
			{
				Debug.LogError("[PopcornFX] Attempting to load corrupted PKFxEffectAsset, Delete and reimport it.");
				return;
			}
			PKFxManager.SetBuiltAsset(fxAsset);
			PKFxManager.PreloadEffectDependencies(fxAsset);
			PKFxManager.PreloadEffectIFN(fxAsset.AssetVirtualPath, fxAsset.m_UsesMeshRenderer);
			PKFxManager.SetBuiltAsset(null);
		}

		public static int CreateEffect(PKFxEffectAsset fxAsset, Transform t)
		{
			if (fxAsset.AssetVirtualPath == null || fxAsset.AssetVirtualPath.Length == 0)
			{
				Debug.LogError("[PopcornFX] Attempting to load corrupted PKFxEffectAsset, Delete and reimport it.");
				return -1;
			}
			PKFxManager.SetBuiltAsset(fxAsset);
			int res = CreateEffect(fxAsset.AssetVirtualPath, t.localToWorldMatrix, fxAsset.m_UsesMeshRenderer);
			PKFxManager.SetBuiltAsset(null);
			return res;
		}

		//----------------------------------------------------------------------------

		public static int CreateEffect(PKFxEffectAsset fxAsset, Vector3 position, Quaternion rotation, Vector3 scale)
		{
			PKFxManager.SetBuiltAsset(fxAsset);
			Matrix4x4 m = Matrix4x4.identity;
			m.SetTRS(position, rotation, scale);
			int res = CreateEffect(fxAsset.AssetVirtualPath, m, fxAsset.m_UsesMeshRenderer);
			PKFxManager.SetBuiltAsset(null);
			return res;
		}

		//----------------------------------------------------------------------------

		public static int CreateEffect(PKFxEffectAsset fxAsset, Matrix4x4 m)
		{
			if (fxAsset == null)
				return -1;
			PKFxManager.SetBuiltAsset(fxAsset);
			int res = CreateEffect(fxAsset.AssetVirtualPath, m, fxAsset.m_UsesMeshRenderer);
			PKFxManager.SetBuiltAsset(null);
			return res;
		}

		//----------------------------------------------------------------------------

		private static int CreateEffect(string path, Matrix4x4 m, bool usesMeshRenderer)
		{
			SFxDesc		desc;

			desc.m_Transforms = m;
			desc.m_FxPath = path;
			desc.m_UsesMeshRenderer = usesMeshRenderer;
			return PKFxManager.InstantiateEffect(ref desc);
		}

		//----------------------------------------------------------------------------

		public static bool UpdateTransformEffect(int FxGUID, Transform t)
		{
			return PKFxManager.SetEffectTransform(FxGUID, t);
		}
	}
}
