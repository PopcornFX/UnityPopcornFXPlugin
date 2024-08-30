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

		public static string EBaseTypeIDToString(EBaseTypeID id)
		{
			switch (id)
			{
				case (EBaseTypeID.BaseType_Evolved):
					return "Evolved";
				case (EBaseTypeID.BaseType_Void):
					return "Void";
				case (EBaseTypeID.BaseType_Bool):
					return "Bool";
				case (EBaseTypeID.BaseType_Bool2):
					return "Bool2";
				case (EBaseTypeID.BaseType_Bool3):
					return "Bool3";
				case (EBaseTypeID.BaseType_Bool4):
					return "Bool4";
				case (EBaseTypeID.BaseType_U8):
					return "UByte";
				case (EBaseTypeID.BaseType_UByte2):
					return "UByte2";
				case (EBaseTypeID.BaseType_UByte3):
					return "UByte3";
				case (EBaseTypeID.BaseType_UByte4):
					return "UByte4";
				case (EBaseTypeID.BaseType_I8):
					return "Byte";
				case (EBaseTypeID.BaseType_Byte2):
					return "Byte2";
				case (EBaseTypeID.BaseType_Byte3):
					return "Byte3";
				case (EBaseTypeID.BaseType_Byte4):
					return "Byte4";
				case (EBaseTypeID.BaseType_U16):
					return "UWord";
				case (EBaseTypeID.BaseType_UWord2):
					return "UWord2";
				case (EBaseTypeID.BaseType_UWord3):
					return "UWord3";
				case (EBaseTypeID.BaseType_UWord4):
					return "UWord4";
				case (EBaseTypeID.BaseType_I16):
					return "Word";
				case (EBaseTypeID.BaseType_Word2):
					return "Word2";
				case (EBaseTypeID.BaseType_Word3):
					return "Word3";
				case (EBaseTypeID.BaseType_Word4):
					return "Word4";
				case (EBaseTypeID.BaseType_U32):
					return "UInt";
				case (EBaseTypeID.BaseType_UInt2):
					return "UInt2";
				case (EBaseTypeID.BaseType_UInt3):
					return "UInt3";
				case (EBaseTypeID.BaseType_UInt4):
					return "UInt4";
				case (EBaseTypeID.BaseType_I32):
					return "Int";
				case (EBaseTypeID.BaseType_Int2):
					return "Int2";
				case (EBaseTypeID.BaseType_Int3):
					return "Int3";
				case (EBaseTypeID.BaseType_Int4):
					return "Int4";
				case (EBaseTypeID.BaseType_U64):
					return "UInt64";
				case (EBaseTypeID.BaseType_I64):
					return "Int64";
				case (EBaseTypeID.BaseType_Float):
					return "Float";
				case (EBaseTypeID.BaseType_Float2):
					return "Float2";
				case (EBaseTypeID.BaseType_Float3):
					return "Float3";
				case (EBaseTypeID.BaseType_Float4):
					return "Float4";
				case (EBaseTypeID.BaseType_Double):
					return "Double";
				case (EBaseTypeID.BaseType_Quaternion):
					return "Quaternion";
				default:
					return "Type not supported";

			}
		}
	}
}
