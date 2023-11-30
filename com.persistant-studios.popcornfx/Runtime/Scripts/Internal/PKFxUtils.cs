//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PopcornFX
{
	public enum EAttributeType : int
	{
		Bool = 1,
		Bool2,
		Bool3,
		Bool4,
		Int = 25,
		Int2,
		Int3,
		Int4,
		Float = 31,
		Float2,
		Float3,
		Float4,
		Quaternion = 36
	}

	public enum EAttributeSemantic : int
	{
		None = 0,
		Coordinate3D,
		Scale3D,
		Color,
		Angle,
		Visibility,
	}

	public enum EAttributeDropMode : int
	{
		None = 0,
		SingleSelect,
		MultiSelect
	}

	internal struct Int4
	{
		public int x;
		public int y;
		public int z;
		public int w;

		public Int4(float _x, float _y, float _z, float _w)
		{
			x = PKFxUtils.Float2Int(_x);
			y = PKFxUtils.Float2Int(_y);
			z = PKFxUtils.Float2Int(_z);
			w = PKFxUtils.Float2Int(_w);
		}

		public Int4(int _x, int _y, int _z, int _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
		public Int4(Int4 oth)
		{
			x = oth.x;
			y = oth.y;
			z = oth.z;
			w = oth.w;
		}

		public static bool operator ==(Int4 f, Int4 s)
		{
			return (s.x == f.x &&
					s.y == f.y &&
					s.z == f.z &&
					s.w == f.w);
		}
		public static bool operator !=(Int4 f, Int4 s)
		{
			return !(s.x == f.x ||
					 s.y == f.y ||
					 s.z == f.z ||
					 s.w == f.w);

		}
		public override bool Equals(System.Object oth)
		{
			if (!(oth is Int4))
				return false;
			Int4 v = (Int4)oth;
			return x == v.x &&
					y == v.y &&
					z == v.z &&
					w == v.w;
		}

		public override int GetHashCode()
		{
			return x.GetHashCode() ^
					y.GetHashCode() ^
					z.GetHashCode() ^
					w.GetHashCode();
		}
	}

	//----------------------------------------------------------------------------
	internal class PKFxUtils
	{
		public static int Float2Int(float fff)
		{
			return BitConverter.ToInt32(BitConverter.GetBytes(fff), 0);
		}

		public static float Int2Float(int i)
		{
			return BitConverter.ToSingle(BitConverter.GetBytes(i), 0);
		}

		public static bool ArrayContains(string[] array, string s)
		{
			foreach (string _s in array)
			{
				if (_s == s)
					return true;
			}
			return false;
		}

		//----------------------------------------------------------------------------

		public static T[] ForFill<T>(T value, int nbElts)
		{
			var a = new T[nbElts];
			for (int i = 0; i < nbElts; ++i)
			{
				a[i] = value;
			}
			return a;
		}

		public static float Linear2sRGB(float channel)
		{
#if true //ACCURATE_SRGB
			if (channel <= 0.0031308f)
				return channel * 12.92f;
			else
				return 1.055f * Mathf.Pow(channel, 1 / 2.4f) - 0.055f;
#else
			return Mathf.Pow(channel, 1 / 2.2f);
#endif
		}

		public static float sRGB2Linear(float channel)
		{
#if true //ACCURATE_SRGB
			if (channel <= 0.04045f)
				return channel / 12.92f;
			else
				return Mathf.Pow((channel + 0.055f) / 1.055f, 2.4f);
#else
			return Mathf.Pow(channel, 2.2f);
#endif
		}
	}
}
