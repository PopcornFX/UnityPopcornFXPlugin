//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Unity Editor Only class.
// Exposed in runtime due to the limitation of importing symbols from editor
//	folder into runtime classes.
//----------------------------------------------------------------------------
#if UNITY_EDITOR

using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace PopcornFX
{
	public static class PKFxAssetCreationUtils : object
	{
		public static Dictionary<int, List<PKFxEffectAsset>> m_DependenciesLoading = new Dictionary<int, List<PKFxEffectAsset>>();
		public static Dictionary<int, List<PKFxEffectAsset>> DependenciesLoading { get { return m_DependenciesLoading; } }

		public static string	m_TempBakeDirectory = "Temp/PopcornFx/Baked/";

		//----------------------------------------------------------------------------
		// Public Methods
		//----------------------------------------------------------------------------

		public static bool CreatePKFXAsset(SAssetChangesDesc assetChange)
		{
			return ProcessAssetCreationData(assetChange);
		}

		//----------------------------------------------------------------------------

		public static bool UpdatePKFXAsset(PKFxEffectAsset fxAsset, string path)
		{
			PKFxEffectAsset newAsset = ScriptableObject.CreateInstance<PKFxEffectAsset>();
			newAsset.AssetVirtualPath = path;
			string fullPath = newAsset.AssetFullPath + ".asset";
			newAsset.m_Data = fxAsset.m_Data; // Copy the old asset content in the new one
			AssetDatabase.DeleteAsset(fullPath); // Delete previous asset
			if (ProcessAssetChangeData(newAsset, path) == false)
				return false;
			AssetDatabase.CreateAsset(newAsset, fullPath); // Re-create the asset with the new object
			return true;
		}

		//----------------------------------------------------------------------------

		public static bool UpdateAndRenamePKFXAsset(PKFxEffectAsset fxAsset, string oldPath, string newPath)
		{
			PKFxEffectAsset newAsset = ScriptableObject.CreateInstance<PKFxEffectAsset>();
			newAsset.AssetVirtualPath = newPath;
			string fullPath = newAsset.AssetFullPath + ".asset";
			newAsset.m_Data = fxAsset.m_Data; // Copy the old asset content in the new one
			AssetDatabase.DeleteAsset(fullPath); // Delete previous asset
			if (ProcessAssetChangeData(fxAsset, oldPath) == false)
				return false;
			AssetDatabase.CreateAsset(newAsset, fullPath); // Re-create the asset with the new object
			return true;
		}

		//----------------------------------------------------------------------------

		public static void NotifyAssetPostProcess(string path)
		{
			int key = path.GetHashCode();
			if (DependenciesLoading.ContainsKey(key))
			{
				UnityEngine.Object obj = AssetDatabase.LoadAssetAtPath(path, typeof(UnityEngine.Object));
				if (obj == null)
				{
					Debug.LogError("[PKFX] Unable to load dependency at " + path);
					return;
				}
				List<PKFxEffectAsset> assets = DependenciesLoading[key];
				foreach (PKFxEffectAsset asset in assets)
				{
					var dependency = asset.m_Dependencies.Find(x => path.Contains(x.m_Path));
					if (dependency != null)
					{
						dependency.m_Object = obj;
						ApplyPKImportSetting(dependency, path);
					}
					if (PKFxManager.IsSupportedTextureExtension(Path.GetExtension(path)))
					{
						for (int i = 0; i < asset.m_RendererDescs.Count; ++i)
						{
							SBatchDesc rdr = asset.m_RendererDescs[i];

							PKFxRenderFeatureBinding binding = PKFxSettings.MaterialFactory.ResolveBatchBinding(rdr);

							if (asset.m_Materials.Count > rdr.MaterialIdx)
								binding.BindMaterialProperties(rdr, asset.m_Materials[rdr.MaterialIdx], asset);
						}
					}
				}
				DependenciesLoading.Remove(key);
			}
		}

		//----------------------------------------------------------------------------
		// Private Methods
		//----------------------------------------------------------------------------

		public static void CreateAssetFolderUpToPath(string path)
		{
			string parentfolderPath = "Assets" + PKFxSettings.UnityPackFxPath;
			char[] separators = { '/', '\\' };

			if (AssetDatabase.IsValidFolder(parentfolderPath) == false)
			{
				string root = PKFxSettings.UnityPackFxPath;
				if (root.StartsWith("/"))
					root = root.Substring(1);
				AssetDatabase.CreateFolder("Assets", root);
			}

			path = Path.GetDirectoryName(path);

			string[] folders = path.Split(separators, StringSplitOptions.RemoveEmptyEntries);
			foreach (string folder in folders)
			{
				if (AssetDatabase.IsValidFolder(Path.Combine(parentfolderPath, folder)) == false)
					AssetDatabase.CreateFolder(parentfolderPath, folder);
				parentfolderPath = parentfolderPath + "/" + folder;
			}
		}
		//----------------------------------------------------------------------------

		private static bool ProcessAssetCreationData(SAssetChangesDesc assetChange)
		{
			PKFxEffectAsset fxAsset = ScriptableObject.CreateInstance<PKFxEffectAsset>();

			if (fxAsset != null)
			{
				fxAsset.m_Data = File.ReadAllBytes(m_TempBakeDirectory + assetChange.m_Path);
				fxAsset.AssetVirtualPath = assetChange.m_Path;
				if (ProcessAssetChangeData(fxAsset, assetChange.m_Path) == false)
					return false;
				CreateAssetFolderUpToPath(assetChange.m_Path);
				AssetDatabase.CreateAsset(fxAsset, fxAsset.AssetFullPath + ".asset");
#if !(UNITY_2017 || UNITY_2018 || UNITY_2019)
				PKFxSettings.Instance.AddGUIDForAsset(fxAsset.AssetFullPath + ".asset", AssetDatabase.GUIDFromAssetPath(fxAsset.AssetFullPath + ".asset"));
#endif
			}
			return true;
		}

		//----------------------------------------------------------------------------

		private static bool ProcessAssetChangeData(PKFxEffectAsset fxAsset, string fxPathToPatch)
		{
			GCHandle	handle = GCHandle.Alloc(fxAsset.m_Data, GCHandleType.Pinned);
			IntPtr		fileContentPtr = handle.AddrOfPinnedObject();

			PKFxManager.SetImportedAsset(fxAsset);

			if (PKFxManager.BrowseEffectContent(fileContentPtr, fxAsset.m_Data.Length, m_TempBakeDirectory + fxAsset.AssetVirtualPath) == false)
			{
				handle.Free();
				Debug.LogError("[PopcornFX] Reimport of " + fxAsset.AssetVirtualPath + " failed");
				return false;
			}
			fxAsset.ComputePropertiesHash();

			// Fix all references in the current scene:
			bool sceneHasChanged = false;
			UnityEngine.Object[] effects = UnityEngine.Object.FindObjectsOfType(typeof(PKFxEmitter));

			foreach (UnityEngine.Object obj in effects)
			{
				PKFxEmitter effect = obj as PKFxEmitter;

				if (effect.EffectName == fxPathToPatch) // Sometimes the effect.m_FxAsset is null here, so we test against the m_FxName
				{
					if (effect.UpdateEffectAsset(fxAsset, false, false))
						sceneHasChanged = true;
				}
			}

			if (sceneHasChanged)
			{
				Scene currentScene = SceneManager.GetActiveScene();
				EditorSceneManager.MarkSceneDirty(currentScene);
			}

			PKFxManager.SetImportedAsset(null);
			handle.Free();

			ImportAssetDependencies(fxAsset);
			return true;
		}

		//----------------------------------------------------------------------------

		private static void ImportAssetDependencies(PKFxEffectAsset fxAsset)
		{
			foreach (PKFxEffectAsset.DependencyDesc dependency in fxAsset.m_Dependencies)
			{
				dependency.m_Path = dependency.m_Path.Replace("\\", "/");
				dependency.m_Path = dependency.m_Path.Replace("//", "/");
				CreateAssetFolderUpToPath(dependency.m_Path);

				//Cases where we need originals assets without bake.
				if (dependency.HasUsageFlag(EUseInfoFlag.IsLinearTextureRenderer))
				{
					string originalFileName = Path.GetFileNameWithoutExtension(dependency.m_Path).Replace("_linear", "");
					string originalPath = Path.GetDirectoryName(dependency.m_Path) + "/" + originalFileName + Path.GetExtension(dependency.m_Path);
					string sourceFile = m_TempBakeDirectory + originalPath;
					CreateDependencyAsset(fxAsset, dependency.m_Path, sourceFile);
				}
				else if (dependency.HasUsageFlag(EUseInfoFlag.IsMeshRenderer))
				{
					string sourceFile = PKFxSettings.PopcornPackFxPath + "/" + dependency.m_Path;
					CreateDependencyAsset(fxAsset, dependency.m_Path, sourceFile);
				}
				else
				{
					string sourceFile = m_TempBakeDirectory + dependency.m_Path;
					CreateDependencyAsset(fxAsset, dependency.m_Path, sourceFile);
				}
			}
		}

		//----------------------------------------------------------------------------

		private static void CreateDependencyAsset(PKFxEffectAsset fxAsset, string dstVirtualPath, string srcFile)
		{
			if (!File.Exists(srcFile))
				return;

			string dstPackPath = "Assets" + PKFxSettings.UnityPackFxPath;
			string dstFullPath = dstPackPath + "/" + dstVirtualPath;

			try
			{
				FileUtil.ReplaceFile(srcFile, dstFullPath);
			}
			catch (Exception e)
			{
				// From time to time, the ReplaceFile will fail when the destination file already exist
				// We can probably safely ignore that here...
				Debug.LogWarning("[PopcornFX] FileUtil.ReplaceFile failed: " + e.Message);
			}

			int key = dstFullPath.GetHashCode();
			if (!DependenciesLoading.ContainsKey(key))
			{
				DependenciesLoading.Add(key, new List<PKFxEffectAsset>());
				DependenciesLoading[key].Add(fxAsset);
				AssetDatabase.ImportAsset(dstFullPath, ImportAssetOptions.ForceUpdate | ImportAssetOptions.ForceSynchronousImport);
			}
			else if (!DependenciesLoading[key].Contains(fxAsset))
				DependenciesLoading[key].Add(fxAsset);
		}

		//----------------------------------------------------------------------------

		private delegate void GetWidthAndHeight(TextureImporter importer, ref int width, ref int height);
		private static GetWidthAndHeight getWidthAndHeightDelegate;

		public static void GetOriginalTextureSize(TextureImporter importer, ref int width, ref int height)
		{
			if (getWidthAndHeightDelegate == null)
			{
				var method = typeof(TextureImporter).GetMethod("GetWidthAndHeight", BindingFlags.NonPublic | BindingFlags.Instance);
				if (method != null)
				{
					getWidthAndHeightDelegate = Delegate.CreateDelegate(typeof(GetWidthAndHeight), null, method) as GetWidthAndHeight;
				}
				else
				{
					Debug.LogError("[PopcornFX] Could not load the function TextureImporter.GetWidthAndHeight (internal unity api function)");
					return;
				}
			}

			getWidthAndHeightDelegate.Invoke(importer, ref width, ref height);
		}

		private static void ApplyPKImportSetting(PKFxEffectAsset.DependencyDesc dependency, string path)
		{
			string fExt = Path.GetExtension(path);

			if (PKFxManager.IsSupportedMeshExtension(fExt))
			{
				AssetImporter assetImporter = AssetImporter.GetAtPath(path);
				ModelImporter modelImporter = assetImporter as ModelImporter;

				modelImporter.isReadable = true;
				modelImporter.bakeAxisConversion = true;
				modelImporter.SaveAndReimport();
			}
			if (PKFxManager.IsSupportedTextureExtension(fExt))
			{
				AssetImporter assetImporter = AssetImporter.GetAtPath(path);
				TextureImporter textureImporter = assetImporter as TextureImporter;
				IHVImageFormatImporter ddsImporter = assetImporter as IHVImageFormatImporter;
				bool depIsLinearTexture = dependency.HasUsageFlag(EUseInfoFlag.IsLinearTextureRenderer);

				if (textureImporter == null && ddsImporter == null)
				{
					if (depIsLinearTexture)
						Debug.LogWarning("[PopcornFX] Could not set the texture \"" + path + "\" to linear", AssetDatabase.LoadAssetAtPath<Texture>(path));
					return;
				}
				if (textureImporter != null)
				{
					bool reimport = false;
					if (dependency.HasUsageFlag(EUseInfoFlag.IsVatTexture))
					{
						textureImporter.sRGBTexture = false;
						textureImporter.npotScale = TextureImporterNPOTScale.None;
						textureImporter.mipmapEnabled = false;
						textureImporter.textureCompression = TextureImporterCompression.Uncompressed;
						int width = 0, height = 0;
						GetOriginalTextureSize(textureImporter, ref width, ref height);
						textureImporter.maxTextureSize = width > height ? width : height;
						reimport = true;
					}
					else if (textureImporter.sRGBTexture == depIsLinearTexture)
					{
						textureImporter.sRGBTexture = !depIsLinearTexture;
						reimport = true;
					}
					if (dependency.HasUsageFlag(EUseInfoFlag.IsTextureSampler))
					{
						textureImporter.isReadable = true;
						reimport = true;
					}
					if (reimport)
						textureImporter.SaveAndReimport();
				}
				else if (ddsImporter != null)
				{
					if (depIsLinearTexture)
						Debug.LogWarning("[PopcornFX] Please verify import settings for texture '" + path + "' as DDS Importer does not allow to set gamma space.", AssetDatabase.LoadAssetAtPath<Texture>(path));
					if (dependency.HasUsageFlag(EUseInfoFlag.IsTextureSampler))
					{
						ddsImporter.isReadable = true;
						ddsImporter.SaveAndReimport();
					}
				}
			}
		}

		//----------------------------------------------------------------------------
	}
}

#endif