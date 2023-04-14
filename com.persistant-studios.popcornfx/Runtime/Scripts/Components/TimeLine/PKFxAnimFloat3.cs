//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;

namespace PopcornFX
{
	public class PKFxAnimFloat3 : MonoBehaviour
	{
		public PKFxEmitter m_Fx = null;
		public string m_AttributeName;
		public Vector3 m_Value;

		private int m_AttributeId = -1;

		void Start()
		{
			if (m_Fx == null)
			{
				m_Fx = GetComponent<PKFxEmitter>();
				if (m_Fx == null)
				{
					Debug.LogError("[PopcornFX] PKFxAnimFloat3 Doesn't have a target nor a PKFxFX component attached to it", this);
					enabled = false;
				}
			}
			if (m_Fx != null)
			{
				m_AttributeId = m_Fx.EffectAsset.AttributeIdFromName(m_AttributeName);
				if (m_AttributeId == -1)
				{
					Debug.LogError("[PopcornFX] The attribute " + m_AttributeName + " does not exist in the effect " + m_Fx.EffectName, this);
				}
			}
		}

		void LateUpdate()
		{
			if (m_AttributeId != -1)
			{
				m_Fx.SetAttributeSafe(m_AttributeId, m_Value.x, m_Value.y, m_Value.z);
			}
		}

		void Reset()
		{
			if (m_Fx == null)
			{
				m_Fx = GetComponent<PKFxEmitter>();
				if (m_Fx == null)
				{
					m_Fx = GetComponentInParent<PKFxEmitter>();
				}
			}
		}
	}
}