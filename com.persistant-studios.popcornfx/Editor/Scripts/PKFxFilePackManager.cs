//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using UnityEditor;
using System.IO;
using System;

namespace PopcornFX
{
	[InitializeOnLoad]
	public class PKFxFilePackManager : AssetPostprocessor
	{
		static private bool m_DllLoaded = false;

		//----------------------------------------------------------------------------

		static PKFxFilePackManager()
		{
			EditorApplication.update += Update;
#if UNITY_2018_1_OR_NEWER
			EditorApplication.quitting += Quit;
		
			if (PlayerSettings.allowUnsafeCode == false)
			{
				Debug.LogWarning("[PopcornFX] PopcornFX plugin rely on unsafe code to be activated. We took the permission to allow it. To revert, change your player settings.");
				PlayerSettings.allowUnsafeCode = true;
			}
#endif
		}

		static bool Initialize()
		{
			if (PKFxManager.IsDllLoaded() && !Application.isPlaying)
			{
				PKFxManager.StartupPopcorn(false);
				PKFxManager.StartupPopcornFileWatcher(PKFxSettings.EnableEffectHotreload);
				m_DllLoaded = true;

				if (!Directory.Exists("Assets" + PKFxSettings.UnityPackFxPath))
				{
					string DirName = PKFxSettings.UnityPackFxPath;
					if (DirName.StartsWith("/"))
						DirName = DirName.Substring(1);
					if (DirName.EndsWith("/"))
						DirName = DirName.Substring(DirName.Length - 1, 1);
					AssetDatabase.CreateFolder("Assets", DirName);
				}

				return true;
			}
			return false;
		}

		//----------------------------------------------------------------------------

		static void Update()
		{
			if (EditorApplication.isPlaying == true)
				return;
			if (m_DllLoaded == false)
			{
				if (Initialize() == false)
					return;
			}

			PKFxManager.LockPackWatcherChanges();

			int totalChanges = 0;
			int remainingChanges = 0;
			bool unstackChangesSuccess = true;

			try // Need try catch to avoid dead lock!
			{
				unstackChangesSuccess = PKFxManager.PullPackWatcherChanges(out totalChanges);

				remainingChanges = totalChanges;
				while (remainingChanges > 0 && unstackChangesSuccess)
				{
					unstackChangesSuccess = PKFxManager.PullPackWatcherChanges(out remainingChanges);

					if (EditorUtility.DisplayCancelableProgressBar("Baking and importing PopcornFX effects",
																	"Importing \'" + PKFxManager.GetImportedAssetName() + "\' and its dependencies.",
																	(float)(totalChanges - remainingChanges) / (float)totalChanges))
					{
						remainingChanges = 0;
						PKFxManager.CancelPackWatcherChanges();
					}
				}

				EditorUtility.ClearProgressBar();

				if (unstackChangesSuccess == false)
					Debug.LogWarning("[PopcornFX] PackWatcher Unstack issue");

			}
			catch (Exception e)
			{
				Debug.LogError("Error in effect importer: " + e.Message);
			}

			PKFxManager.UnlockPackWatcherChanges();

			// We can display the PopcornFX logs here for baking errors:
			PKFxLogger.LogUpdate(PKFxManager.EPopcornLogLevels.Level_Info);
		}

		//----------------------------------------------------------------------------

		static void Quit()
		{
			EditorApplication.update -= Update;
#if UNITY_2018_1_OR_NEWER
			EditorApplication.quitting -= Quit;
#endif
		}

		//----------------------------------------------------------------------------

		static void OnPostprocessAllAssets(string[] importedAssets, string[] deletedAssets, string[] movedAssets, string[] movedFromAssetPaths)
		{
			AssetDatabase.StartAssetEditing();
			foreach (string str in importedAssets)
			{
				PKFxAssetCreationUtils.NotifyAssetPostProcess(str);
			}
			AssetDatabase.StopAssetEditing();
		}
	}
}