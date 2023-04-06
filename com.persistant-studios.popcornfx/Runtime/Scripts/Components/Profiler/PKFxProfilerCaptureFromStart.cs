//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using System.Collections;

namespace PopcornFX
{
	public class PKFxProfilerCaptureFromStart : MonoBehaviour
	{
		public int FrameCountToCapture = 200;

		private bool m_InCapture = false;
		private int m_FrameCaptured = 0;

		private void Start()
		{
			m_InCapture = true;
			PKFxManager.ProfilerEnable(true);

		}

		//----------------------------------------------------------------------------

		void Update()
		{
			if (m_InCapture)
			{
				m_FrameCaptured++;
				if (m_FrameCaptured >= FrameCountToCapture)
				{
					m_FrameCaptured = 0;
					PKFxManager.ProfilerWriteReport(Application.persistentDataPath + "/ProfileReport.pkpr");
					Debug.Log("[PopcornFX] Profiling report written to " + Application.persistentDataPath + "/ProfileReport.pkpr");
					m_InCapture = false;
					PKFxManager.ProfilerEnable(false);
				}
			}
		}
	}
}