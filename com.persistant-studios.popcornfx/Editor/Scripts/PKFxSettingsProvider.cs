using PopcornFX;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class PKFxSettingsProvider : MonoBehaviour
{
	static class MyCustomSettingsIMGUIRegister
	{
		[SettingsProvider]
		public static SettingsProvider CreateMyCustomSettingsProvider()
		{
			var provider = new SettingsProvider("Project/PKFxSettings", SettingsScope.Project)
			{
				label = "PopcornFX Plugin",
				guiHandler = (searchContext) =>
				{
					PKFxSettingsUIUtils.DrawInspectorUI(true);
				},

				keywords = SettingsProvider.GetSearchKeywordsFromGUIContentProperties<PKFxSettingsUIUtils>()
			};

			return provider;
		}
	}
}
