//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PopcornFX
{
	public class PKFxLogger : MonoBehaviour
	{
		public PKFxManager.EPopcornLogLevels m_MinimumLogLevel = PKFxManager.EPopcornLogLevels.Level_Debug;

		void Update()
		{
			LogUpdate(m_MinimumLogLevel);
		}

		public static void	LogUpdate(PKFxManager.EPopcornLogLevels minimumLogLevel)
		{
			string							currentLog = "";
			PKFxManager.EPopcornLogLevels	level = PKFxManager.EPopcornLogLevels.Level_Info;

			if (minimumLogLevel == PKFxManager.EPopcornLogLevels.Level_None)
				return;

			while (PKFxManager.GetPopcornFXLogNumber() > 0 && PKFxManager.GetPopcornFXLog(ref currentLog, ref level) >= 0)
			{
				if ((int)level < (int)minimumLogLevel)
					continue;
				if (currentLog.Length == 0)
					continue;

				switch (level)
				{
					case PKFxManager.EPopcornLogLevels.Level_Debug:
					case PKFxManager.EPopcornLogLevels.Level_Info:
						Debug.Log("[PopcornFX-Cpp]: " + currentLog);
						break;
					case PKFxManager.EPopcornLogLevels.Level_Error:
					case PKFxManager.EPopcornLogLevels.Level_ErrorCritical:
						Debug.LogError("[PopcornFX-Cpp]: " + currentLog);
						break;
					case PKFxManager.EPopcornLogLevels.Level_Warning:
						Debug.LogWarning("[PopcornFX-Cpp]: " + currentLog);
						break;
					default:
						break;
				}
			}
		}
	}
}