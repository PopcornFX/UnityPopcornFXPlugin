//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using System;
using System.Runtime.InteropServices;

namespace PopcornFX
{
	// Public structure to access the attribute types:
	// For float4 we just use Vector4
	public class Vector4Int
	{
		private int		m_X;
		private int		m_Y;
		private int		m_Z;
		private int		m_W;

		public int x { get { return m_X; } set { m_X = value; } }
		public int y { get { return m_Y; } set { m_Y = value; } }
		public int z { get { return m_Z; } set { m_Z = value; } }
		public int w { get { return m_W; } set { m_W = value; } }

		public Vector4Int(int v)
		{
			m_X = m_Y = m_Z = m_W = v;
		}

		public Vector4Int(int x, int y, int z, int w)
		{
			m_X = x;
			m_Y = y;
			m_Z = z;
			m_W = w;
		}
	}

	public class Vector2Bool
	{
		private bool m_X;
		private bool m_Y;

		public bool x { get { return m_X; } set { m_X = value; } }
		public bool y { get { return m_Y; } set { m_Y = value; } }

		public Vector2Bool(bool v)
		{
			m_X = m_Y = v;
		}

		public Vector2Bool(bool x, bool y)
		{
			m_X = x;
			m_Y = y;
		}
	}

	public class Vector3Bool
	{
		private bool m_X;
		private bool m_Y;
		private bool m_Z;

		public bool x { get { return m_X; } set { m_X = value; } }
		public bool y { get { return m_Y; } set { m_Y = value; } }
		public bool z { get { return m_Z; } set { m_Z = value; } }

		public Vector3Bool(bool v)
		{
			m_X = m_Y = m_Z = v;
		}

		public Vector3Bool(bool x, bool y, bool z)
		{
			m_X = x;
			m_Y = y;
			m_Z = z;
		}
	}

	public class Vector4Bool
	{
		private bool		m_X;
		private bool		m_Y;
		private bool		m_Z;
		private bool		m_W;

		public bool x { get { return m_X; } set { m_X = value; } }
		public bool y { get { return m_Y; } set { m_Y = value; } }
		public bool z { get { return m_Z; } set { m_Z = value; } }
		public bool w { get { return m_W; } set { m_W = value; } }

		public Vector4Bool(bool v)
		{
			m_X = m_Y = m_Z = m_W = v;
		}

		public Vector4Bool(bool x, bool y, bool z, bool w)
		{
			m_X = x;
			m_Y = y;
			m_Z = z;
			m_W = w;
		}
	}

	[StructLayout(LayoutKind.Explicit)]
	[Serializable]
	public struct SGenericTypedValueContainer
	{
		//C# Union
		[FieldOffset(0), SerializeField] public int i1; //Or
		[FieldOffset(0), SerializeField] public float f1; //Or
		[FieldOffset(0), NonSerialized] public byte b1;
		[FieldOffset(1), NonSerialized] public byte b2;
		[FieldOffset(2), NonSerialized] public byte b3;
		[FieldOffset(3), NonSerialized] public byte b4;

		public byte[] ToByteArray()
		{
			byte[] result = new byte[4];

			result[0] = b1;
			result[1] = b2;
			result[2] = b3;
			result[3] = b4;
			return result;
		}

		internal void WriteToByteArray(ref byte[] array, int startIdx)
		{
			array[startIdx + 0] = b1;
			array[startIdx + 1] = b2;
			array[startIdx + 2] = b3;
			array[startIdx + 3] = b4;
		}
	}

	[Serializable]
	public struct SAttribContainer_Vector4
	{
		public SGenericTypedValueContainer x;
		public SGenericTypedValueContainer y;
		public SGenericTypedValueContainer z;
		public SGenericTypedValueContainer w;
	}

	public class PKFxAttributesContainer
	{
		private bool m_WasModified;
		private IntPtr m_AttributesBuffer;
		private PKFxEffectAsset m_FxAsset;
		private int m_FXGuid;

		public PKFxAttributesContainer(PKFxEffectAsset asset, int fxId)
		{
			m_FXGuid = fxId;
			m_WasModified = false;
			m_FxAsset = asset;
			m_AttributesBuffer = PKFxManager.GetAttributesBuffer(fxId);
		}

		#region SetAttributeSafe from name

		public void SetAttributeSafe(string name, float valueX)
		{
			SetAttributeSafe(m_FxAsset.AttributeIdFromName(name), valueX);
		}

		public void SetAttributeSafe(string name, float valueX, float valueY)
		{
			SetAttributeSafe(m_FxAsset.AttributeIdFromName(name), valueX, valueY);
		}

		public void SetAttributeSafe(string name, float valueX, float valueY, float valueZ)
		{
			SetAttributeSafe(m_FxAsset.AttributeIdFromName(name), valueX, valueY, valueZ);
		}

		public void SetAttributeSafe(string name, float valueX, float valueY, float valueZ, float valueW)
		{
			SetAttributeSafe(m_FxAsset.AttributeIdFromName(name), valueX, valueY, valueZ, valueW);
		}

		public void SetAttributeSafe(string name, int valueX)
		{
			SetAttributeSafe(m_FxAsset.AttributeIdFromName(name), valueX);
		}

		public void SetAttributeSafe(string name, int valueX, int valueY)
		{
			SetAttributeSafe(m_FxAsset.AttributeIdFromName(name), valueX, valueY);
		}

		public void SetAttributeSafe(string name, int valueX, int valueY, int valueZ)
		{
			SetAttributeSafe(m_FxAsset.AttributeIdFromName(name), valueX, valueY, valueZ);
		}

		public void SetAttributeSafe(string name, int valueX, int valueY, int valueZ, int valueW)
		{
			SetAttributeSafe(m_FxAsset.AttributeIdFromName(name), valueX, valueY, valueZ, valueW);
		}

		#endregion

		#region SetAttributeSafe from ID

		public void SetAttributeSafe(int attribId, float valueX)
		{
			if (attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;
			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Float)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute");
				return;
			}
			if (attributeDesc.HasMin())
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.f1, valueX);
			if (attributeDesc.HasMax())
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.f1, valueX);
			SetAttributeUnsafe(attribId, valueX);
		}

		public void SetAttributeSafe(int attribId, float valueX, float valueY)
		{
			if (attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;
			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Float2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute");
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.f1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.f1, valueY);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.f1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.f1, valueY);
			}
			SetAttributeUnsafe(attribId, valueX, valueY);
		}

		public void SetAttributeSafe(int attribId, float valueX, float valueY, float valueZ)
		{
			if (attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;
			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Float3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute");
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.f1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.f1, valueY);
				valueZ = Mathf.Max(attributeDesc.m_MinValue.z.f1, valueZ);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.f1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.f1, valueY);
				valueZ = Mathf.Min(attributeDesc.m_MaxValue.z.f1, valueZ);
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ);
		}

		public void SetAttributeSafe(int attribId, float valueX, float valueY, float valueZ, float valueW)
		{
			if (attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;
			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Float4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute");
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.f1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.f1, valueY);
				valueZ = Mathf.Max(attributeDesc.m_MinValue.z.f1, valueZ);
				valueW = Mathf.Max(attributeDesc.m_MinValue.w.f1, valueW);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.f1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.f1, valueY);
				valueZ = Mathf.Min(attributeDesc.m_MaxValue.z.f1, valueZ);
				valueW = Mathf.Min(attributeDesc.m_MaxValue.w.f1, valueW);
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ, valueW);
		}

		public void SetAttributeSafe(int attribId, int valueX)
		{
			if (attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;
			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Int)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute");
				return;
			}
			if (attributeDesc.HasMin())
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.i1, valueX);
			if (attributeDesc.HasMax())
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.i1, valueX);
			SetAttributeUnsafe(attribId, valueX);
		}

		public void SetAttributeSafe(int attribId, int valueX, int valueY)
		{
			if (attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;
			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Int2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute");
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.i1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.i1, valueY);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.i1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.i1, valueY);
			}
			SetAttributeUnsafe(attribId, valueX, valueY);
		}

		public void SetAttributeSafe(int attribId, int valueX, int valueY, int valueZ)
		{
			if (attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;
			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Int3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute");
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.i1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.i1, valueY);
				valueZ = Mathf.Max(attributeDesc.m_MinValue.z.i1, valueZ);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.i1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.i1, valueY);
				valueZ = Mathf.Min(attributeDesc.m_MaxValue.z.i1, valueZ);
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ);
		}

		public void SetAttributeSafe(int attribId, int valueX, int valueY, int valueZ, int valueW)
		{
			if (attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;
			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Int4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute");
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.i1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.i1, valueY);
				valueZ = Mathf.Max(attributeDesc.m_MinValue.z.i1, valueZ);
				valueW = Mathf.Max(attributeDesc.m_MinValue.w.i1, valueW);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.i1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.i1, valueY);
				valueZ = Mathf.Min(attributeDesc.m_MaxValue.z.i1, valueZ);
				valueW = Mathf.Min(attributeDesc.m_MaxValue.w.i1, valueW);
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ, valueW);
		}

		#endregion

		#region SetAttributeUnsafe

		public void SetAttributeUnsafe(int attribId, float valueX)
		{
			m_WasModified = true;
			unsafe
			{
				float* attribPtr = (float*)m_AttributesBuffer.ToPointer() + attribId * 4;
				attribPtr[0] = valueX;
			}
		}

		public void SetAttributeUnsafe(int attribId, float valueX, float valueY)
		{
			m_WasModified = true;
			unsafe
			{
				float* attribPtr = (float*)m_AttributesBuffer.ToPointer() + attribId * 4;
				attribPtr[0] = valueX;
				attribPtr[1] = valueY;
			}
		}

		public void SetAttributeUnsafe(int attribId, float valueX, float valueY, float valueZ)
		{
			m_WasModified = true;
			unsafe
			{
				float* attribPtr = (float*)m_AttributesBuffer.ToPointer() + attribId * 4;
				attribPtr[0] = valueX;
				attribPtr[1] = valueY;
				attribPtr[2] = valueZ;
			}
		}

		public void SetAttributeUnsafe(int attribId, float valueX, float valueY, float valueZ, float valueW)
		{
			m_WasModified = true;
			unsafe
			{
				float* attribPtr = (float*)m_AttributesBuffer.ToPointer() + attribId * 4;
				attribPtr[0] = valueX;
				attribPtr[1] = valueY;
				attribPtr[2] = valueZ;
				attribPtr[3] = valueW;
			}
		}

		public void SetAttributeUnsafe(int attribId, Vector4 value)
		{
			m_WasModified = true;
			unsafe
			{
				Vector4* attribPtr = (Vector4*)m_AttributesBuffer.ToPointer() + attribId;
				attribPtr[0] = value;
			}
		}

		public void SetAttributeUnsafe(int attribId, int valueX)
		{
			m_WasModified = true;
			unsafe
			{
				int* attribPtr = (int*)m_AttributesBuffer.ToPointer() + attribId * 4;
				attribPtr[0] = valueX;
			}
		}

		public void SetAttributeUnsafe(int attribId, int valueX, int valueY)
		{
			m_WasModified = true;
			unsafe
			{
				int* attribPtr = (int*)m_AttributesBuffer.ToPointer() + attribId * 4;
				attribPtr[0] = valueX;
				attribPtr[1] = valueY;
			}
		}

		public void SetAttributeUnsafe(int attribId, int valueX, int valueY, int valueZ)
		{
			m_WasModified = true;
			unsafe
			{
				int* attribPtr = (int*)m_AttributesBuffer.ToPointer() + attribId * 4;
				attribPtr[0] = valueX;
				attribPtr[1] = valueY;
				attribPtr[2] = valueZ;
			}
		}

		public void SetAttributeUnsafe(int attribId, int valueX, int valueY, int valueZ, int valueW)
		{
			m_WasModified = true;
			unsafe
			{
				int* attribPtr = (int*)m_AttributesBuffer.ToPointer() + attribId * 4;
				attribPtr[0] = valueX;
				attribPtr[1] = valueY;
				attribPtr[2] = valueZ;
				attribPtr[3] = valueW;
			}
		}

		public void SetAllAttributes(SGenericTypedValueContainer[] attributes)
		{
			m_WasModified = true;
			if (attributes.Length != m_FxAsset.m_AttributeDescs.Count * 4)
			{
				Debug.LogError("[PopcornFX] The attribute array does not contain the same number of attributes than the pkfx asset : " + attributes.Length + " != " + m_FxAsset.m_AttributeDescs.Count * 4);
				return;
			}
			byte[] array = new byte[attributes.Length * 4];

			for (int i = 0; i < attributes.Length; ++i)
			{
				attributes[i].WriteToByteArray(ref array, i * 4);
			}
			Marshal.Copy(array, 0, m_AttributesBuffer, attributes.Length * 4);
		}

		#endregion

		public void UpdateAttributes()
		{
			if (m_WasModified)
			{
				m_WasModified = false;
				unsafe
				{
					PKFxManager.SetAttributesDirty(m_FXGuid);
				}
			}
		}

		override public string ToString()
		{
			return "PkFxAttributeContainer " + m_FxAsset.AssetVirtualPath;
		}
	}
}
