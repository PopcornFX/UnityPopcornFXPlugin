//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System;
using AOT;

namespace PopcornFX
{
	// Usage:
	// 'Unity.exe -batchmode -projectPath [pathToProject] -executeMethod PopcornFX.PKFxPkkgImporter.ImportPkkgFromCmdLineArgument --pkkg [pathToPkkg]'
	//
	public class PKFxPkkgImporter
	{
		public int AssetCount = 0;
		public static PKFxPkkgImporter	Instance;

		[MonoPInvokeCallback(typeof(AssetChangeCallback))]
		public static void OnAssetChangeBatchMode(ref SAssetChangesDesc assetChange)
		{
			Instance.AssetCount--;
			PKFxManagerImpl.OnAssetChange(ref assetChange);
		}

		public static void ImportPkkgFromCmdLineArgument()
		{
			// Fake coroutine setup
			PKFxPkkgImporter.Instance = new PKFxPkkgImporter();
			var it = ImportPkkgFromCmdLineArgument_Coroutine();
			while (it != null && it.MoveNext()) { }
			EditorApplication.Exit(0);
		}

		public static IEnumerator ImportPkkgFromCmdLineArgument_Coroutine()
		{
			PKFxSettings.PopcornPackFxPath = "";
			while (!PKFxManager.IsDllLoaded())
			{
				yield return null;
			}

			PKFxManager.StartupPopcorn(false);

			Debug.Log("[PopcornFX] Parsing command line argument to extract Pkkg path");
			string[]	cmdLineArg = System.Environment.GetCommandLineArgs();
			string		pkkgPath = "";
			bool		pkkgArg = false;

			for (int i = 0; i < cmdLineArg.Length; ++i)
			{
				if (pkkgArg)
				{
					pkkgPath = cmdLineArg[i];
					break;
				}	
				if (cmdLineArg[i] == "--pkkg")
				{
					pkkgArg = true;
				}
			}
			PKFxManager.ExtractedPkkg = "";
			bool ret = PKFxManager.ExtractPkkg(pkkgPath, true);
			if (ret)
			{
				Debug.Log(PKFxManager.ExtractedPkkg);
				Uri fileUri = new Uri(PKFxManager.ExtractedPkkg);
				Uri referenceUri = new Uri(Application.dataPath);
				Uri relative = referenceUri.MakeRelativeUri(fileUri);
				string Srcpath = referenceUri.MakeRelativeUri(fileUri).ToString();

				string pkkgPackPath = Uri.UnescapeDataString(Srcpath);

				string PopcornPackFxPath = PKFxSettings.PopcornPackFxPath;

				PKFxSettings.PopcornPackFxPath = Path.GetFullPath(pkkgPackPath);

				PKFxSettings.GetAllAssetPath();

				Instance.AssetCount = PKFxSettings.AssetPathList.Count;

				PKFxManagerImpl.SetDelegateOnAssetChange(PKFxDelegateHandler.Instance.DelegateToFunctionPointer(new AssetChangeCallback(OnAssetChangeBatchMode)));

				PKFxSettings.ReimportAssets(PKFxSettings.AssetPathList);

				// block below Copy pasted from PKFxFilePackManager.Update function as custom asset processor don't seems to be instanciated in batch mode
				PKFxManager.LockPackWatcherChanges();

				int totalChanges = 0;
				int remainingChanges = 0;
				bool unstackChangesSuccess = true;
				List<string> updatedAssets = null;

				try // Need try catch to avoid dead lock!
				{
					unstackChangesSuccess = PKFxManager.PullPackWatcherChanges(out totalChanges);

					if (totalChanges > 0)
						updatedAssets = new List<string>(totalChanges);
					remainingChanges = totalChanges;
					while (remainingChanges > 0 && unstackChangesSuccess)
					{
						unstackChangesSuccess = PKFxManager.PullPackWatcherChanges(out remainingChanges);

						if (!updatedAssets.Contains(PKFxManager.GetImportedAssetPath()))
							updatedAssets.Add(PKFxManager.GetImportedAssetPath());
					}

					if (unstackChangesSuccess == false)
						Debug.LogWarning("[PopcornFX] PackWatcher Unstack issue");

				}
				catch (Exception e)
				{
					Debug.LogError("[PopcornFX] Error in effect importer: " + e.Message);
				}

				PKFxManager.UnlockPackWatcherChanges();
				//end of copy from PKFxFilePackManager.Update

				// Not ideal:
				// will only exit on success, as assetCount will never reach 0 if bake fail for any reason.
				// Operation in prod environnement should be with timeout and/or handling error case on bake
				while (Instance.AssetCount != 0)
				{
					yield return null;
				}
				PKFxManagerImpl.SetDelegateOnAssetChange(PKFxDelegateHandler.Instance.DelegateToFunctionPointer(new AssetChangeCallback(PKFxManagerImpl.OnAssetChange)));

				PKFxManager.DeleteExtractedPkkg(PKFxSettings.PopcornPackFxPath);

				PKFxSettings.PopcornPackFxPath = PopcornPackFxPath;

				yield return null;
			}
		}
	}
}

