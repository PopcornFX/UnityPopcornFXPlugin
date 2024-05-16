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
		public static string	m_ThumbnailsBakeDirectory = "Editor/Thumbnails/";

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
			newAsset.name = Path.GetFileName(path);
			newAsset.AssetVirtualPath = path;
			string fullPath = newAsset.AssetFullPath + ".asset";
			newAsset.m_Data = fxAsset.m_Data; // Copy the old asset content in the new one

			UnityEngine.Object[] data = AssetDatabase.LoadAllAssetsAtPath(fullPath);
			foreach (UnityEngine.Object asset in data)
			{
				PKFxCustomMaterialInfo info = asset as PKFxCustomMaterialInfo;
				if (info != null)
				{
					newAsset.m_CustomMaterials.Add(info);
				}
			}

			if (ProcessAssetChangeData(newAsset, path) == false)
				return false;

			PKFxEffectAsset oldAsset = AssetDatabase.LoadAssetAtPath<PKFxEffectAsset>(fullPath);
			EditorUtility.CopySerialized(newAsset, oldAsset);


			return true;
		}

		//----------------------------------------------------------------------------

		public static bool UpdateAndRenamePKFXAsset(PKFxEffectAsset fxAsset, string oldPath, string newPath)
		{
			PKFxEffectAsset newAsset = ScriptableObject.CreateInstance<PKFxEffectAsset>();
			newAsset.name = Path.GetFileName(newPath);
			newAsset.AssetVirtualPath = newPath;
			string fullPath = newAsset.AssetFullPath + ".asset";
			newAsset.m_Data = fxAsset.m_Data; // Copy the old asset content in the new one
			if (ProcessAssetChangeData(newAsset, oldPath) == false)
				return false;
			PKFxEffectAsset oldAsset = AssetDatabase.LoadAssetAtPath<PKFxEffectAsset>(fullPath);

			EditorUtility.CopySerialized(newAsset, oldAsset);
			return true;
		}

		//----------------------------------------------------------------------------

		public static void NotifyAssetPostProcess(string path)
		{
			int key = path.ToLowerInvariant().GetHashCode();
			if (DependenciesLoading.ContainsKey(key))
			{
				UnityEngine.Object obj = null;
				string editorResourcesPath = "Assets/Editor Default Resources/";
				if (path.StartsWith(editorResourcesPath))
					obj = EditorGUIUtility.Load(path.Substring(editorResourcesPath.Length));
				else
					obj = AssetDatabase.LoadAssetAtPath(path, typeof(UnityEngine.Object));
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
					if (dependency.HasUsageFlag(EUseInfoFlag.IsThumbnail))
					{
						asset.m_EditorThumbnail = (Texture2D)obj;
						if (PKFxSettings.UseThumbnailsInBuilds)
							asset.m_Thumbnail = (Texture2D)obj;
						asset.m_Dependencies.Remove(dependency);
					}
					else if (dependency.HasUsageFlag(EUseInfoFlag.IsAnimatedThumbnail))
					{
						//The animated thumbnails is imported one time as Texture2D and later as Texture2DArray, skip the Texture2D
						Texture2DArray animatedThumbnail = obj as Texture2DArray;
						if (animatedThumbnail == null)
							return;
						asset.m_EditorAnimatedThumbnail = animatedThumbnail;
						if (PKFxSettings.UseAnimatedThumbnailsInBuilds)
							asset.m_AnimatedThumbnail = animatedThumbnail;
						asset.m_Dependencies.Remove(dependency);
					}
					else if (PKFxManager.IsSupportedTextureExtension(Path.GetExtension(path)))
					{
						for (int i = 0; i < asset.m_RendererDescs.Count; ++i)
						{
							SBatchDesc rdr = asset.m_RendererDescs[i];
							if (rdr.m_Type == ERendererType.Light || rdr.m_Type == ERendererType.Sound)
								continue;
							PKFxRenderFeatureBinding binding = PKFxSettings.MaterialFactory.ResolveBatchBinding(rdr);

							if (binding != null)
							{
								List<PKFxEffectAsset.MaterialUIDToIndex> indexes = asset.m_MaterialIndexes.FindAll(item => item.m_UID == rdr.m_UID);
								if (indexes != null)
								{
									foreach (PKFxEffectAsset.MaterialUIDToIndex index in indexes)
									{
										// Attempt to bind all materials properties, even though some texture can be in import process, do not log errors.
										binding.BindMaterialProperties(rdr, asset.m_Materials[index.m_Idx], asset, false);
									}
								}
							}
							else
								Debug.LogWarning("[PopcornFX] No material binding found for batch descriptor: " + rdr.m_GeneratedName);
						}
					}
				}
				DependenciesLoading.Remove(key);
			}
		}

		//----------------------------------------------------------------------------
		// Private Methods
		//----------------------------------------------------------------------------

		public static void CreateAssetFolderUpToPath(string path, bool editorOnly = false)
		{
			string rootPath = editorOnly ? "Assets/Editor Default Resources" : "Assets";
			string parentfolderPath = rootPath + PKFxSettings.UnityPackFxPath;
			char[] separators = { '/', '\\' };

			if (editorOnly && AssetDatabase.IsValidFolder(rootPath) == false)
				AssetDatabase.CreateFolder("Assets", "Editor Default Resources");

			if (AssetDatabase.IsValidFolder(parentfolderPath) == false)
			{
				string packPath = PKFxSettings.UnityPackFxPath;
				if (packPath.StartsWith("/"))
					packPath = packPath.Substring(1);
				AssetDatabase.CreateFolder(rootPath, packPath);
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
				PKFxSettings.Instance.AddGUIDForAsset(fxAsset.AssetFullPath + ".asset", AssetDatabase.GUIDFromAssetPath(fxAsset.AssetFullPath + ".asset").ToString());
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

			PKFxManager.SetImportedAsset(null);
			handle.Free();

			ImportAssetDependencies(fxAsset);
			if (PKFxSettings.EnableThumbnails)
				ImportThumbnailIfExists(fxAsset, EUseInfoFlag.IsThumbnail);
			if (PKFxSettings.EnableAnimatedThumbnails)
				ImportThumbnailIfExists(fxAsset, EUseInfoFlag.IsAnimatedThumbnail);

			return true;
		}

		//----------------------------------------------------------------------------

		private static void ImportThumbnailIfExists(PKFxEffectAsset fxAsset, EUseInfoFlag usage)
		{
			string ext = "";
			bool editorOnly = true;

			if (usage == EUseInfoFlag.IsThumbnail)
			{
				ext = ".png";
				editorOnly = !PKFxSettings.UseThumbnailsInBuilds;
			}
			else if (usage == EUseInfoFlag.IsAnimatedThumbnail)
			{
				ext = ".anim.png";
				editorOnly = !PKFxSettings.UseAnimatedThumbnailsInBuilds;
			}
			else
				return;

			string thumbnailPath = fxAsset.AssetVirtualPath + ext;
			string thumbnailFullPath = m_TempBakeDirectory + m_ThumbnailsBakeDirectory + thumbnailPath;

			if (File.Exists(thumbnailFullPath))
			{
				//Add the thumbnail dependency
				PKFxEffectAsset.DependencyDesc dependency = new PKFxEffectAsset.DependencyDesc();
				dependency.m_Path = m_ThumbnailsBakeDirectory + thumbnailPath;
				dependency.m_UsageFlags = (int)usage;
				fxAsset.m_Dependencies.Add(dependency);

				CreateAssetFolderUpToPath(dependency.m_Path, editorOnly);
				CreateDependencyAsset(fxAsset, dependency, thumbnailFullPath, editorOnly);
			}
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
					CreateDependencyAsset(fxAsset, dependency, sourceFile);
				}
				else if (dependency.HasUsageFlag(EUseInfoFlag.IsMeshRenderer))
				{
					string sourceFile = m_TempBakeDirectory + dependency.m_Path;
					CreateDependencyAsset(fxAsset, dependency, sourceFile);
				}
				else if (dependency.HasUsageFlag(EUseInfoFlag.IsMeshSampler))
				{
					string sourceFile = m_TempBakeDirectory + dependency.m_Path;
					CreateDependencyAsset(fxAsset, dependency, sourceFile);

					string outputName = Path.ChangeExtension(dependency.m_Path, ".pkmm");
					dependency.m_Path = outputName;
					sourceFile = m_TempBakeDirectory + dependency.m_Path;
					CreateDependencyAsset(fxAsset, dependency, sourceFile);
				}
				else
				{
					string sourceFile = m_TempBakeDirectory + dependency.m_Path;
					CreateDependencyAsset(fxAsset, dependency, sourceFile);
				}
			}
		}

		//----------------------------------------------------------------------------

		private static void CreateDependencyAsset(PKFxEffectAsset fxAsset, PKFxEffectAsset.DependencyDesc dependencyDesc, string srcFile, bool editorOnly = false)
		{
			if (!File.Exists(srcFile))
			{
				return;
			}

			string dstVirtualPath = dependencyDesc.m_Path;
			string rootPath = editorOnly ? "Assets/Editor Default Resources" : "Assets";
			string dstPackPath = rootPath + PKFxSettings.UnityPackFxPath;
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

			int key = dstFullPath.ToLowerInvariant().GetHashCode();
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

		//----------------------------------------------------------------------------

		private static void ApplyPKImportSetting(PKFxEffectAsset.DependencyDesc dependency, string path)
		{
			string fExt = Path.GetExtension(path);

			if (PKFxManager.IsSupportedMeshExtension(fExt))
			{
				AssetImporter assetImporter = AssetImporter.GetAtPath(path);
				ModelImporter modelImporter = assetImporter as ModelImporter;

				modelImporter.isReadable = true;
				modelImporter.sortHierarchyByName = false;
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
					if (dependency.HasUsageFlag(EUseInfoFlag.IsLookupTexture))
					{
						textureImporter.sRGBTexture = false;
						textureImporter.wrapMode = TextureWrapMode.Clamp;
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
						textureImporter.wrapMode = TextureWrapMode.Clamp;
						textureImporter.sRGBTexture = !depIsLinearTexture;
						reimport = true;
					}
					if (dependency.HasUsageFlag(EUseInfoFlag.IsTextureSampler))
					{
						textureImporter.isReadable = true;
						reimport = true;
					}
					if (dependency.HasUsageFlag(EUseInfoFlag.IsThumbnail))
					{
						//textureImporter.textureType = TextureImporterType.Sprite;
						textureImporter.textureCompression = TextureImporterCompression.Uncompressed;
						reimport = true;
					}
					else if (dependency.HasUsageFlag(EUseInfoFlag.IsAnimatedThumbnail))
					{
						textureImporter.textureShape = TextureImporterShape.Texture2DArray;
						TextureImporterSettings settings = new TextureImporterSettings();
						textureImporter.ReadTextureSettings(settings);
						settings.flipbookRows = 48;
						textureImporter.SetTextureSettings(settings);
						reimport = true;
					}
					if (reimport)
						textureImporter.SaveAndReimport();
				}
				else if (ddsImporter != null)
				{
					string metaFilePath = AssetDatabase.GetTextMetaFilePathFromAssetPath(path);
					if (metaFilePath != null)
					{
						string metaData = File.ReadAllText(metaFilePath);
						if (depIsLinearTexture)
						{
							metaData = metaData.Replace("sRGBTexture: 1", "sRGBTexture: 0");
							metaData = metaData.Replace("wrapU: 0", "wrapU: 1");
							metaData = metaData.Replace("wrapV: 0", "wrapV: 1");
							metaData = metaData.Replace("wrapW: 0", "wrapW: 1");
						}
						if (dependency.HasUsageFlag(EUseInfoFlag.IsTextureSampler))
						{
							metaData = metaData.Replace("isReadable: 0", "isReadable: 1");
						}
						File.WriteAllText(metaFilePath, metaData);
					}
					else
					{
						if (depIsLinearTexture)
							Debug.LogWarning("[PopcornFX] Please verify import settings for texture '" + path + "' as DDS Importer does not allow to set gamma space.", AssetDatabase.LoadAssetAtPath<Texture>(path));
						if (dependency.HasUsageFlag(EUseInfoFlag.IsTextureSampler))
							ddsImporter.isReadable = true;
						ddsImporter.SaveAndReimport();
					}
				}
			}
		}

		//----------------------------------------------------------------------------

		public static void MoveThumbnails(bool fromEditor, bool animated)
		{
			string srcPath = (fromEditor ? "Assets/Editor Default Resources" : "Assets") + PKFxSettings.UnityPackFxPath + "/";
			string dstPath = (fromEditor ? "Assets" : "Assets/Editor Default Resources") + PKFxSettings.UnityPackFxPath + "/";
			string virtualPath =  m_ThumbnailsBakeDirectory;

			if (!Directory.Exists(srcPath + virtualPath))
				return;

			string ext = animated ? ".anim.png" : ".png";
			string searchPattern = "*.pkfx" + ext;
			string effectRootPath = "Assets" + PKFxSettings.UnityPackFxPath + "/";

			foreach (string file in Directory.EnumerateFiles(srcPath + virtualPath, searchPattern, SearchOption.AllDirectories))
			{
				string assetPath = file.Substring(srcPath.Length).Replace("\\", "/");

				CreateAssetFolderUpToPath(assetPath, !fromEditor);
				AssetDatabase.MoveAsset(srcPath + assetPath, dstPath + assetPath);

				string effectPath = assetPath.Substring(m_ThumbnailsBakeDirectory.Length);
				effectPath = effectPath.Substring(0, effectPath.Length - ext.Length);
				effectPath = effectPath + ".asset";
				PKFxEffectAsset effectAsset = AssetDatabase.LoadAssetAtPath<PKFxEffectAsset>(effectRootPath + effectPath);

				if (effectAsset != null)
				{
					if (animated)
					{
						if (fromEditor)
							effectAsset.m_AnimatedThumbnail = effectAsset.m_EditorAnimatedThumbnail;
						else
							effectAsset.m_AnimatedThumbnail = null;
					}
					else
					{
						if (fromEditor)
							effectAsset.m_Thumbnail = effectAsset.m_EditorThumbnail;
						else
							effectAsset.m_Thumbnail = null;
					}
					EditorUtility.SetDirty(effectAsset);
					AssetDatabase.SaveAssetIfDirty(effectAsset);
				}
			}

			DeleteEmptyFolders(Path.GetDirectoryName(srcPath + virtualPath));
		}

		//----------------------------------------------------------------------------
		public static void DeleteEmptyFolders(string path)
		{
			while (AssetDatabase.IsValidFolder(path))
			{
				string[] remainingAssets = AssetDatabase.FindAssets("*", new string[] { path });
				bool onlyDirectories = true;
				foreach (var guid in remainingAssets)
				{
					string assetPath = AssetDatabase.GUIDToAssetPath(guid);
					if (!AssetDatabase.IsValidFolder(assetPath))
					{
						onlyDirectories = false;
						break;
					}
				}
				if (onlyDirectories)
				{
					AssetDatabase.DeleteAsset(path);
					path = Path.GetDirectoryName(path);
				}
				else
					break;
			}
		}

		//----------------------------------------------------------------------------
	}
}

#endif