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

		public string m_ColorPropertyName;
		public string m_CursorPropertyName = "_VATCursor";

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


			public NativeArray<Matrix4x4> transforms;
			public NativeArray<Vector4> colors;
			public NativeArray<float> cursors;

			public void Execute(int h)
			{
				unsafe
				{
					Matrix4x4* instanceTransform = (Matrix4x4*)buffer.ToPointer();
					Vector4* instanceColor = (Vector4*)(instanceTransform + count);
					float* instanceCursor = (float*)(instanceColor + count);

					//Matrix4x4 Multiply is 90% of the task
					transforms[h] = instanceTransform[offset + h] * meshTransform;
					colors[h] = instanceColor[offset + h];
					cursors[h] = instanceCursor[offset + h];
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
			if (m_ColorPropertyName == null || m_ColorPropertyName.Length == 0)
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
					NativeArray<Vector4> colors = new NativeArray<Vector4>(1023, Allocator.TempJob);
					NativeArray<float> cursors = new NativeArray<float>(1023, Allocator.TempJob);

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
								job.colors = colors;
								job.transforms = transforms;
								job.cursors = cursors;
								//In data
								job.meshTransform = t;
								job.offset = i;
								job.buffer = m_PerInstanceBuffer[j];
								job.count = m_InstancesCount[j];

								int dataLeft = Math.Min(m_InstancesCount[j] - i, 1023);

								// Schedule the job with one Execute per index in the results array and only 1 item per processing batch
								JobHandle handle = job.Schedule(Math.Min(dataLeft, 1023), 1);

								// Wait for the job to complete
								handle.Complete();

								generalOffset += dataLeft;
								materialProp.SetVectorArray(m_ColorPropertyName, colors.ToArray());
								materialProp.SetFloatArray(m_CursorPropertyName, cursors.ToArray());
								Graphics.DrawMeshInstanced(m, 0, m_Material, transforms.ToArray(), dataLeft, materialProp, m_CastShadow ? UnityEngine.Rendering.ShadowCastingMode.On : UnityEngine.Rendering.ShadowCastingMode.Off);
							}
						}
					}
					transforms.Dispose();
					colors.Dispose();
					cursors.Dispose();
				}
			}
			else
			{
				for (int j = 0; j < m_Meshes.Length; ++j)
				{
					MaterialPropertyBlock materialProp = new MaterialPropertyBlock();
					Mesh m = m_Meshes[j];
					Matrix4x4 t = m_MeshesImportTransform[j];

					for (int i = 0; i < m_InstancesCount[j]; i++)
					{
						Matrix4x4 transform;
						Vector4 color;
						float cursor;

						unsafe
						{
							Matrix4x4* instanceTransform = (Matrix4x4*)m_PerInstanceBuffer[j].ToPointer();
							Vector4* instanceColor = (Vector4*)(instanceTransform + m_InstancesCount[j]);
							float* instanceCursor = (float*)(instanceColor + m_InstancesCount[j]);

							transform = instanceTransform[i] * t;
							color = instanceColor[i];
							cursor = instanceCursor[i];
						}
						materialProp.SetVector(m_ColorPropertyName, color);
						materialProp.SetFloat(m_CursorPropertyName, cursor);
						Graphics.DrawMesh(m, transform, m_Material, 0, null, 0, materialProp, m_CastShadow);
					}
				}
			}
		}
	}
}