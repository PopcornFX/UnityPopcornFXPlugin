using System.IO;
using UnityEditor;
// Added new using
using UnityEngine;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using PopcornFX;

 class PKFxAssetsBuildProcessor : IPreprocessBuildWithReport, IPostprocessBuildWithReport
{
	public int callbackOrder { get { return 0; } }

	public void OnPostprocessBuild(BuildReport report)
	{
		PKFxManager.SetTargetPlatformForAssets(PKFxSettings.CurrentPlatformName);
	}

	public void OnPreprocessBuild(BuildReport report)
	{
		PKFxManager.SetTargetPlatformForAssets(report.summary.platform.ToString());
	}
}