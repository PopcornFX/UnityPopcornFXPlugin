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
		public Mesh[] Meshes
		{
			get { return m_Meshes; }
			set
			{
				m_Meshes = value;
				m_InstancesCount = new int[m_Meshes.Length];
				m_PerInstanceBuffer = new IntPtr[m_Meshes.Length];
			}
		}
		private Mesh[] m_Meshes;
		public Matrix4x4[] m_MeshesImportTransform;
		public Material m_Material;

		public bool m_CastShadow = false;

		private int[] m_InstancesCount;

		private IntPtr[] m_PerInstanceBuffer;

		public int		m_ShaderVariation = 0;
		public string	m_DiffuseColorPropertyName;
		public string	m_EmissiveColorPropertyName;
		public string	m_AlphaRemapCursorPropertyName;
		public string	m_VATCursorPropertyName = "_VATCursor";

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

			public void Execute(int h)
			{
				unsafe
				{
					void* currentPtr = buffer.ToPointer();

					Matrix4x4* instanceTransform = (Matrix4x4*)currentPtr;
					currentPtr = instanceTransform + count;
					//Matrix4x4 Multiply is 90% of the task
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
					NativeArray<Matrix4x4> transforms = new NativeArray<Matrix4x4>(1023, Allocator.TempJob);
					NativeArray<Vector4> diffuseColors = new NativeArray<Vector4>(1023, Allocator.TempJob);
					NativeArray<Vector4> emissiveColors = new NativeArray<Vector4>(1023, Allocator.TempJob);
					NativeArray<float> alphaCursors = new NativeArray<float>(1023, Allocator.TempJob);
					NativeArray<float> vatCursors = new NativeArray<float>(1023, Allocator.TempJob);

					for (int j = 0; j < m_Meshes.Length; ++j)
					{
						Mesh m = m_Meshes[j];
						Matrix4x4 t = m_MeshesImportTransform[j];

						if (m_PerInstanceBuffer[j] != IntPtr.Zero && m_InstancesCount[j] > 0)
						{
							for (int i = 0; i < m_InstancesCount[j]; i += 1023)
							{
								MeshData job = new MeshData();
								//Out
								job.transforms = transforms;
								job.diffuseColors = diffuseColors;
								job.emissiveColors = emissiveColors;
								job.alphaCursors = alphaCursors;
								job.vatCursors = vatCursors;
								//In data
								job.meshTransform = t;
								job.offset = i;
								job.buffer = m_PerInstanceBuffer[j];
								job.count = m_InstancesCount[j];
								job.m_ShaderVariation = m_ShaderVariation;

								int dataLeft = Math.Min(m_InstancesCount[j] - i, 1023);
								bool isLit = false;

								// Schedule the job with one Execute per index in the results array and only 1 item per processing batch
								JobHandle handle = job.Schedule(Math.Min(dataLeft, 1023), 1);

								// Wait for the job to complete
								handle.Complete();

								generalOffset += dataLeft;
								materialProp.SetVectorArray(m_DiffuseColorPropertyName, diffuseColors.ToArray());
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Emissive) != 0)
								{
									materialProp.SetVectorArray(m_EmissiveColorPropertyName, emissiveColors.ToArray());
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_AlphaRemap) != 0)
								{
									materialProp.SetFloatArray(m_AlphaRemapCursorPropertyName, alphaCursors.ToArray());
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_FluidVAT) != 0 ||
									(m_ShaderVariation & (int)EShaderVariationFlags.Has_RigidVAT) != 0 ||
									(m_ShaderVariation & (int)EShaderVariationFlags.Has_SoftVAT) != 0)
								{
									materialProp.SetFloatArray(m_VATCursorPropertyName, vatCursors.ToArray());
								}
								if ((m_ShaderVariation & (int)EShaderVariationFlags.Has_Lighting) != 0)
								{
									isLit = true;
								}
								Graphics.DrawMeshInstanced(
									m,
									0,
									m_Material,
									transforms.ToArray(),
									dataLeft,
									materialProp,
									m_CastShadow ? UnityEngine.Rendering.ShadowCastingMode.On : UnityEngine.Rendering.ShadowCastingMode.Off,
									isLit
								);
							}
						}
					}
					transforms.Dispose();
					diffuseColors.Dispose();
					emissiveColors.Dispose();
					alphaCursors.Dispose();
					vatCursors.Dispose();
				}
			}
			else
			{
				for (int j = 0; j < m_Meshes.Length; ++j)
				{
					MaterialPropertyBlock materialProp = new MaterialPropertyBlock();
					Mesh m = m_Meshes[j];
					Matrix4x4 t = m_MeshesImportTransform[j];

					unsafe
					{
						Matrix4x4* instanceTransform = null;
						Vector4* instanceDiffuseColor = null;
						Vector3* instanceEmissiveColor = null;
						float* instanceAlphaCursor = null;
						float* instanceVATCursor = null;

						void* currentPtr = m_PerInstanceBuffer[j].ToPointer();
						int instanceCount = m_InstancesCount[j];
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

						for (int i = 0; i < m_InstancesCount[j]; i++)
						{
							Matrix4x4 meshTransform = instanceTransform[i] * t;
							materialProp.SetVector(m_DiffuseColorPropertyName, instanceDiffuseColor[i]);
							if (instanceEmissiveColor != null && !string.IsNullOrEmpty(m_EmissiveColorPropertyName))
								materialProp.SetVector(m_EmissiveColorPropertyName, instanceEmissiveColor[i]);
							if (instanceAlphaCursor != null && !string.IsNullOrEmpty(m_AlphaRemapCursorPropertyName))
								materialProp.SetFloat(m_AlphaRemapCursorPropertyName, instanceAlphaCursor[i]);
							if (instanceVATCursor != null && !string.IsNullOrEmpty(m_VATCursorPropertyName))
								materialProp.SetFloat(m_VATCursorPropertyName, instanceVATCursor[i]);
							Graphics.DrawMesh(m, meshTransform, m_Material, 0, null, 0, materialProp, m_CastShadow, isLit);
						}
					}
				}
			}
		}
	}
}