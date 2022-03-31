//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using NUnit.Framework;
using UnityEngine;
using UnityEngine.TestTools;
using PopcornFX;
using AOT;

public class PopcornFXTestsEditor_ImportSamples
{
	public int AssetCount = 0;
	public static PopcornFXTestsEditor_ImportSamples Instance;

	[MonoPInvokeCallback(typeof(AssetChangeCallback))]
	public static void OnAssetChangeUnitTest(ref SAssetChangesDesc assetChange)
	{
		Instance.AssetCount--;
		PKFxManagerImpl.OnAssetChange(ref assetChange);
	}

	public IEnumerator ImportPack(string sourcePack)
	{
		Instance = this;

		string	PopcornPackFxPath = PKFxSettings.PopcornPackFxPath;
		string	UnityPackFxPath = PKFxSettings.UnityPackFxPath;
		bool	EnableForceDeterminism = PKFxSettings.EnableForceDeterminism;

		PKFxSettings.PopcornPackFxPath = sourcePack;
		PKFxSettings.UnityPackFxPath = "/PopcornFXAssets";
		PKFxSettings.EnableForceDeterminism = true;

		PKFxSettings.GetAllAssetPath();

		AssetCount = PKFxSettings.AssetPathList.Count;

		Assert.IsTrue(AssetCount != 0);

		PKFxManagerImpl.SetDelegateOnAssetChange(PKFxDelegateHandler.Instance.DelegateToFunctionPointer(new AssetChangeCallback(OnAssetChangeUnitTest)));

		PKFxSettings.ReimportAssets(PKFxSettings.AssetPathList);

		while (AssetCount != 0)
		{
			yield return null;
		}
		PKFxSettings.PopcornPackFxPath = PopcornPackFxPath;
		PKFxSettings.UnityPackFxPath = UnityPackFxPath;
		PKFxSettings.EnableForceDeterminism = EnableForceDeterminism;

		PKFxManagerImpl.SetDelegateOnAssetChange(PKFxDelegateHandler.Instance.DelegateToFunctionPointer(new AssetChangeCallback(PKFxManagerImpl.OnAssetChange)));

		yield return null;
	}

	[UnityTest]
	public IEnumerator PopcornFXTestsEditor_ImportSamples_SimplePasses()
	{
		return ImportPack("../../../../../release/application/Packs/Samples");
	}

	[UnityTest]
	public IEnumerator PopcornFXTestsEditor_ImportTutorials_SimplePasses()
	{
		return ImportPack("../../../../../release/application/Packs/Tutorials");
	}

	[UnityTest]
	public IEnumerator PopcornFXTestsEditor_ImportUT_SimplePasses()
	{
		return ImportPack("../../../../../release/application/Packs/UnitTests/PopcornFX");
	}
}
