//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Runtime.InteropServices;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using Unity.Jobs;
using UnityEngine;

namespace PopcornFX
{
	public class PKFxMeshInstancesRenderer : MonoBehaviour
	{
		public class MeshToDraw
		{
			public Mesh			m_Mesh;
			public int			m_SubMeshId;
			public Matrix4x4	m_ImportTransform;

			public MeshToDraw(Mesh mesh, int subMeshId, Matrix4x4 importTransform)
			{
				m_Mesh = mesh;
				m_SubMeshId = subMeshId;
				m_ImportTransform = importTransform;
			}
		}

		public int[] m_PerLODsSubmeshCount;
		public MeshToDraw[] Meshes
		{
			get { return m_Meshes; }
			set
			{
				m_Meshes = value;
				m_InstancesCount = new int[m_Meshes.Length];
				m_PerInstanceBuffer = new IntPtr[m_Meshes.Length];
			}
		}

		private MeshToDraw[] m_Meshes;
		public Material m_Material;

		public bool m_CastShadow = false;

		private int[] m_InstancesCount;

		private IntPtr[] m_PerInstanceBuffer;

		public int		m_ShaderVariation = 0;
		public string	m_DiffuseColorPropertyName;
		public string	m_EmissiveColorPropertyName;
		public string	m_AlphaRemapCursorPropertyName;
		public string	m_AtlasIdPropertyName = "_AtlasId";
		public string	m_TextureAtlasPropertyName = "_AtlasRects";
		public string	m_VATCursorPropertyName = "_VATCursor";
		public string	m_SkeletalAnimCursor0PropertyName = "_SkeletalAnimCursor0";
		public string	m_SkeletalAnimIdx0PropertyName = "_SkeletalAnimIdx0";
		public string	m_SkeletalAnimCursor1PropertyName = "_SkeletalAnimCursor1";
		public string	m_SkeletalAnimIdx1PropertyName = "_SkeletalAnimIdx1";
		public string	m_SkeletalAnimTransitionPropertyName = "_SkeletalAnimTransition";
		public string	m_SkeletalMeshTransformRow0 = "_SkeletalMeshTransform0";
		public string	m_SkeletalMeshTransformRow1 = "_SkeletalMeshTransform1";
		public string	m_SkeletalMeshTransformRow2 = "_SkeletalMeshTransform2";
		public string	m_SkeletalMeshTransformRow3 = "_SkeletalMeshTransform3";

		public string	m_TransformUVRotatePropertyName = "_TransformUVRotate";
		public string	m_TransformUVOffsetScalePropertyName = "_TransformUVOffsetScale";
		public string   m_AlphaMasksUVDistortionsCursorsPropertyName = "_AlphaMasksUVDistortionsCursors";
		public string   m_DissolveCursorAndRawUV0PropertyName = "_DissolveCursorAndRawUV0";

		NativeArray<Matrix4x4> m_Transforms;
		NativeArray<Vector4> m_DiffuseColors;
		NativeArray<Vector4> m_EmissiveColors;
		NativeArray<float> m_AlphaCursors;
		NativeArray<float> m_VatCursors;
		NativeArray<float> m_AnimCursor0;
		NativeArray<float> m_AnimIdx0;
		NativeArray<float> m_AnimCursor1;
		NativeArray<float> m_AnimIdx1;
		NativeArray<float> m_AnimTransition;
		NativeArray<float> m_AtlasId;
		NativeArray<float> m_TransformUVRotate;
		NativeArray<Vector4> m_TransformUVOffsetScale;
		NativeArray<Vector4> m_AlphaMasksUVDistortionsCursors;
		NativeArray<Vector4> m_DissolveCursorAndRawUV0;

		Matrix4x4[] m_TransformsArray;
		Vector4[] m_DiffuseColorsArray;
		Vector4[] m_EmissiveColorsArray;
		float[] m_AlphaCursorsArray;
		float[] m_VatCursorsArray;
		float[] m_AnimCursor0Array;
		float[] m_AnimIdx0Array;
		float[] m_AnimCursor1Array;
		float[] m_AnimIdx1Array;
		float[] m_AnimTransitionArray;
		float[] m_AtlasIdArray;
		float[] m_TransformUVRotateArray;
		Vector4[] m_TransformUVOffsetScaleArray;
		Vector4[] m_AlphaMasksUVDistortionsCursorsArray;
		Vector4[] m_DissolveCursorAndRawUV0Array;

		MaterialPropertyBlock m_PropertyBlock;

		// Job adding two floating point values together
		public struct MeshData : IJobParallelFor
		{
			[ReadOnly]
			public Matrix4x4 meshTransform;
			[ReadOnly]
			public int offset;
			[NativeDisableUnsafePtrRestriction]
			[ReadOnly]
			public IntPtr buffer;
			[ReadOnly]
			public int count;
			[ReadOnly]
			public int m_ShaderVariation;


			public NativeArray<Matrix4x4> transforms;
			public NativeArray<Vector4> diffuseColors;
			public NativeArray<Vector4> emissiveColors;
			public NativeArray<float> alphaCursors;
			public NativeArray<float> vatCursors;
			
			// Skeletal Anim:
			public NativeArray<float> animCursor0;
			public NativeArray<float> animIdx0;
			public NativeArray<float> animCursor1;
			public NativeArray<float> animIdx1;
			public NativeArray<float> animTransition;

			public NativeArray<float> atlasId;
			public NativeArray<float> transformUVRotate;
			public NativeArray<Vector4> transformUVOffsetScale;

			public Vector4 transformUVOffsetScaleVec;
            public NativeArray<Vector4> alphaMasksAndUVDistortionAnimationCursors;
			public NativeArray<Vector4> dissolveAnimationCursorAndRawUVs;

			public void Execute(int h)
			{
				unsafe
				{
					void *currentPtr = buffer.ToPointer();

					Matrix4x4* instanceTransform = (Matrix4x4*)currentPtr;
					currentPtr = instanceTransform + count;
					//Matrix4x4 Multiply is 90% of the task
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
						transforms[h] = instanceTransform[offset + h];
					else
						transforms[h] = instanceTransform[offset + h] * meshTransform;


					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Color) != 0)
					{
						Vector4 *instanceColor = (Vector4*)currentPtr;
						diffuseColors[h] = instanceColor[offset + h];
						currentPtr = instanceColor + count;
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Emissive) != 0)
					{
						Vector4 *instanceEmissiveColor = (Vector4*)currentPtr;
						emissiveColors[h] = instanceEmissiveColor[offset + h];
						currentPtr = instanceEmissiveColor + count;
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_AlphaRemap) != 0)
					{
						float *instanceAlphaCursor = (float*)currentPtr;
						alphaCursors[h] = instanceAlphaCursor[offset + h];
						currentPtr = instanceAlphaCursor + count;
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Atlas) != 0)
					{
						float *instanceAtlasId = (float*)currentPtr;
						atlasId[h] = instanceAtlasId[offset + h];
						currentPtr = instanceAtlasId + count;
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_FluidVAT) != 0 ||
						(m_ShaderVariation & (int)EShaderVariationFlags.Has_RigidVAT) != 0 ||
						(m_ShaderVariation & (int)EShaderVariationFlags.Has_SoftVAT) != 0)
					{
						float *instanceCursor = (float*)currentPtr;
						vatCursors[h] = instanceCursor[offset + h];
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
					{
						uint *instanceCursorAnimIdx = (uint*)currentPtr;
						animIdx0[h] = (float)instanceCursorAnimIdx[offset + h];
						currentPtr = instanceCursorAnimIdx + count;
						float *instanceCursorAnim0 = (float*)currentPtr;
						animCursor0[h] = instanceCursorAnim0[offset + h];
						currentPtr = instanceCursorAnim0 + count;
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalTrackInterpol) != 0)
					{
						uint *instanceCursorAnim1Idx = (uint*)currentPtr;
						animIdx1[h] = (float)instanceCursorAnim1Idx[offset + h];
						currentPtr = instanceCursorAnim1Idx + count;
						float *instanceCursorAnim1 = (float*)currentPtr;
						animCursor1[h] = instanceCursorAnim1[offset + h];
						currentPtr = instanceCursorAnim1 + count;
						float *instanceTransitionCursorAnim1 = (float*)currentPtr;
						animTransition[h] = instanceTransitionCursorAnim1[offset + h];
						currentPtr = instanceTransitionCursorAnim1 + count;
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_TransformUVs) != 0)
					{
						float *instanceTransformRotate = (float*)currentPtr;
						transformUVRotate[h] = instanceTransformRotate[offset + h];
						currentPtr = instanceTransformRotate + count;
						Vector2 *instanceTransformOffset = (Vector2*)currentPtr;
						currentPtr = instanceTransformOffset + count;
						Vector2 *instanceTransformScale = (Vector2*)currentPtr;
						currentPtr = instanceTransformScale + count;
						transformUVOffsetScaleVec.Set(instanceTransformOffset[offset + h].x, instanceTransformOffset[offset + h].y, instanceTransformScale[offset + h].x, instanceTransformScale[offset + h].y);
						transformUVOffsetScale.ReinterpretStore(h, transformUVOffsetScaleVec);
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_AlphaMasks) != 0 && (m_ShaderVariation & (int)EShaderVariationFlags.Has_UVDistortions) != 0)
					{
						Vector2 *instanceAnimationCursors = (Vector2*)currentPtr;
						Vector2 alphaMasksCursors = instanceAnimationCursors[offset + h];
						currentPtr = instanceAnimationCursors + count;
						instanceAnimationCursors = (Vector2*)currentPtr;
						Vector4 alphaMasksAndUVDistortionAnimationCursorsVec = new Vector4(alphaMasksCursors.x, alphaMasksCursors.y, instanceAnimationCursors[offset + h].x, instanceAnimationCursors[offset + h].y);
						alphaMasksAndUVDistortionAnimationCursors.ReinterpretStore(h, alphaMasksAndUVDistortionAnimationCursorsVec);
						currentPtr = instanceAnimationCursors + count;
					}
					else
					{
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_AlphaMasks) != 0)
						{
							Vector2 *instanceAnimationCursors = (Vector2*)currentPtr;
							alphaMasksAndUVDistortionAnimationCursors[h] = new Vector4(instanceAnimationCursors[offset + h].x, instanceAnimationCursors[offset + h].y, 0.0f, 0.0f);
							currentPtr = instanceAnimationCursors + count;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_UVDistortions) != 0)
						{
							Vector2 *instanceAnimationCursors = (Vector2*)currentPtr;
							alphaMasksAndUVDistortionAnimationCursors[h] = new Vector4(0.0f, 0.0f, instanceAnimationCursors[offset + h].x, instanceAnimationCursors[offset + h].y);
							currentPtr = instanceAnimationCursors + count;
						}
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Dissolve) != 0)
					{
						float *instanceCursor = (float*)currentPtr;
						float dissolveCursor = instanceCursor[offset + h];
						currentPtr = instanceCursor + count;
						Vector4 dissolveCursorAndRawUV0Vec;
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Atlas) != 0)
						{
							Vector2 *instanceRawUV0 = (Vector2*)currentPtr;
							Vector2 rawUV0 = instanceRawUV0[offset + h];
							dissolveCursorAndRawUV0Vec = new Vector4(dissolveCursor, rawUV0.x, rawUV0.y, 0.0f);
							currentPtr = instanceRawUV0 + count;
						}
						else
							dissolveCursorAndRawUV0Vec = new Vector4(dissolveCursor, 0.0f, 0.0f, 0.0f);
						dissolveAnimationCursorAndRawUVs.ReinterpretStore(h, dissolveCursorAndRawUV0Vec);
						
					}
				}
			}
		}

		public void SetInstanceBuffer(IntPtr perInstance, int submesh)
		{
			if (submesh < 0)
			{
				for (int i = 0; i < m_InstancesCount.Length; ++i)
					m_PerInstanceBuffer[i] = perInstance;
			}
			else
				m_PerInstanceBuffer[submesh] = perInstance;
		}

		public void SetInstanceCount(int instanceCount, int submesh)
		{
			if (submesh < 0)
			{
				for (int i = 0; i < m_InstancesCount.Length; ++i)
					m_InstancesCount[i] = instanceCount;
			}
			else
				m_InstancesCount[submesh] = instanceCount;
		}

		private void Start()
		{
		}

		public void OnEnable()
		{
			if (PKFxSettings.UseMeshInstancing)
			{
				m_Transforms = new NativeArray<Matrix4x4>(1023, Allocator.Persistent);
				m_DiffuseColors = new NativeArray<Vector4>(1023, Allocator.Persistent);
				m_EmissiveColors = new NativeArray<Vector4>(1023, Allocator.Persistent);
				m_AlphaCursors = new NativeArray<float>(1023, Allocator.Persistent);
				m_VatCursors = new NativeArray<float>(1023, Allocator.Persistent);
				m_AtlasId = new NativeArray<float>(1023, Allocator.Persistent);
				m_AnimCursor0 = new NativeArray<float>(1023, Allocator.Persistent);
				m_AnimIdx0 = new NativeArray<float>(1023, Allocator.Persistent);
				m_AnimCursor1 = new NativeArray<float>(1023, Allocator.Persistent);
				m_AnimIdx1 = new NativeArray<float>(1023, Allocator.Persistent);
				m_AnimTransition = new NativeArray<float>(1023, Allocator.Persistent);
				m_TransformUVRotate = new NativeArray<float>(1023, Allocator.Persistent);
				m_TransformUVOffsetScale = new NativeArray<Vector4>(1023, Allocator.Persistent);
				m_AlphaMasksUVDistortionsCursors = new NativeArray<Vector4>(1023, Allocator.Persistent);
				m_DissolveCursorAndRawUV0 = new NativeArray<Vector4>(1023, Allocator.Persistent);

				m_TransformsArray = new Matrix4x4[1023];
				m_DiffuseColorsArray = new Vector4[1023];
				m_EmissiveColorsArray = new Vector4[1023];
				m_AlphaCursorsArray = new float[1023];
				m_VatCursorsArray = new float[1023];
				m_AnimCursor0Array = new float[1023];
				m_AnimIdx0Array = new float[1023];
				m_AnimCursor1Array = new float[1023];
				m_AnimIdx1Array = new float[1023];
				m_AnimTransitionArray = new float[1023];
				m_AtlasIdArray = new float[1023];
				m_TransformUVRotateArray = new float[1023];
				m_TransformUVOffsetScaleArray = new Vector4[1023];
				m_AlphaMasksUVDistortionsCursorsArray = new Vector4[1023];
				m_DissolveCursorAndRawUV0Array = new Vector4[1023];

			}
			m_PropertyBlock = new MaterialPropertyBlock();
		}

		public void OnDisable()
		{
			if (PKFxSettings.UseMeshInstancing)
			{
				m_Transforms.Dispose();
				m_DiffuseColors.Dispose();
				m_EmissiveColors.Dispose();
				m_AlphaCursors.Dispose();
				m_VatCursors.Dispose();
				m_AtlasId.Dispose();
				m_AnimCursor0.Dispose();
				m_AnimIdx0.Dispose();
				m_AnimCursor1.Dispose();
				m_AnimIdx1.Dispose();
				m_AnimTransition.Dispose();
				m_TransformUVRotate.Dispose();
				m_TransformUVOffsetScale.Dispose();
				m_AlphaMasksUVDistortionsCursors.Dispose();
				m_DissolveCursorAndRawUV0.Dispose();
			}
		}

		private Matrix4x4[] CopyNativeArrayToArray(NativeArray<Matrix4x4> from, Matrix4x4[] to, int size)
		{
			NativeArray<Matrix4x4>.Copy(from, to, size);
			return to;
		}
		private Vector4[] CopyNativeArrayToArray(NativeArray<Vector4> from, Vector4[] to, int size)
		{
			NativeArray<Vector4>.Copy(from, to, size);
			return to;
		}
		private float[] CopyNativeArrayToArray(NativeArray<float> from, float[] to, int size)
		{
			NativeArray<float>.Copy(from, to, size);
			return to;
		}

		public void DrawMeshes()
		{
			if (!gameObject.activeSelf || m_Meshes == null || m_Meshes.Length == 0)
				return;
			int generalOffset = 0;
			if (PKFxSettings.UseMeshInstancing)
			{
				unsafe
				{
					m_PropertyBlock.Clear();

					int bufferIdx = 0;

					foreach (MeshToDraw meshToDraw in m_Meshes)
					{
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
						{
							m_PropertyBlock.SetVector(m_SkeletalMeshTransformRow0, meshToDraw.m_ImportTransform.GetRow(0));
							m_PropertyBlock.SetVector(m_SkeletalMeshTransformRow1, meshToDraw.m_ImportTransform.GetRow(1));
							m_PropertyBlock.SetVector(m_SkeletalMeshTransformRow2, meshToDraw.m_ImportTransform.GetRow(2));
							m_PropertyBlock.SetVector(m_SkeletalMeshTransformRow3, meshToDraw.m_ImportTransform.GetRow(3));
						}
						if (m_PerInstanceBuffer[bufferIdx] != IntPtr.Zero && m_InstancesCount[bufferIdx] > 0)
						{
							for (int i = 0; i < m_InstancesCount[bufferIdx]; i += 1023)
							{
								MeshData job = new MeshData();
								//Out
								job.transforms = m_Transforms;
								job.diffuseColors = m_DiffuseColors;
								job.emissiveColors = m_EmissiveColors;
								job.alphaCursors = m_AlphaCursors;
								job.vatCursors = m_VatCursors;
								job.atlasId = m_AtlasId;
								job.animCursor0 = m_AnimCursor0;
								job.animIdx0 = m_AnimIdx0;
								job.animCursor1 = m_AnimCursor1;
								job.animIdx1 = m_AnimIdx1;
								job.animTransition = m_AnimTransition;

								job.transformUVRotate = m_TransformUVRotate;
								job.transformUVOffsetScale = m_TransformUVOffsetScale;

								job.alphaMasksAndUVDistortionAnimationCursors = m_AlphaMasksUVDistortionsCursors;
								job.dissolveAnimationCursorAndRawUVs = m_DissolveCursorAndRawUV0;

								//In data
								job.meshTransform = meshToDraw.m_ImportTransform;
								job.offset = i;
								job.buffer = m_PerInstanceBuffer[bufferIdx];
								job.count = m_InstancesCount[bufferIdx];
								job.m_ShaderVariation = m_ShaderVariation;

								int dataLeft = Math.Min(m_InstancesCount[bufferIdx] - i, 1023);
								bool isLit = false;

								// Schedule the job with one Execute per index in the results array and only 1 item per processing batch
								JobHandle handle = job.Schedule(dataLeft, 1);

								// Wait for the job to complete
								handle.Complete();

								generalOffset += dataLeft;

								m_PropertyBlock.SetVectorArray(m_DiffuseColorPropertyName, CopyNativeArrayToArray(m_DiffuseColors, m_DiffuseColorsArray, dataLeft));
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Emissive) != 0)
								{
									m_PropertyBlock.SetVectorArray(m_EmissiveColorPropertyName, CopyNativeArrayToArray(m_EmissiveColors, m_EmissiveColorsArray, dataLeft));
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_AlphaRemap) != 0)
								{
									m_PropertyBlock.SetFloatArray(m_AlphaRemapCursorPropertyName, CopyNativeArrayToArray(m_AlphaCursors, m_AlphaCursorsArray, dataLeft));
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_FluidVAT) != 0 ||
									(m_ShaderVariation & (int)EShaderVariationFlags.Has_RigidVAT) != 0 ||
									(m_ShaderVariation & (int)EShaderVariationFlags.Has_SoftVAT) != 0)
								{
									m_PropertyBlock.SetFloatArray(m_VATCursorPropertyName, CopyNativeArrayToArray(m_VatCursors, m_VatCursorsArray, dataLeft));
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Lighting) != 0)
								{
									isLit = true;
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
								{
									m_PropertyBlock.SetFloatArray(m_SkeletalAnimCursor0PropertyName, CopyNativeArrayToArray(m_AnimCursor0, m_AnimCursor0Array, dataLeft));
									m_PropertyBlock.SetFloatArray(m_SkeletalAnimIdx0PropertyName, CopyNativeArrayToArray(m_AnimIdx0, m_AnimIdx0Array, dataLeft));
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalTrackInterpol) != 0)
								{
									m_PropertyBlock.SetFloatArray(m_SkeletalAnimCursor1PropertyName, CopyNativeArrayToArray(m_AnimCursor1, m_AnimCursor1Array, dataLeft));
									m_PropertyBlock.SetFloatArray(m_SkeletalAnimIdx1PropertyName, CopyNativeArrayToArray(m_AnimIdx1, m_AnimIdx1Array, dataLeft));
									m_PropertyBlock.SetFloatArray(m_SkeletalAnimTransitionPropertyName, CopyNativeArrayToArray(m_AnimTransition, m_AnimTransitionArray, dataLeft));
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Atlas) != 0)
								{
									m_PropertyBlock.SetFloatArray(m_AtlasIdPropertyName, CopyNativeArrayToArray(m_AtlasId, m_AtlasIdArray, dataLeft));
								}

								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_TransformUVs) != 0)
								{
									m_PropertyBlock.SetFloatArray(m_TransformUVRotatePropertyName, CopyNativeArrayToArray(m_TransformUVRotate, m_TransformUVRotateArray, dataLeft));
									m_PropertyBlock.SetVectorArray(m_TransformUVOffsetScalePropertyName, CopyNativeArrayToArray(m_TransformUVOffsetScale, m_TransformUVOffsetScaleArray, dataLeft));
								}

								if ((m_ShaderVariation & ((int)EShaderVariationFlags.Has_AlphaMasks) | (int)EShaderVariationFlags.Has_UVDistortions) != 0)
								{
									m_PropertyBlock.SetVectorArray(m_AlphaMasksUVDistortionsCursorsPropertyName, CopyNativeArrayToArray(m_AlphaMasksUVDistortionsCursors, m_AlphaMasksUVDistortionsCursorsArray, dataLeft));
								}
								if ((m_ShaderVariation & ((int)EShaderVariationFlags.Has_Dissolve | (int)EShaderVariationFlags.Has_Atlas))  != 0)
								{
									m_PropertyBlock.SetVectorArray(m_DissolveCursorAndRawUV0PropertyName, CopyNativeArrayToArray(m_DissolveCursorAndRawUV0, m_DissolveCursorAndRawUV0Array, dataLeft));
								}

								Graphics.DrawMeshInstanced(
									meshToDraw.m_Mesh,
									meshToDraw.m_SubMeshId,
									m_Material,
									CopyNativeArrayToArray(m_Transforms, m_TransformsArray, dataLeft),
									dataLeft,
									m_PropertyBlock,
									m_CastShadow ? UnityEngine.Rendering.ShadowCastingMode.On : UnityEngine.Rendering.ShadowCastingMode.Off,
									isLit
								);
							}
						}
						++bufferIdx;
					}
				}
			}
			else
			{
				int bufferIdx = 0;

				foreach (MeshToDraw meshToDraw in m_Meshes)
				{
					m_PropertyBlock.Clear();

					unsafe
					{
						Matrix4x4* instanceTransform = null;
						Vector4* instanceDiffuseColor = null;
						Vector4* instanceEmissiveColor = null;
						float* instanceAlphaCursor = null;
						float* instanceVATCursor = null;
						float* atlasId = null;

						float* instanceCurrentAnimCursor = null;
						uint* instanceCurrentAnimIdx = null;
						float* instanceNextAnimCursor = null;
						uint* instanceNextAnimIdx = null;
						float* instanceTransitionRatio = null;


						float* instanceTransformUVRotate = null;
						Vector2* instanceTransformUVOffset = null;
						Vector2* instanceTransformUVScale = null;

						Vector2* instanceAlphaMasksCursors = null;
						Vector2* instanceUVDistortionsCursors = null;
						float* instanceDissolveCursors = null;
						Vector2* instanceRawUV0 = null;

						void* currentPtr = m_PerInstanceBuffer[bufferIdx].ToPointer();
						int instanceCount = m_InstancesCount[bufferIdx];
						bool isLit = false;

						instanceTransform = (Matrix4x4*)currentPtr;
						currentPtr = instanceTransform + instanceCount;
						instanceDiffuseColor = (Vector4*)currentPtr;
						currentPtr = instanceDiffuseColor + instanceCount;

						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Emissive) != 0)
						{
							instanceEmissiveColor = (Vector4*)currentPtr;
							currentPtr = instanceEmissiveColor + instanceCount;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_AlphaRemap) != 0)
						{
							instanceAlphaCursor = (float*)currentPtr;
							currentPtr = instanceAlphaCursor + instanceCount;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_FluidVAT) != 0 ||
							(m_ShaderVariation & (int)EShaderVariationFlags.Has_RigidVAT) != 0 ||
							(m_ShaderVariation & (int)EShaderVariationFlags.Has_SoftVAT) != 0)
						{
							instanceVATCursor = (float*)currentPtr;
							currentPtr = instanceVATCursor + instanceCount;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Atlas) != 0)
						{
							atlasId = (float*)currentPtr;
							currentPtr = atlasId + instanceCount;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Lighting) != 0)
						{
							isLit = true;
						}

						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
						{
							m_PropertyBlock.SetVector(m_SkeletalMeshTransformRow0, meshToDraw.m_ImportTransform.GetRow(0));
							m_PropertyBlock.SetVector(m_SkeletalMeshTransformRow1, meshToDraw.m_ImportTransform.GetRow(1));
							m_PropertyBlock.SetVector(m_SkeletalMeshTransformRow2, meshToDraw.m_ImportTransform.GetRow(2));
							m_PropertyBlock.SetVector(m_SkeletalMeshTransformRow3, meshToDraw.m_ImportTransform.GetRow(3));
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
						{
							instanceCurrentAnimIdx = (uint*)currentPtr;
							currentPtr = instanceCurrentAnimIdx + instanceCount;
							instanceCurrentAnimCursor = (float*)currentPtr;
							currentPtr = instanceCurrentAnimCursor + instanceCount;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalTrackInterpol) != 0)
						{
							instanceNextAnimIdx = (uint*)currentPtr;
							currentPtr = instanceNextAnimIdx + instanceCount;
							instanceNextAnimCursor = (float*)currentPtr;
							currentPtr = instanceNextAnimCursor + instanceCount;
							instanceTransitionRatio = (float*)currentPtr;
							currentPtr = instanceTransitionRatio + instanceCount;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_TransformUVs) != 0)
						{
							instanceTransformUVRotate = (float*)currentPtr;
							currentPtr = instanceTransformUVRotate + instanceCount;
							instanceTransformUVOffset = (Vector2*)currentPtr;
							currentPtr = instanceTransformUVOffset + instanceCount;
							instanceTransformUVScale = (Vector2*)currentPtr;
							currentPtr = instanceTransformUVScale + instanceCount;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_AlphaMasks) != 0)
						{
							instanceAlphaMasksCursors = (Vector2*)currentPtr;
							currentPtr = instanceAlphaMasksCursors + instanceCount;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_UVDistortions) != 0)
						{
							instanceUVDistortionsCursors = (Vector2*)currentPtr;
							currentPtr = instanceUVDistortionsCursors + instanceCount;
						}
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Dissolve) != 0) 
						{ 
							instanceDissolveCursors = (float*)currentPtr;
							currentPtr = instanceDissolveCursors + instanceCount;
							instanceRawUV0 = (Vector2*)(currentPtr);
							currentPtr = instanceRawUV0 + instanceCount;
						}

						for (int i = 0; i < m_InstancesCount[bufferIdx]; i++)
						{
							Matrix4x4 meshTransform;

							if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
								meshTransform = instanceTransform[i];
							else
								meshTransform = instanceTransform[i] * meshToDraw.m_ImportTransform;
							m_PropertyBlock.SetVector(m_DiffuseColorPropertyName, instanceDiffuseColor[i]);
							if (instanceEmissiveColor != null && !string.IsNullOrEmpty(m_EmissiveColorPropertyName))
								m_PropertyBlock.SetVector(m_EmissiveColorPropertyName, instanceEmissiveColor[i]);
							if (instanceAlphaCursor != null && !string.IsNullOrEmpty(m_AlphaRemapCursorPropertyName))
								m_PropertyBlock.SetFloat(m_AlphaRemapCursorPropertyName, instanceAlphaCursor[i]);
							if (instanceVATCursor != null && !string.IsNullOrEmpty(m_VATCursorPropertyName))
								m_PropertyBlock.SetFloat(m_VATCursorPropertyName, instanceVATCursor[i]);
							if (atlasId != null && !string.IsNullOrEmpty(m_AtlasIdPropertyName))
								m_PropertyBlock.SetFloat(m_AtlasIdPropertyName, atlasId[i]);
							if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
							{
								if (instanceCurrentAnimIdx != null && !string.IsNullOrEmpty(m_SkeletalAnimIdx0PropertyName))
									m_PropertyBlock.SetFloat(m_SkeletalAnimIdx0PropertyName, instanceCurrentAnimIdx[i]);
								if (instanceCurrentAnimCursor != null && !string.IsNullOrEmpty(m_SkeletalAnimCursor0PropertyName))
									m_PropertyBlock.SetFloat(m_SkeletalAnimCursor0PropertyName, instanceCurrentAnimCursor[i]);
							}
							if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalTrackInterpol) != 0)
							{
								if (instanceNextAnimIdx != null && !string.IsNullOrEmpty(m_SkeletalAnimIdx1PropertyName))
									m_PropertyBlock.SetFloat(m_SkeletalAnimIdx1PropertyName, instanceNextAnimIdx[i]);
								if (instanceNextAnimCursor != null && !string.IsNullOrEmpty(m_SkeletalAnimCursor1PropertyName))
									m_PropertyBlock.SetFloat(m_SkeletalAnimCursor1PropertyName, instanceNextAnimCursor[i]);
								if (instanceTransitionRatio != null && !string.IsNullOrEmpty(m_SkeletalAnimTransitionPropertyName))
									m_PropertyBlock.SetFloat(m_SkeletalAnimTransitionPropertyName, instanceTransitionRatio[i]);
							}
							if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_TransformUVs) != 0)
							{
								if (instanceTransformUVRotate != null && !string.IsNullOrEmpty(m_TransformUVRotatePropertyName))
									m_PropertyBlock.SetFloat(m_TransformUVRotatePropertyName, instanceTransformUVRotate[i]);
								if (instanceTransformUVOffset != null && instanceTransformUVScale != null && !string.IsNullOrEmpty(m_TransformUVOffsetScalePropertyName))
								{
									m_PropertyBlock.SetVector(m_TransformUVOffsetScalePropertyName, new Vector4(instanceTransformUVOffset[i].x, instanceTransformUVOffset[i].y, instanceTransformUVScale[i].x, instanceTransformUVScale[i].y));
								}
							}
							if ((m_ShaderVariation & ((int)EShaderVariationFlags.Has_AlphaMasks | (int)EShaderVariationFlags.Has_UVDistortions)) != 0)
							{
								Vector4 alphaMasksUVDistortions = new Vector4();
								if (instanceAlphaMasksCursors != null && !string.IsNullOrEmpty(m_AlphaMasksUVDistortionsCursorsPropertyName))
								{
									alphaMasksUVDistortions.x = instanceAlphaMasksCursors[i].x;
									alphaMasksUVDistortions.y = instanceAlphaMasksCursors[i].y;
								}
								if (instanceUVDistortionsCursors != null && !string.IsNullOrEmpty(m_AlphaMasksUVDistortionsCursorsPropertyName))
								{
									alphaMasksUVDistortions.z = instanceUVDistortionsCursors[i].x;
									alphaMasksUVDistortions.w = instanceUVDistortionsCursors[i].y;
								}		
							}
							if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Dissolve) != 0)
							{
								Vector3 dissolveAndRawUV0 = new Vector3();
								if (!string.IsNullOrEmpty(m_DissolveCursorAndRawUV0PropertyName))
								{
									if (instanceDissolveCursors != null)
									{
										dissolveAndRawUV0.x = instanceDissolveCursors[i];
									}
									if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Atlas) != 0 && (instanceRawUV0 != null))
									{
										dissolveAndRawUV0.y = instanceRawUV0[i].x;
										dissolveAndRawUV0.z = instanceRawUV0[i].y;
									}
								}

							}

							Graphics.DrawMesh(meshToDraw.m_Mesh, meshTransform, m_Material, 0, null, meshToDraw.m_SubMeshId, m_PropertyBlock, m_CastShadow, isLit);
						}
					}
					++bufferIdx;
				}
			}
		}
	}
}