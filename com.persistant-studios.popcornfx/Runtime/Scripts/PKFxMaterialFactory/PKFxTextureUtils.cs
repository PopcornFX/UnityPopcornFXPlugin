//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Runtime.InteropServices;
using UnityEngine;
using static PopcornFX.PKFxEmitter;

namespace PopcornFX
{
	internal enum EImageFormat : int
	{
		Invalid = 0,
		BGR8 = 3,
		BGRA8,
		BGRA8_sRGB,
		DXT1 = 8,
		DXT1_sRGB,
		DXT3,
		DXT3_sRGB,
		DXT5,
		DXT5_sRGB,
		RGB8_ETC1 = 16,
		RGB8_ETC2,
		RGBA8_ETC2,
		RGB8A1_ETC2,
		RGB4_PVRTC1,
		RGB2_PVRTC1,
		RGBA4_PVRTC1,
		RGBA2_PVRTC1,
		Fp32_RGBA = 26
	}

	internal static class PKImageConverter
	{
		public static void ARGB2BGRA(ref byte[] data)
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
	
		public static void RGBA2BGRA(ref byte[] data)
		{
			for (int id = 0; id < data.Length; id += 4)
			{
				byte tmp = data[id];
				data[id] = data[id + 2];
				data[id + 2] = tmp;
			}
		}
	
		public static void RGB2BGR(ref byte[] data)
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
		internal static EImageFormat ResolveImageFormat(Texture2D t, ref byte[] data)
		{
			if (t.format == TextureFormat.DXT1)
				return EImageFormat.DXT1;
			else if (t.format == TextureFormat.DXT5)
				return EImageFormat.DXT5;
			else if (t.format == TextureFormat.ARGB32)
			{
				PKImageConverter.ARGB2BGRA(ref data);
				return EImageFormat.BGRA8;
			}
			else if (t.format == TextureFormat.RGBA32)
			{
				PKImageConverter.RGBA2BGRA(ref data);
				return EImageFormat.BGRA8;
			}
			else if (t.format == TextureFormat.BGRA32)
				return EImageFormat.BGRA8;
			else if (t.format == TextureFormat.RGB24)
			{
				PKImageConverter.RGB2BGR(ref data);
				return EImageFormat.BGR8;
			}
			else if (t.format == TextureFormat.PVRTC_RGB4)
				return EImageFormat.RGB4_PVRTC1;
			else if (t.format == TextureFormat.PVRTC_RGBA4)
				return EImageFormat.RGBA4_PVRTC1;
			else if (t.format == TextureFormat.PVRTC_RGB2)
				return EImageFormat.RGB2_PVRTC1;
			else if (t.format == TextureFormat.PVRTC_RGBA2)
				return EImageFormat.RGBA2_PVRTC1;
			else if (t.format == TextureFormat.ETC_RGB4)
				return EImageFormat.RGB8_ETC1;
			else if (t.format == TextureFormat.ETC2_RGB)
				return EImageFormat.RGB8_ETC2;
			else if (t.format == TextureFormat.ETC2_RGBA8)
				return EImageFormat.RGBA8_ETC2;
			else if (t.format == TextureFormat.ETC2_RGBA1)
				return EImageFormat.RGB8A1_ETC2;
			else if (t.format == TextureFormat.RGBAFloat)
				return EImageFormat.Fp32_RGBA;
			else
			{
				Debug.LogError("[PopcornFX] " + t.name + " texture format not supported : " + t.format);
				return EImageFormat.Invalid;
			}
		}

		//----------------------------------------------------------------------------

		internal static IntPtr GetAndUpdateTextureToFill(Texture2D texture, Sampler.ETexcoordMode wrapMode)
		{
			byte[] data = GetRawTextureData(texture);
			EImageFormat imageFormat = ResolveImageFormat(texture, ref data);
			IntPtr textureToFillPtr = PKFxManager.GetTextureSamplerToFill(data.Length);

			unsafe
			{
				STextureSamplerToFill* textureToFill = (STextureSamplerToFill*)textureToFillPtr.ToPointer();

				if (textureToFill->m_TextureData == IntPtr.Zero ||
					imageFormat == EImageFormat.Invalid)
					return IntPtr.Zero;

				Marshal.Copy(data, 0, textureToFill->m_TextureData, data.Length);
				textureToFill->m_Width = texture.width;
				textureToFill->m_Height = texture.height;
				textureToFill->m_PixelFormat = (int)imageFormat;
				textureToFill->m_WrapMode = (int)wrapMode;
			}
			return textureToFillPtr;
		}

		public static byte[] GetRawTextureData(Texture2D texture)
		{
			byte[] data = texture.GetRawTextureData();

			if (data == null || data.Length == 0)
				Debug.LogError("[PopcornFX] Please check the 'Read/Write' setting for the texture '" + texture.name + "'.");
			return data;
		}
	}
}
