//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using System.IO;
using UnityEngine.SceneManagement;
using UnityEditor.AssetImporters;
using UnityEditor.Experimental.AssetImporters;
using UnityEditor;

namespace PopcornFX
{
	[ScriptedImporter(1, new[] { "pkat", "pkmm", "pkfm", "pksc", "pkan", "pkvf" })]
	public class PKFxImporter : ScriptedImporter
	{
		public override void OnImportAsset(AssetImportContext ctx)
		{
			PKFxAsset fxa = null;

			if (Path.GetExtension(ctx.assetPath) == ".pkat")
			{
				fxa = ScriptableObject.CreateInstance<PKFxAtlasAsset>();
			}
			else if (Path.GetExtension(ctx.assetPath) == ".pkan")
			{
				fxa = ScriptableObject.CreateInstance<PKFxAnimationAsset>();
			}
			else if (Path.GetExtension(ctx.assetPath) == ".pkfm")
			{
				fxa = ScriptableObject.CreateInstance<PKFxFontMetricsAsset>();
			}
			else if (Path.GetExtension(ctx.assetPath) == ".pkmm")
			{
				fxa = ScriptableObject.CreateInstance<PKFxMeshAsset>();
			}
			else if (Path.GetExtension(ctx.assetPath) == ".pkvf")
			{
				fxa = ScriptableObject.CreateInstance<PKFxVectorFieldAsset>();
			}

			if (fxa != null)
			{
				fxa.m_Data = File.ReadAllBytes(ctx.assetPath);
				fxa.name = fxa.AssetFullPath = ctx.assetPath;
#if UNITY_2017_3_OR_NEWER
				ctx.AddObjectToAsset(fxa.name, fxa);
				ctx.SetMainObject(fxa);
#else
				ctx.SetMainAsset(assetVirtualPath, fxa, null);
#endif
			}
		}
	}
}