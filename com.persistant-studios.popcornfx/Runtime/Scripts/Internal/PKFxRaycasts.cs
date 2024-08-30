//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using AOT;
using UnityEngine;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using Unity.Jobs;
using Unity.Jobs.LowLevel.Unsafe;

#if UNITY_EDITOR
using System.Reflection;
#endif

namespace PopcornFX
{
	public class PKFxRaycasts
	{
		private class SpherecastBuffers
		{
			public NativeArray<RaycastHit>			m_Results;
			public NativeArray<SpherecastCommand>	m_Commands;
			public int m_Size = 0;

			private SpherecastBuffers() { }

			public SpherecastBuffers(int count)
			{
				m_Size = _NextOrEqualPowerOfTwo(count);
				m_Results = new NativeArray<RaycastHit>(m_Size, Allocator.Persistent);//, NativeArrayOptions.UninitializedMemory);
				m_Commands = new NativeArray<SpherecastCommand>(m_Size, Allocator.Persistent);//, NativeArrayOptions.UninitializedMemory);
			}

			~SpherecastBuffers()
			{
				DisposeIFN();
			}

			public void DisposeIFN()
			{
				if (m_Size > 0)
				{
					if (m_Results.Length != 0)
						m_Results.Dispose();
					if (m_Commands.Length != 0)
						m_Commands.Dispose();
				}
				m_Size = 0;
			}

			private int _NextOrEqualPowerOfTwo(int value)
			{
				value--;
				for (int i = 1; i < sizeof(int) * 8; i += i)
				{
					value |= value >> i;
				}
				value++;
				return value;
			}
		}

		private class RaycastBuffers
		{
			public NativeArray<RaycastHit> m_Results;
			public NativeArray<RaycastCommand> m_Commands;
			public int m_Size = 0;

			private RaycastBuffers() { }

			public RaycastBuffers(int count)
			{
				m_Size = _NextOrEqualPowerOfTwo(count);
				m_Results = new NativeArray<RaycastHit>(m_Size, Allocator.Persistent);//, NativeArrayOptions.UninitializedMemory);
				m_Commands = new NativeArray<RaycastCommand>(m_Size, Allocator.Persistent);//, NativeArrayOptions.UninitializedMemory);
			}

			~RaycastBuffers()
			{
				DisposeIFN();
			}

			public void DisposeIFN()
			{
				if (m_Size > 0)
				{
					if (m_Results.Length != 0)
						m_Results.Dispose();
					if (m_Commands.Length != 0)
						m_Commands.Dispose();
				}
				m_Size = 0;
			}

#if UNITY_EDITOR
			// To debug IFN
			public static void PrintStructsMemoryLayout()
			{
				{
					string memoryStr = typeof(RaycastCommand).Name + "\n";
					FieldInfo[] fields = typeof(RaycastCommand).GetFields(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
					foreach (var f in fields)
						memoryStr += f.Name + " " + f.FieldType + "\n";
					memoryStr += "SizeOf: " + UnsafeUtility.SizeOf<RaycastCommand>() + "\n";
					memoryStr += "Align: " + UnsafeUtility.AlignOf<RaycastCommand>() + "\n";
					Debug.Log(memoryStr);
				}

				{
					string memoryStr = typeof(RaycastHit).Name + "\n";
					FieldInfo[] fields = typeof(RaycastHit).GetFields(BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
					foreach (var f in fields)
						memoryStr += f.Name + " " + f.FieldType + "\n";
					memoryStr += "SizeOf: " + UnsafeUtility.SizeOf<RaycastHit>() + "\n";
					memoryStr += "Align: " + UnsafeUtility.AlignOf<RaycastHit>() + "\n";
					Debug.Log(memoryStr);
				}
			}
#endif

			private int	_NextOrEqualPowerOfTwo(int value)
			{
				value--;
				for (int i = 1; i < sizeof(int) * 8; i += i)
				{
					value |= value >> i;
				}
				value++;
				return value;
			}
		}

		static RaycastBuffers		m_Buffers = null;
		static SpherecastBuffers	m_SphereBuffers = null;
		static int					m_Count = 0;

		public static void Clear()
		{
			if (m_Buffers != null)
			{
				m_Buffers.DisposeIFN();
				m_Buffers = null;
			}
			if (m_SphereBuffers != null)
			{
				m_SphereBuffers.DisposeIFN();
				m_SphereBuffers = null;
			}
			m_Count = 0;
		}
		

		public delegate void RaycastStartCallback(int count, ref IntPtr raycastCommand);
		[MonoPInvokeCallback(typeof(RaycastStartCallback))]
		public static void OnRaycastStart(int count, ref IntPtr raycastCommand)
		{
			m_Count = count;
			if (m_Buffers == null || m_Buffers.m_Size < count)
			{
				if (m_Buffers != null)
					m_Buffers.DisposeIFN();
				m_Buffers = new RaycastBuffers(count);
			}

			unsafe
			{
				raycastCommand = (IntPtr)NativeArrayUnsafeUtility.GetUnsafeBufferPointerWithoutChecks(m_Buffers.m_Commands);
			}
		}

		public delegate void RaycastPackCallback(ref IntPtr raycastResult);
		[MonoPInvokeCallback(typeof(RaycastPackCallback))]
		public static void OnRaycastPack(ref IntPtr raycastResult)
		{
			if (PKFxSettings.CustomRaycast != null)
			{
				PKFxSettings.CustomRaycast(ref raycastResult);
				return;
			}
			unsafe
			{
				int commandsPerJob = Mathf.Max(m_Count / JobsUtility.JobWorkerCount, 1);
#if UNITY_2022_3_OR_NEWER
				JobHandle handle = RaycastCommand.ScheduleBatch(m_Buffers.m_Commands.GetSubArray(0, m_Count), m_Buffers.m_Results.GetSubArray(0, m_Count), commandsPerJob, 1/*Max hits*/, default(JobHandle));
#else
				JobHandle handle = RaycastCommand.ScheduleBatch(m_Buffers.m_Commands.GetSubArray(0, m_Count), m_Buffers.m_Results.GetSubArray(0, m_Count), commandsPerJob, default(JobHandle));
#endif

				handle.Complete();

				raycastResult = (IntPtr)NativeArrayUnsafeUtility.GetUnsafeBufferPointerWithoutChecks(m_Buffers.m_Results);
			}
		}

		public delegate void SpherecastStartCallback(int count, ref IntPtr raycastCommand);
		[MonoPInvokeCallback(typeof(SpherecastStartCallback))]
		public static void OnSpherecastStart(int count, ref IntPtr raycastCommand)
		{
			m_Count = count;
			if (m_SphereBuffers == null || m_SphereBuffers.m_Size < count)
			{
				if (m_SphereBuffers != null)
					m_SphereBuffers.DisposeIFN();
				m_SphereBuffers = new SpherecastBuffers(count);
			}

			unsafe
			{
				raycastCommand = (IntPtr)NativeArrayUnsafeUtility.GetUnsafeBufferPointerWithoutChecks(m_SphereBuffers.m_Commands);
			}
		}

		public delegate void SpherecastPackCallback(ref IntPtr raycastResult);
		[MonoPInvokeCallback(typeof(SpherecastPackCallback))]
		public static void OnSpherecastPack(ref IntPtr raycastResult)
		{
			if (PKFxSettings.CustomSpherecast != null)
			{
				PKFxSettings.CustomSpherecast(ref raycastResult);
				return;
			}
			unsafe
			{
				int commandsPerJob = Mathf.Max(m_Count / JobsUtility.JobWorkerCount, 1);
#if UNITY_2022_3_OR_NEWER
				JobHandle handle = SpherecastCommand.ScheduleBatch(m_SphereBuffers.m_Commands.GetSubArray(0, m_Count), m_SphereBuffers.m_Results.GetSubArray(0, m_Count), commandsPerJob, 1/*Max hits*/, default(JobHandle));
#else
				JobHandle handle = SpherecastCommand.ScheduleBatch(m_SphereBuffers.m_Commands.GetSubArray(0, m_Count), m_SphereBuffers.m_Results.GetSubArray(0, m_Count), commandsPerJob, default(JobHandle));
#endif

				handle.Complete();

				raycastResult = (IntPtr)NativeArrayUnsafeUtility.GetUnsafeBufferPointerWithoutChecks(m_SphereBuffers.m_Results);
			}
		}

	}
}