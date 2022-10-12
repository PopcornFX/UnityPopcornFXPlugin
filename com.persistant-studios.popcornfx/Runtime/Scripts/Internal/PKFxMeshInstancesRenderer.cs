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

			public void Execute(int h)
			{
				unsafe
				{
					void* currentPtr = buffer.ToPointer();

					Matrix4x4* instanceTransform = (Matrix4x4*)currentPtr;
					currentPtr = instanceTransform + count;
					//Matrix4x4 Multiply is 90% of the task
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
						transforms[h] = instanceTransform[offset + h];
					else
						transforms[h] = instanceTransform[offset + h] * meshTransform;

					Vector4* instanceColor = (Vector4*)currentPtr;
					diffuseColors[h] = instanceColor[offset + h];
					currentPtr = instanceColor + count;

					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Emissive) != 0)
					{
						Vector3* instanceEmissiveColor = (Vector3*)currentPtr;
						emissiveColors[h] = instanceEmissiveColor[offset + h];
						currentPtr = instanceEmissiveColor + count;
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_AlphaRemap) != 0)
					{
						float* instanceAlphaCursor = (float*)currentPtr;
						alphaCursors[h] = instanceAlphaCursor[offset + h];
						currentPtr = instanceAlphaCursor + count;
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_FluidVAT) != 0 ||
						(m_ShaderVariation & (int)EShaderVariationFlags.Has_RigidVAT) != 0 ||
						(m_ShaderVariation & (int)EShaderVariationFlags.Has_SoftVAT) != 0)
					{
						float* instanceCursor = (float*)currentPtr;
						vatCursors[h] = instanceCursor[offset + h];
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
					{
						uint* instanceCursorAnimIdx = (uint*)currentPtr;
						animIdx0[h] = (float)instanceCursorAnimIdx[offset + h];
						currentPtr = instanceCursorAnimIdx + count;
						float* instanceCursorAnim0 = (float*)currentPtr;
						animCursor0[h] = instanceCursorAnim0[offset + h];
						currentPtr = instanceCursorAnim0 + count;
					}
					if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalTrackInterpol) != 0)
					{
						uint* instanceCursorAnim1Idx = (uint*)currentPtr;
						animIdx1[h] = (float)instanceCursorAnim1Idx[offset + h];
						currentPtr = instanceCursorAnim1Idx + count;
						float* instanceCursorAnim1 = (float*)currentPtr;
						animCursor1[h] = instanceCursorAnim1[offset + h];
						currentPtr = instanceCursorAnim1 + count;
						float* instanceTransitionCursorAnim1 = (float*)currentPtr;
						animTransition[h] = instanceTransitionCursorAnim1[offset + h];
						currentPtr = instanceTransitionCursorAnim1 + count;
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
			if (m_DiffuseColorPropertyName == null || m_DiffuseColorPropertyName.Length == 0)
			{
				Debug.LogError("[PopcornFX] Error : Mesh Color Property Name is empty, set it in MaterialFactory");
			}
		}

		public void OnEnable()
		{
			m_Transforms = new NativeArray<Matrix4x4>(1023, Allocator.Persistent);
			m_DiffuseColors = new NativeArray<Vector4>(1023, Allocator.Persistent);
			m_EmissiveColors = new NativeArray<Vector4>(1023, Allocator.Persistent);
			m_AlphaCursors = new NativeArray<float>(1023, Allocator.Persistent);
			m_VatCursors = new NativeArray<float>(1023, Allocator.Persistent);
			m_AnimCursor0 = new NativeArray<float>(1023, Allocator.Persistent);
			m_AnimIdx0 = new NativeArray<float>(1023, Allocator.Persistent);
			m_AnimCursor1 = new NativeArray<float>(1023, Allocator.Persistent);
			m_AnimIdx1 = new NativeArray<float>(1023, Allocator.Persistent);
			m_AnimTransition = new NativeArray<float>(1023, Allocator.Persistent);
		}

		public void OnDisable()
		{
			m_Transforms.Dispose();
			m_DiffuseColors.Dispose();
			m_EmissiveColors.Dispose();
			m_AlphaCursors.Dispose();
			m_VatCursors.Dispose();
			m_AnimCursor0.Dispose();
			m_AnimIdx0.Dispose();
			m_AnimCursor1.Dispose();
			m_AnimIdx1.Dispose();
			m_AnimTransition.Dispose();
		}

		void LateUpdate()
		{
			if (m_Meshes == null || m_Meshes.Length == 0)
				return;
			int generalOffset = 0;
			if (PKFxSettings.UseMeshInstancing)
			{
				unsafe
				{
					MaterialPropertyBlock materialProp = new MaterialPropertyBlock();
					int bufferIdx = 0;

					foreach (MeshToDraw meshToDraw in m_Meshes)
					{
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
						{
							materialProp.SetVector(m_SkeletalMeshTransformRow0, meshToDraw.m_ImportTransform.GetRow(0));
							materialProp.SetVector(m_SkeletalMeshTransformRow1, meshToDraw.m_ImportTransform.GetRow(1));
							materialProp.SetVector(m_SkeletalMeshTransformRow2, meshToDraw.m_ImportTransform.GetRow(2));
							materialProp.SetVector(m_SkeletalMeshTransformRow3, meshToDraw.m_ImportTransform.GetRow(3));
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
								job.animCursor0 = m_AnimCursor0;
								job.animIdx0 = m_AnimIdx0;
								job.animCursor1 = m_AnimCursor1;
								job.animIdx1 = m_AnimIdx1;
								job.animTransition = m_AnimTransition;

								//In data
								job.meshTransform = meshToDraw.m_ImportTransform;
								job.offset = i;
								job.buffer = m_PerInstanceBuffer[bufferIdx];
								job.count = m_InstancesCount[bufferIdx];
								job.m_ShaderVariation = m_ShaderVariation;

								int dataLeft = Math.Min(m_InstancesCount[bufferIdx] - i, 1023);
								bool isLit = false;

								// Schedule the job with one Execute per index in the results array and only 1 item per processing batch
								JobHandle handle = job.Schedule(Math.Min(dataLeft, 1023), 1);

								// Wait for the job to complete
								handle.Complete();

								generalOffset += dataLeft;
								materialProp.SetVectorArray(m_DiffuseColorPropertyName, m_DiffuseColors.ToArray());
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Emissive) != 0)
								{
									materialProp.SetVectorArray(m_EmissiveColorPropertyName, m_EmissiveColors.ToArray());
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_AlphaRemap) != 0)
								{
									materialProp.SetFloatArray(m_AlphaRemapCursorPropertyName, m_AlphaCursors.ToArray());
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_FluidVAT) != 0 ||
									(m_ShaderVariation & (int)EShaderVariationFlags.Has_RigidVAT) != 0 ||
									(m_ShaderVariation & (int)EShaderVariationFlags.Has_SoftVAT) != 0)
								{
									materialProp.SetFloatArray(m_VATCursorPropertyName, m_VatCursors.ToArray());
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Lighting) != 0)
								{
									isLit = true;
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
								{
									materialProp.SetFloatArray(m_SkeletalAnimCursor0PropertyName, m_AnimCursor0.ToArray());
									materialProp.SetFloatArray(m_SkeletalAnimIdx0PropertyName, m_AnimIdx0.ToArray());
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalTrackInterpol) != 0)
								{
									materialProp.SetFloatArray(m_SkeletalAnimCursor1PropertyName, m_AnimCursor1.ToArray());
									materialProp.SetFloatArray(m_SkeletalAnimIdx1PropertyName, m_AnimIdx1.ToArray());
									materialProp.SetFloatArray(m_SkeletalAnimTransitionPropertyName, m_AnimTransition.ToArray());
								}

								Graphics.DrawMeshInstanced(
									meshToDraw.m_Mesh,
									meshToDraw.m_SubMeshId,
									m_Material,
									m_Transforms.ToArray(),
									dataLeft,
									materialProp,
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
					MaterialPropertyBlock materialProp = new MaterialPropertyBlock();

					unsafe
					{
						Matrix4x4* instanceTransform = null;
						Vector4* instanceDiffuseColor = null;
						Vector3* instanceEmissiveColor = null;
						float* instanceAlphaCursor = null;
						float* instanceVATCursor = null;

						float* instanceCurrentAnimCursor = null;
						uint* instanceCurrentAnimIdx = null;
						float* instanceNextAnimCursor = null;
						uint* instanceNextAnimIdx = null;
						float* instanceTransitionRatio = null;

						void* currentPtr = m_PerInstanceBuffer[bufferIdx].ToPointer();
						int instanceCount = m_InstancesCount[bufferIdx];
						bool isLit = false;

						instanceTransform = (Matrix4x4*)currentPtr;
						currentPtr = instanceTransform + instanceCount;
						instanceDiffuseColor = (Vector4*)currentPtr;
						currentPtr = instanceDiffuseColor + instanceCount;

						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Emissive) != 0)
						{
							instanceEmissiveColor = (Vector3*)currentPtr;
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
						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Lighting) != 0)
						{
							isLit = true;
						}

						if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
						{
							materialProp.SetVector(m_SkeletalMeshTransformRow0, meshToDraw.m_ImportTransform.GetRow(0));
							materialProp.SetVector(m_SkeletalMeshTransformRow1, meshToDraw.m_ImportTransform.GetRow(1));
							materialProp.SetVector(m_SkeletalMeshTransformRow2, meshToDraw.m_ImportTransform.GetRow(2));
							materialProp.SetVector(m_SkeletalMeshTransformRow3, meshToDraw.m_ImportTransform.GetRow(3));
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

						for (int i = 0; i < m_InstancesCount[bufferIdx]; i++)
						{
							Matrix4x4 meshTransform;

							if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
								meshTransform = instanceTransform[i];
							else
								meshTransform = instanceTransform[i] * meshToDraw.m_ImportTransform;
							materialProp.SetVector(m_DiffuseColorPropertyName, instanceDiffuseColor[i]);
							if (instanceEmissiveColor != null && !string.IsNullOrEmpty(m_EmissiveColorPropertyName))
								materialProp.SetVector(m_EmissiveColorPropertyName, instanceEmissiveColor[i]);
							if (instanceAlphaCursor != null && !string.IsNullOrEmpty(m_AlphaRemapCursorPropertyName))
								materialProp.SetFloat(m_AlphaRemapCursorPropertyName, instanceAlphaCursor[i]);
							if (instanceVATCursor != null && !string.IsNullOrEmpty(m_VATCursorPropertyName))
								materialProp.SetFloat(m_VATCursorPropertyName, instanceVATCursor[i]);
							if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalAnim) != 0)
							{
								if (instanceCurrentAnimIdx != null && !string.IsNullOrEmpty(m_SkeletalAnimIdx0PropertyName))
									materialProp.SetFloat(m_SkeletalAnimIdx0PropertyName, instanceCurrentAnimIdx[i]);
								if (instanceCurrentAnimCursor != null && !string.IsNullOrEmpty(m_SkeletalAnimCursor0PropertyName))
									materialProp.SetFloat(m_SkeletalAnimCursor0PropertyName, instanceCurrentAnimCursor[i]);
							}
							if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_SkeletalTrackInterpol) != 0)
							{
								if (instanceNextAnimIdx != null && !string.IsNullOrEmpty(m_SkeletalAnimIdx1PropertyName))
									materialProp.SetFloat(m_SkeletalAnimIdx1PropertyName, instanceNextAnimIdx[i]);
								if (instanceNextAnimCursor != null && !string.IsNullOrEmpty(m_SkeletalAnimCursor1PropertyName))
									materialProp.SetFloat(m_SkeletalAnimCursor1PropertyName, instanceNextAnimCursor[i]);
								if (instanceTransitionRatio != null && !string.IsNullOrEmpty(m_SkeletalAnimTransitionPropertyName))
									materialProp.SetFloat(m_SkeletalAnimTransitionPropertyName, instanceTransitionRatio[i]);
							}

							Graphics.DrawMesh(meshToDraw.m_Mesh, meshTransform, m_Material, 0, null, meshToDraw.m_SubMeshId, materialProp, m_CastShadow, isLit);
						}
					}
					++bufferIdx;
				}
			}
		}
	}
}