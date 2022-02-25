//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace PopcornFX
{

	// Allocates a pinned buffer and frees it when destroyed
	internal class PinnedData
	{
		public IntPtr m_PinnedData = IntPtr.Zero;

		public void PinData(byte[] rawData, int size)
		{
			m_PinnedData = Marshal.AllocHGlobal(size);
			Marshal.Copy(rawData, 0, m_PinnedData, size);
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

		public static IntPtr PinAsset(PKFxAsset asset)
		{
			var pinnedData = new PinnedAssetData(asset);
			if (PKFxManagerImpl.m_NeedsFreeing == null)
				PKFxManagerImpl.m_NeedsFreeing = new List<PinnedData>();
			PKFxManagerImpl.m_NeedsFreeing.Add(pinnedData);
			return pinnedData.m_PinnedData;
		}

		//----------------------------------------------------------------------------

		public static IntPtr PinAsset(PKFxRuntimeMeshAsset asset)
		{
			var pinnedData = new PinnedRuntimeMeshAssetData(asset);
			if (PKFxManagerImpl.m_NeedsFreeing == null)
				PKFxManagerImpl.m_NeedsFreeing = new List<PinnedData>();
			PKFxManagerImpl.m_NeedsFreeing.Add(pinnedData);
			return pinnedData.m_PinnedData;
		}

		//----------------------------------------------------------------------------

		public static ulong PinTexture(Texture2D tex, ref IntPtr handler)
		{
			var pinnedData = new PinnedTexData(tex);
			if (PKFxManagerImpl.m_NeedsFreeing == null)
				PKFxManagerImpl.m_NeedsFreeing = new List<PinnedData>();
			PKFxManagerImpl.m_NeedsFreeing.Add(pinnedData);
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
			//PinData(asset.m_Data, size);
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
			//PinData(asset.m_Data, size);
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
			byte[] imgData = PKFxTextureUtils.GetRawTextureData(m_Asset);
			byte[] size = BitConverter.GetBytes(imgData.Length);
			byte[] format = BitConverter.GetBytes((int)PKFxTextureUtils.ResolveImageFormat(m_Asset, ref imgData));

			Array.Copy(width, header, width.Length);
			Array.Copy(height, 0, header, width.Length, height.Length);
			Array.Copy(size, 0, header, width.Length + height.Length, size.Length);
			Array.Copy(format, 0, header, width.Length + height.Length + size.Length, format.Length);

			byte[] data = new byte[header.Length + imgData.Length];
			Array.Copy(header, data, header.Length);
			Array.Copy(imgData, 0, data, header.Length, imgData.Length);

#if NETFX_CORE
				m_DataLength = (ulong)data.Length;
#else
			m_DataLength = (ulong)data.LongLength;
#endif

			if (data.Length != 0)
				PinData(data, data.Length);
			else
				Debug.LogError("[PopcornFX] Sampler " + m_Asset.name + " : Could not get raw texture data.");
		}
	};

}