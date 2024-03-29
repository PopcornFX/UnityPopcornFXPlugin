//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;

namespace PopcornFX
{
	// Allocates a pinned buffer and frees it when destroyed
	internal class PinnedData
	{
		public IntPtr	m_PinnedData = IntPtr.Zero;
		public int		m_RefCount = 0;

		public void PinData(byte[] rawData, int size)
		{
			m_PinnedData = Marshal.AllocHGlobal(size);
			Marshal.Copy(rawData, 0, m_PinnedData, size);
			++m_RefCount;
		}

		public void FreePinnedData()
		{
			if (m_PinnedData != IntPtr.Zero)
			{
				Marshal.FreeHGlobal(m_PinnedData);
				m_PinnedData = IntPtr.Zero;
			}
		}

		~PinnedData()
		{
			FreePinnedData();
		}

		//----------------------------------------------------------------------------

		public static IntPtr PinAsset(PKFxAsset asset, string path)
		{
			Hash128 hash = Hash128.Compute(path);
			if (PKFxManagerImpl.m_PinnedResource.ContainsKey(hash))
			{
				PinnedData existingPinnedData = PKFxManagerImpl.m_PinnedResource[hash];
				++existingPinnedData.m_RefCount;
				return existingPinnedData.m_PinnedData;
			}

			var pinnedData = new PinnedAssetData(asset);
			PKFxManagerImpl.m_PinnedResource.Add(hash, pinnedData);
			return pinnedData.m_PinnedData;
		}

		//----------------------------------------------------------------------------

		public static IntPtr PinAsset(PKFxRuntimeMeshAsset asset, string path)
		{
			Hash128 hash = Hash128.Compute(path);
			if (PKFxManagerImpl.m_PinnedResource.ContainsKey(hash))
			{
				PinnedData existingPinnedData = PKFxManagerImpl.m_PinnedResource[hash];
				++existingPinnedData.m_RefCount;
				return existingPinnedData.m_PinnedData;
			}

			var pinnedData = new PinnedRuntimeMeshAssetData(asset);

			PKFxManagerImpl.m_PinnedResource.Add(hash, pinnedData);
			return pinnedData.m_PinnedData;
		}

		//----------------------------------------------------------------------------

		public static ulong PinTexture(Texture2D tex, ref IntPtr handler, string path)
		{
			Hash128 hash = Hash128.Compute(path);
			if (PKFxManagerImpl.m_PinnedResource.ContainsKey(hash))
			{
				PinnedTexData existingPinnedData = PKFxManagerImpl.m_PinnedResource[hash] as PinnedTexData;
				handler = existingPinnedData.m_PinnedData;
				++existingPinnedData.m_RefCount;
				return existingPinnedData.m_DataLength;
			}
			PinnedTexData pinnedData = new PinnedTexData(tex);
			PKFxManagerImpl.m_PinnedResource.Add(hash, pinnedData);

			handler = pinnedData.m_PinnedData;
			return pinnedData.m_DataLength;
		}
	}

	//----------------------------------------------------------------------------

	class PinnedAssetData : PinnedData
	{
		public PKFxAsset m_Asset;

		public PinnedAssetData(PKFxAsset asset)
		{
			Debug.Assert(asset.m_Data.Length > 0, asset.AssetVirtualPath + " no data");
			int size = Marshal.SizeOf(asset.m_Data[0]) * asset.m_Data.Length;
			m_PinnedData = Marshal.AllocHGlobal(size);
			Marshal.Copy(asset.m_Data, 0, m_PinnedData, size);
			m_Asset = asset;
		}
	};

	//----------------------------------------------------------------------------

	class PinnedRuntimeMeshAssetData : PinnedData
	{
		public PKFxRuntimeMeshAsset m_Asset;

		public PinnedRuntimeMeshAssetData(PKFxRuntimeMeshAsset asset)
		{
			Debug.Assert(asset.m_Data.Length > 0, asset.m_AssetVirtualPath + " no data");
			int size = Marshal.SizeOf(asset.m_Data[0]) * asset.m_Data.Length;
			m_PinnedData = Marshal.AllocHGlobal(size);
			Marshal.Copy(asset.m_Data, 0, m_PinnedData, size);
			m_Asset = asset;
		}
	};

	//----------------------------------------------------------------------------


	class PinnedTexData : PinnedData
	{
		public Texture2D m_Asset;
		public ulong m_DataLength;

		public PinnedTexData(Texture2D t)
		{
			m_Asset = t;
			byte[] header = new byte[16];
			byte[] width = BitConverter.GetBytes(m_Asset.width);
			byte[] height = BitConverter.GetBytes(m_Asset.height);
			NativeArray<byte> imgData = PKFxTextureUtils.GetRawTextureDataAsNativeArray(m_Asset); //Get ref of texture data
			byte[] size = BitConverter.GetBytes(imgData.Length); 
			byte[] format = BitConverter.GetBytes((int)PKFxTextureUtils.ResolveImageFormat(m_Asset, ref imgData));

			Array.Copy(width, header, width.Length);
			Array.Copy(height, 0, header, width.Length, height.Length);
			Array.Copy(size, 0, header, width.Length + height.Length, size.Length);
			Array.Copy(format, 0, header, width.Length + height.Length + size.Length, format.Length);

			ulong dataSize = Convert.ToUInt64(header.Length + imgData.Length);

			m_PinnedData = Marshal.AllocHGlobal(header.Length + imgData.Length);
			Marshal.Copy(header, 0, m_PinnedData, header.Length);

			unsafe
			{
				void *textureDataPtr = (void*)IntPtr.Add(m_PinnedData, header.Length).ToPointer();
				Buffer.MemoryCopy(NativeArrayUnsafeUtility.GetUnsafePtr(imgData), textureDataPtr, imgData.Length, imgData.Length);
			}

#if NETFX_CORE
			m_DataLength = dataSize;
#else
			m_DataLength = dataSize;
#endif
		}
	};

}