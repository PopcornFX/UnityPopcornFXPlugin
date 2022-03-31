//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace PopcornFX
{
	[Serializable]
	[StructLayout(LayoutKind.Sequential)]
	public struct SPayloadContainer_Vector4
	{
		public SGenericTypedValueContainer x;
		public SGenericTypedValueContainer y;
		public SGenericTypedValueContainer z;
		public SGenericTypedValueContainer w;
	
		public void GetBool4(out bool v1, out bool v2, out bool v3, out bool v4)
		{
			v1 = Convert.ToBoolean(x.b1);
			v2 = Convert.ToBoolean(x.b2);
			v3 = Convert.ToBoolean(x.b3);
			v4 = Convert.ToBoolean(x.b4);
		}

		public void GetFloat3(out Vector3 value)
		{
			value = new Vector3(x.f1, y.f1, z.f1);
		}

		public void GetFloat4(out Vector4 value)
		{
			value = new Vector4(x.f1, y.f1, z.f1, w.f1);
		}
	
		public void GetInt4(out Vector4 value)
		{
			value = new Vector4(x.i1, y.i1, z.i1, w.i1);
		}
	
		public void GetQuaternion(out Quaternion value)
		{
			value = new Quaternion(x.f1, y.f1, z.f1, w.f1);
		}
	}
	
	public struct SPopcornFXPayloadData
	{
		List<SPayloadContainer_Vector4> m_Values;
		public string m_PayloadName;
		public EAttributeType m_PayloadType;
	
		public List<SPayloadContainer_Vector4> Values
		{
			get { return m_Values; }
		}
	
		public int NumberOfValues
		{
			get { return m_Values.Count; }
		}
	
		public SPopcornFXPayloadData(int length = 0, string payloadName = "", EAttributeType type = EAttributeType.Bool)
		{
			m_Values = new List<SPayloadContainer_Vector4>(length);
			m_PayloadType = type;
			m_PayloadName = payloadName;
		}
	
		public void GetBool4(out bool[] value, int index)
		{
			Debug.Assert((m_PayloadType == EAttributeType.Bool) ||
						 (m_PayloadType == EAttributeType.Bool2) ||
						 (m_PayloadType == EAttributeType.Bool3) ||
						 (m_PayloadType == EAttributeType.Bool4));
	
			value = new bool[4];
	
			m_Values[index].GetBool4(out value[0], out value[1], out value[2], out value[3]);
		}
		public void GetFloat3(out Vector3 value, int index)
		{
			Debug.Assert((m_PayloadType == EAttributeType.Float) ||
						 (m_PayloadType == EAttributeType.Float2) ||
						 (m_PayloadType == EAttributeType.Float3));

			m_Values[index].GetFloat3(out value);
		}

		public void GetFloat4(out Vector4 value, int index)
		{
			Debug.Assert((m_PayloadType == EAttributeType.Float) ||
						 (m_PayloadType == EAttributeType.Float2) ||
						 (m_PayloadType == EAttributeType.Float3) ||
						 (m_PayloadType == EAttributeType.Float4));
	
			m_Values[index].GetFloat4(out value);
		}
	
		public void GetInt4(out Vector4 value, int index)
		{
			Debug.Assert((m_PayloadType == EAttributeType.Int) ||
						 (m_PayloadType == EAttributeType.Int2) ||
						 (m_PayloadType == EAttributeType.Int3) ||
						 (m_PayloadType == EAttributeType.Int4));
	
			m_Values[index].GetInt4(out value);
		}
	
		public void GetQuaternion(out Quaternion value, int index)
		{
			Debug.Assert((m_PayloadType == EAttributeType.Quaternion));
	
			m_Values[index].GetQuaternion(out value);
		}
	};
	
	public delegate void PKFxRaiseEventCallback(string eventName, List<SPopcornFXPayloadData> payloads);
	
	public class PKFxRaiseEventCallbackData
	{
		public string m_EventName;
		public PKFxRaiseEventCallback m_Cb;
		public uint m_Count;
	
		public PKFxRaiseEventCallbackData(string eventName, PKFxRaiseEventCallback cb)
		{
			m_EventName = eventName;
			m_Cb = cb;
			m_Count = 1;
		}
	}
}