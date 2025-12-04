//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Runtime.InteropServices;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using UnityEngine;
using static PopcornFX.PKFxEmitter;

namespace PopcornFX
{
	internal enum EImageFormat : int // Duplicate of PopcornFX::CImage::EFormat enum
	{
		Format_Invalid = 0,

		// Basic integer formats:
		Format_Lum8,
		Format_LumAlpha8,
		Format_BGR8,
		Format_BGRA8,
		Format_BGRA8_sRGB,

		// Packed integer format:
		Format_BGRA4,       // 16bpp 4.4.4.4 BGRA Linear
		Format_BGRA4_sRGB,  // 16bpp 4.4.4.4 BGRA sRGB

		// 4 BC/DXT compressed formats:
		Format_DXT1,
		Format_DXT1_sRGB,
		Format_DXT3,
		Format_DXT3_sRGB,
		Format_DXT5,
		Format_DXT5_sRGB,

		Format_BC5_UNorm,
		Format_BC5_SNorm,

		// Mobile-friendly compressed formats:
		Format_RGB8_ETC1,
		Format_RGB8_ETC2,
		Format_RGBA8_ETC2,
		Format_RGB8A1_ETC2,

		Format_RGB4_PVRTC1,
		Format_RGB4_PVRTC1_sRGB,
		Format_RGB2_PVRTC1,
		Format_RGB2_PVRTC1_sRGB,
		Format_RGBA4_PVRTC1,
		Format_RGBA4_PVRTC1_sRGB,
		Format_RGBA2_PVRTC1,
		Format_RGBA2_PVRTC1_sRGB,

		// Fp32 versions of the basic formats:
		Format_Fp32Lum,
		Format_Fp32LumAlpha,
		Format_Fp32RGBA,

		// Fp16 versions of the basic formats:
		Format_Fp16Lum,
		Format_Fp16LumAlpha,
		Format_Fp16RGBA,

		__MaxImageFormats,
	}

	internal static class PKImageConverter
	{
		public static void ARGB2BGRA(ref NativeArray<byte> data)
		{
			for (int id = 0; id < data.Length;)
			{
				byte[] col = new byte[4] { data[id + 3], data[id + 2], data[id + 1], data[id] };
				data[id++] = col[0];
				data[id++] = col[1];
				data[id++] = col[2];
				data[id++] = col[3];
			}
		}
	
		public static void RGBA2BGRA(ref NativeArray<byte> data)
		{
			for (int id = 0; id < data.Length; id += 4)
			{
				byte tmp = data[id];
				data[id] = data[id + 2];
				data[id + 2] = tmp;
			}
		}
	
		public static void RGB2BGR(ref NativeArray<byte> data)
		{
			for (int id = 0; id < data.Length; id += 3)
			{
				byte tmp = data[id];
				data[id] = data[id + 2];
				data[id + 2] = tmp;
			}
		}
	}

	//----------------------------------------------------------------------------
	internal static class PKFxTextureUtils
	{
		//Will reorder data on some image format: ARGB32, RGBA32, RGB24
		internal static EImageFormat ResolveImageFormat(Texture2D t, ref NativeArray<byte> data)
		{
			if (t.format == TextureFormat.DXT1)
				return EImageFormat.Format_DXT1;
			else if (t.format == TextureFormat.DXT5)
				return EImageFormat.Format_DXT5;
			else if (t.format == TextureFormat.ARGB32)
			{
				PKImageConverter.ARGB2BGRA(ref data);
				return EImageFormat.Format_BGRA8;
			}
			else if (t.format == TextureFormat.RGBA32)
			{
				PKImageConverter.RGBA2BGRA(ref data);
				return EImageFormat.Format_BGRA8;
			}
			else if (t.format == TextureFormat.BGRA32)
				return EImageFormat.Format_BGRA8;
			else if (t.format == TextureFormat.RGB24)
			{
				PKImageConverter.RGB2BGR(ref data);
				return EImageFormat.Format_BGR8;
			}
			else if (t.format == TextureFormat.ETC_RGB4)
				return EImageFormat.Format_RGB8_ETC1;
			else if (t.format == TextureFormat.ETC2_RGB)
				return EImageFormat.Format_RGB8_ETC2;
			else if (t.format == TextureFormat.ETC2_RGBA8)
				return EImageFormat.Format_RGBA8_ETC2;
			else if (t.format == TextureFormat.ETC2_RGBA1)
				return EImageFormat.Format_RGB8A1_ETC2;
			else if (t.format == TextureFormat.RGBAFloat)
				return EImageFormat.Format_Fp32RGBA;
			else
			{
				Debug.LogError("[PopcornFX] " + t.name + " texture format not supported : " + t.format);
				return EImageFormat.Format_Invalid;
			}
		}

		//----------------------------------------------------------------------------

		internal static IntPtr GetAndUpdateTextureToFill(Texture2D texture, Sampler.ETexcoordMode wrapMode)
		{
			NativeArray<byte> data = GetRawTextureDataAsNativeArray(texture);
			EImageFormat imageFormat = ResolveImageFormat(texture, ref data);
			IntPtr textureToFillPtr = PKFxManager.GetTextureSamplerToFill(data.Length);

			unsafe
			{
				STextureSamplerToFill* textureToFill = (STextureSamplerToFill*)textureToFillPtr.ToPointer();

				if (textureToFill->m_TextureData == IntPtr.Zero ||
					imageFormat == EImageFormat.Format_Invalid)
					return IntPtr.Zero;

				void* textureDataPtr = (void*)textureToFill->m_TextureData.ToPointer();
				Buffer.MemoryCopy(NativeArrayUnsafeUtility.GetUnsafePtr(data), textureDataPtr, data.Length, data.Length);
				
				textureToFill->m_Width = texture.width;
				textureToFill->m_Height = texture.height;
				textureToFill->m_PixelFormat = (int)imageFormat;
				textureToFill->m_WrapMode = (int)wrapMode;
			}
			return textureToFillPtr;
		}

		public static NativeArray<byte> GetRawTextureDataAsNativeArray(Texture2D texture)
		{
			NativeArray<byte> data = texture.GetRawTextureData<byte>();
			if (data == null || data.Length == 0)
				Debug.LogError("[PopcornFX] Please check the 'Read/Write' setting for the texture '" + texture.name + "'.");
			return data;
		}

		/*
		 * Warning: This funtion will alloc the byte[]
		 */
		public static byte[] GetRawTextureData(Texture2D texture)
		{
			byte[] data = texture.GetRawTextureData();
			if (data == null || data.Length == 0)
				Debug.LogError("[PopcornFX] Please check the 'Read/Write' setting for the texture '" + texture.name + "'.");
			return data;
		}
	}
}
