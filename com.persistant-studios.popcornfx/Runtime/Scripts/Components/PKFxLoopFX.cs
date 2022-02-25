//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;

namespace PopcornFX
{
	public class PKFxLoopFX : MonoBehaviour
	{
		public PKFxEmitter m_Fx = null;
		public float m_LoopTime = 1.0f;
		private float m_CurTime = 0.0f;

		// Start is called before the first frame update
		void Start()
		{
			if (m_Fx == null)
				m_Fx = GetComponent<PKFxEmitter>();
			if (m_Fx == null)
				Debug.LogError("[PopcornFX] Could not retrieve the PKFXFx to loop...");
		}

		// Update is called once per frame
		void Update()
		{
			if (m_Fx == null)
				return;
			float frameDt = Time.smoothDeltaTime * PKFxManager.TimeMultiplier;
			m_CurTime += frameDt;
			while (m_CurTime - m_LoopTime > 0)
			{
				if (m_Fx.Alive)
					m_Fx.TerminateEffect();
				m_CurTime -= m_LoopTime;
				m_Fx.StartEffectWithDt(m_CurTime);
			}
		}
	}
}
