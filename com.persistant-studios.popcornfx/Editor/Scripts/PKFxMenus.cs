//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEditor;
using UnityEngine;
using System.IO;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;
using System.Collections.Generic;
using System.Reflection;
using UnityEditor.VersionControl;
using System.Linq;

namespace PopcornFX
{
	public class PKFxMenus : MonoBehaviour
	{
		private static void WarnRestart()
		{
			Debug.LogWarning("[PopcornFX] Settings : For this setting to take effect, you'll need to re-open your project.");
		}

		//----------------------------------------------------------------------------

		private static bool UpdatePKFXReference(PKFxEmitter fx)
		{
			bool loadingFailed = false;
			string oldPath = fx.EffectName;
			string newPath = PKFxEmitter.UpdateAssetPathIFN(oldPath);

			if ((fx.EffectAsset == null && !string.IsNullOrEmpty(fx.EffectName)) ||
				newPath != oldPath)
			{
				fx.EffectName = newPath;

				string parentfolderPath = "Assets" + PKFxSettings.UnityPackFxPath;
				string assetPath = parentfolderPath + "/" + fx.EffectName;

				PKFxEffectAsset fxNewAsset = (PKFxEffectAsset)AssetDatabase.LoadAssetAtPath(assetPath, typeof(PKFxEffectAsset));

				if (fxNewAsset == null)
				{
					Debug.LogWarning("[PopcornFX] The pkfx \"" + oldPath + "\" could not be upgraded.", fx);
					Debug.LogWarning("[PopcornFX] Could not load the new effect \"" + newPath + "\".");
					loadingFailed = true;
				}
				if (fx.EffectAsset == null && fxNewAsset != null)
				{
					Debug.Log("[PopcornFX] Effect \"" + fx.EffectName + "\" was properly reloaded.");
				}
				fx.m_FxAsset = fxNewAsset;
			}
			fx.UpdatePkFXComponent(fx.EffectAsset, loadingFailed == false);
			if (loadingFailed)
			{
				fx.ClearAllAttributesAndSamplers();
			}
			return true;
		}

		//----------------------------------------------------------------------------

		private static List<string> GetMonobehaviorComponentReferences(Object obj)
		{
			List<string> paths = new List<string>();
			const BindingFlags flags = /*BindingFlags.NonPublic | */BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static;
			FieldInfo[] fields = obj.GetType().GetFields(flags);

			foreach (FieldInfo fieldInfo in fields)
			{
				if (fieldInfo == null)
					continue;
				if (fieldInfo.FieldType == typeof(PKFxEffectAsset))
				{
					PKFxEffectAsset fxAsset = fieldInfo.GetValue(obj) as PKFxEffectAsset;
					if (fxAsset != null)
					{
						paths.Add(fxAsset.AssetVirtualPath);
					}
				}
			}
			return paths;
		}

		//----------------------------------------------------------------------------

		private static bool UpdateMonobehaviorComponentReferences(Object obj)
		{
			const BindingFlags flags = /*BindingFlags.NonPublic | */BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static;
			FieldInfo[] fields = obj.GetType().GetFields(flags);
			bool fieldsPatched = false;

			foreach (FieldInfo fieldInfo in fields)
			{
				if (fieldInfo == null)
					continue;
				if (fieldInfo.FieldType == typeof(PKFxEffectAsset))
				{
					PKFxEffectAsset fxAsset = fieldInfo.GetValue(obj) as PKFxEffectAsset;
					if (fxAsset != null)
					{
						string newPath = PKFxEmitter.UpdateAssetPathIFN(fxAsset.AssetVirtualPath);
						PKFxEffectAsset fxNewAsset = (PKFxEffectAsset)AssetDatabase.LoadAssetAtPath(newPath, typeof(PKFxEffectAsset));
						if (fxNewAsset != null)
						{
							fieldInfo.SetValue(obj, fxNewAsset);
							fieldsPatched = true;
						}
						else
						{
							Debug.LogWarning("[PopcornFX] [" + obj.GetType().Name + "] The pkfx \"" + fxAsset.AssetVirtualPath + "\" could not be upgraded.", obj);
							Debug.LogWarning("[PopcornFX] [" + obj.GetType().Name + "] Could not load the new effect \"" + newPath + "\".", fxAsset);
						}
					}
				}
			}
			return fieldsPatched;
		}

		//----------------------------------------------------------------------------

		private static bool UpdateAssetsReferences(Object obj)
		{
			PKFxEmitter emitterComponent = (obj as PKFxEmitter);

			if (emitterComponent != null)
			{
				return UpdatePKFXReference(emitterComponent);
			}
			else
			{
				return UpdateMonobehaviorComponentReferences(obj);
			}
		}

		//----------------------------------------------------------------------------

		public static List<string> GetFxsOnAllScenesAndPrefabs(string rootPath)
		{
			List<string> EffectsNames = new List<string>();
			Scene startingScene = EditorSceneManager.GetActiveScene();
			string startingScenePath = null;

			if (!string.IsNullOrEmpty(startingScene.path))
				startingScenePath = startingScene.path;

			string[] folders = { rootPath };

			string[] foundScenes = AssetDatabase.FindAssets("t:SceneAsset", folders);
			string[] foundPrefabs = AssetDatabase.FindAssets("t:Prefab", folders);
			string[] foundScripts = AssetDatabase.FindAssets("t:Script", folders);

			foreach (var guid in foundScenes)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);

				if (path.StartsWith("Assets") || path.Contains("popcornfx"))
					EditorSceneManager.OpenScene(path);
				else
					continue;
				Object[] objs = Resources.FindObjectsOfTypeAll(typeof(MonoBehaviour));

				foreach (var obj in objs)
				{
					if (obj == null)
						continue;
					PKFxEmitter emitterComponent = (obj as PKFxEmitter);

					if (emitterComponent != null)
					{
						string effectName = emitterComponent.EffectName;
						string extension = Path.GetExtension(effectName);
						if (extension == ".asset")
							effectName = effectName.Substring(0, effectName.Length - extension.Length);
						if (!EffectsNames.Contains(effectName))
							EffectsNames.Add(effectName);
					}
					else
					{
						List<string> monoRefs = GetMonobehaviorComponentReferences(obj);
						if (monoRefs.Count > 0)
							EffectsNames = EffectsNames.Union(monoRefs).ToList();
					}
				}
			}

			//restore the starting scene
			if (startingScenePath != null)
				EditorSceneManager.OpenScene(startingScenePath);

			foreach (string guid in foundPrefabs)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);

				Object[] objs = AssetDatabase.LoadAllAssetsAtPath(path);
				foreach (var obj in objs)
				{
					if (obj == null)
						continue;
					PKFxEmitter emitterComponent = (obj as PKFxEmitter);

					if (emitterComponent != null)
					{
						string effectName = emitterComponent.EffectName;
						string extension = Path.GetExtension(effectName);
						if (extension == ".asset")
							effectName = effectName.Substring(0, effectName.Length - extension.Length);
						if (!EffectsNames.Contains(effectName))
							EffectsNames.Add(effectName);
					}
					else
					{
						List<string> monoRefs = GetMonobehaviorComponentReferences(obj);
						if (monoRefs.Count > 0)
							EffectsNames = EffectsNames.Union(monoRefs).ToList();
					}
				}
			}

			foreach (string guid in foundScripts)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);
				Object[] objs = AssetDatabase.LoadAllAssetsAtPath(path);
				foreach (var obj in objs)
				{
					if (obj == null)
						continue;
					PKFxEmitter emitterComponent = (obj as PKFxEmitter);

					if (emitterComponent != null)
					{
						string effectName = emitterComponent.EffectName;
						string extension = Path.GetExtension(effectName);
						if (extension == ".asset")
							effectName = effectName.Substring(0, effectName.Length - extension.Length);
						if (!EffectsNames.Contains(effectName))
							EffectsNames.Add(effectName);
					}
					else
					{
						List<string> monoRefs = GetMonobehaviorComponentReferences(obj);
						if (monoRefs.Count > 0)
							EffectsNames = EffectsNames.Union(monoRefs).ToList();
					}
				}
			}

			return EffectsNames;
		}

		//----------------------------------------------------------------------------
		public static void UpdateFxsOnAllScenesAndPrefabs(string rootPath)
		{
			Scene startingScene = EditorSceneManager.GetActiveScene();
			string startingScenePath = null;

			if (!EditorSceneManager.SaveCurrentModifiedScenesIfUserWantsTo())
				return; //abort

			if (!string.IsNullOrEmpty(startingScene.path))
				startingScenePath = startingScene.path;


			string[] folders = { rootPath };

			string[] foundScenes = AssetDatabase.FindAssets("t:SceneAsset", folders);
			string[] foundPrefabs = AssetDatabase.FindAssets("t:Prefab", folders);
			string[] foundScripts = AssetDatabase.FindAssets("t:Script", folders);

			int referenceUpdateCount = 0;
			int totalElemCount = foundScenes.Length + foundPrefabs.Length + foundScripts.Length;
			int currElemCount = 0;

			foreach (var guid in foundScenes)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);

				EditorUtility.DisplayProgressBar("Update PKFxFX References", path, (float)currElemCount / (float)totalElemCount);

				if (path.StartsWith("Assets") || path.Contains("popcornfx"))
					EditorSceneManager.OpenScene(path);
				else
					continue;
				Object[] objs = Resources.FindObjectsOfTypeAll(typeof(MonoBehaviour));
				bool sceneUpdated = false;

				foreach (var obj in objs)
				{
					if (obj == null)
						continue;
					if (UpdateAssetsReferences(obj))
					{
						referenceUpdateCount++;
						sceneUpdated = true;
					}
				}
				if (sceneUpdated)
				{
					if (Provider.isActive)
					{
						Asset assetRef = new Asset(path);
						Task task = Provider.Checkout(assetRef, CheckoutMode.Both);
						task.Wait();
					}
					EditorSceneManager.SaveScene(SceneManager.GetActiveScene());
				}
				currElemCount++;
			}
			Resources.UnloadUnusedAssets();

			//restore the starting scene
			if (startingScenePath != null)
				EditorSceneManager.OpenScene(startingScenePath);

			bool assetUpdated = false;
			List<string> filesToReimport = new List<string>();
			AssetList updatedAssets = new AssetList();

			foreach (string guid in foundPrefabs)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);

				EditorUtility.DisplayProgressBar("Update PKFxFX References", path, (float)currElemCount / (float)totalElemCount);

				Object[] objs = AssetDatabase.LoadAllAssetsAtPath(path);
				foreach (var obj in objs)
				{
					if (obj == null)
						continue;
					if (UpdateAssetsReferences(obj))
					{
						referenceUpdateCount++;
						assetUpdated = true;
						EditorUtility.SetDirty(obj);
						filesToReimport.Add(path);
						updatedAssets.Add(new Asset(path));
					}
				}
				currElemCount++;
			}

			foreach (string guid in foundScripts)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);

				EditorUtility.DisplayProgressBar("Update PKFxFX References", path, (float)currElemCount / (float)totalElemCount);

				Object[] objs = AssetDatabase.LoadAllAssetsAtPath(path);
				foreach (var obj in objs)
				{
					if (obj == null)
						continue;
					if (UpdateAssetsReferences(obj))
					{
						referenceUpdateCount++;
						assetUpdated = true;
						EditorUtility.SetDirty(obj);
						filesToReimport.Add(path);
						updatedAssets.Add(new Asset(path));

					}
				}
				currElemCount++;
			}
			if (assetUpdated)
			{
				if (Provider.isActive)
				{
					Task task = Provider.Checkout(updatedAssets, CheckoutMode.Both);
					task.Wait();
				}
				AssetDatabase.SaveAssets();
			}

			EditorUtility.ClearProgressBar();

			foreach (var path in filesToReimport)
				AssetDatabase.ImportAsset(path, ImportAssetOptions.ForceUpdate);
			Debug.Log("[PopcornFX] " + referenceUpdateCount + " PKFxFX references updated.");
		}

		//----------------------------------------------------------------------------

		static bool UpdatePKFXAssetsGUID(string path)
		{
			var GUIDLookup = PKFxSettings.AssetGUID;
			bool updated = false;
			string contents = File.ReadAllText(path);
			foreach (var pair in GUIDLookup)
			{
				if (pair.m_New != pair.m_Old)
				{
					if (contents.Contains("guid: " + pair.m_Old))
					{
						contents = contents.Replace("guid: " + pair.m_Old, "guid: " + pair.m_New);
						updated = true;
					}
				}
			}
			if (updated)
				File.WriteAllText(path, contents);
			return updated;
		}

		//----------------------------------------------------------------------------

		[MenuItem("Assets/PopcornFX/Update PKFxFXAsset GUID (experimental)")]
		static void UpdatePKFxFXReferencesInScene()
		{
			string rootPath;
			if (Selection.activeObject != null)
				rootPath = AssetDatabase.GetAssetPath(Selection.activeObject);
			else
				rootPath = "Assets";
			if (!string.IsNullOrEmpty(rootPath) && File.Exists(rootPath))
			{
				rootPath = Path.GetDirectoryName(rootPath);
			}

			Scene startingScene = EditorSceneManager.GetActiveScene();
			string startingScenePath = null;

			if (!EditorSceneManager.SaveCurrentModifiedScenesIfUserWantsTo())
				return; //abort

			if (!string.IsNullOrEmpty(startingScene.path))
				startingScenePath = startingScene.path;

			int referenceUpdateCount = 0;

			string[] folders = { rootPath };

			string[] foundScenes = AssetDatabase.FindAssets("t:SceneAsset", folders);
			string[] foundPrefabs = AssetDatabase.FindAssets("t:Prefab", folders);
			string[] foundScripts = AssetDatabase.FindAssets("t:Script", folders);

			int totalElemCount = foundScenes.Length + foundPrefabs.Length + foundScripts.Length;
			int currElemCount = 0;

			AssetList updatedAssets = new AssetList();

			foreach (var guid in foundScenes)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);
				if (path.Length == 0)
					continue;

				EditorUtility.DisplayProgressBar("Update PKFxFXAsset GUID from scene: ", path, (float)currElemCount / (float)totalElemCount);

				if (UpdatePKFXAssetsGUID(path))
				{

					referenceUpdateCount++;
				}
				currElemCount++;
			}
			foreach (var guid in foundPrefabs)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);
				if (path.Length == 0)
					continue;

				EditorUtility.DisplayProgressBar("Update PKFxFXAsset GUID from prefab: ", path, (float)currElemCount / (float)totalElemCount);

				if (UpdatePKFXAssetsGUID(path))
				{
					referenceUpdateCount++;
					updatedAssets.Add(new Asset(path));
				}
				currElemCount++;
			}
			foreach (var guid in foundScripts)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);
				if (path.Length == 0)
					continue;

				EditorUtility.DisplayProgressBar("Update PKFxFXAsset GUID from script: ", path, (float)currElemCount / (float)totalElemCount);

				if (UpdatePKFXAssetsGUID(path))
				{
					referenceUpdateCount++;
					updatedAssets.Add(new Asset(path));
				}
				currElemCount++;
			}
			if (currElemCount != 0)
			{
				if (Provider.isActive)
				{
					Task task = Provider.Checkout(updatedAssets, CheckoutMode.Both);
					task.Wait();
				}
			}

			AssetDatabase.SaveAssets();
			AssetDatabase.Refresh();

			Debug.Log("[PopcornFX] " + referenceUpdateCount + " PKFxFXAssets updated.");

			EditorUtility.ClearProgressBar();
			//restore the starting scene
			if (startingScenePath != null)
				EditorSceneManager.OpenScene(startingScenePath);
		}

		//----------------------------------------------------------------------------

		public static void CreatePKFxFXMaterialsIFN(bool reimport = false)
		{
			string[] pkfxEffectAssets = AssetDatabase.FindAssets("t:PKFxEffectAsset");

			PKFxMaterialFactory factory = PKFxSettings.MaterialFactory;
			foreach (string assetGUID in pkfxEffectAssets)
			{
				PKFxEffectAsset asset = (PKFxEffectAsset)AssetDatabase.LoadAssetAtPath(AssetDatabase.GUIDToAssetPath(assetGUID), typeof(PKFxEffectAsset));

				foreach (SBatchDesc batch in asset.m_RendererDescs)
				{
					PKFxEffectAsset.MaterialUIDToIndex index = asset.m_MaterialIndexes.Find(item => item.m_UID == batch.m_UID);
					asset.m_Materials[index.m_Idx] = factory.EditorResolveMaterial(batch, asset);
				}
			}
			AssetDatabase.SaveAssets();
		}

		//----------------------------------------------------------------------------

		[MenuItem("Assets/PopcornFX/Create PKFxFX Materials If necessary")]
		static void _CreatePKFxFXMaterialsIFN(MenuCommand menuCommand)
		{
			CreatePKFxFXMaterialsIFN();
		}

		//----------------------------------------------------------------------------

		[MenuItem("Assets/PopcornFX/Update Quality levels settings")]
		static public void UpdateQualityLevelSettings()
		{
			PKFxManager.SetQualityLevelSettings();

		}

		//----------------------------------------------------------------------------

		[MenuItem("Assets/PopcornFX/Log PKFxFX References")]
		static void LogPKFxFXReferences(MenuCommand menuCommand)
		{
			string path;
			if (Selection.activeObject != null)
				path = AssetDatabase.GetAssetPath(Selection.activeObject);
			else
				path = "Assets";
			if (!string.IsNullOrEmpty(path) && File.Exists(path))
			{
				path = Path.GetDirectoryName(path);
			}

			List<string> effectsUsed = GetFxsOnAllScenesAndPrefabs(path);

			if (effectsUsed.Count != 0)
				Debug.Log(string.Format("Trying to reimport from {0} found PKFx Assets:\n {1}", path, string.Join("\n", effectsUsed)));
			else
				Debug.Log("No PKFx Assets found in " + path);
		}

		//----------------------------------------------------------------------------

		[MenuItem("Assets/PopcornFX/Update PKFxFX References")]
		static void UpdatePKFxFXReferences()
		{
			string path;
			if (Selection.activeObject != null)
				path = AssetDatabase.GetAssetPath(Selection.activeObject);
			else
				path = "Assets";
			if (!string.IsNullOrEmpty(path) && File.Exists(path))
			{
				path = Path.GetDirectoryName(path);
			}

			UpdateFxsOnAllScenesAndPrefabs(path);
		}

		//----------------------------------------------------------------------------

		[MenuItem("Assets/PopcornFX/Import Effect Pack")]
		static void ImportEffectPack()
		{
			if (PKFxSettings.GetAllAssetPath())
			{
				ImportPKFxListEditor window = ScriptableObject.CreateInstance(typeof(ImportPKFxListEditor)) as ImportPKFxListEditor;
				window.Paths = PKFxSettings.AssetPathList;
				window.ShowUtility();
			}
		}

		//----------------------------------------------------------------------------

		[MenuItem("Assets/PopcornFX/Import Pkkg file")]
		static void ImportPkkg()
		{
			string path = EditorUtility.OpenFilePanelWithFilters("Choose PopcornFX Package", "", new[] { "PopcornFX Package", "pkkg" }); ;
			if (File.Exists(path))
			{
				PKFxManager.ExtractPkkg(path, true);

				if (!string.IsNullOrEmpty(PKFxManager.ExtractedPkkg)) // Pkkg extraction/upgrade success
				{
					System.Uri fileUri = new System.Uri(PKFxManager.ExtractedPkkg);
					System.Uri referenceUri = new System.Uri(Application.dataPath);
					System.Uri relative = referenceUri.MakeRelativeUri(fileUri);
					string Srcpath = referenceUri.MakeRelativeUri(fileUri).ToString();

					PKFxSettings.PopcornPackFxPath = System.Uri.UnescapeDataString(Srcpath);
					PKFxSettings.GetAllAssetPath();
					PKFxSettings.ReimportAssets(PKFxSettings.AssetPathList, PKFxSettings.CurrentPlatformName);
				}
			}
		}

		//----------------------------------------------------------------------------

		[MenuItem("Assets/PopcornFX/Create Mesh Data")]
		static void CreateMeshData()
		{
			GameObject[] assets = Selection.gameObjects;

			foreach (GameObject cur in assets)
			{
				SkinnedMeshRenderer[] skinnedMeshRenderers = cur.GetComponentsInChildren<SkinnedMeshRenderer>();
				MeshFilter[] meshFilters = cur.GetComponentsInChildren<MeshFilter>();
				string outputName = cur.name + ".pkmm";
				string path = Path.GetDirectoryName(AssetDatabase.GetAssetPath(cur));

				if ((meshFilters.Length != 0 || skinnedMeshRenderers.Length != 0) && path.Length != 0)
				{
					foreach (SkinnedMeshRenderer skinnedMeshRenderer in skinnedMeshRenderers)
					{
						// For the skinned mesh we cannot take the transform into account as the bones skin the mesh without transforms...
						if (skinnedMeshRenderer.sharedMesh != null)
							PKFxManager.AppendMeshToBakeList(skinnedMeshRenderer.sharedMesh, Matrix4x4.identity, skinnedMeshRenderer.sharedMesh.name);
					}
					foreach (MeshFilter meshFilter in meshFilters)
					{
						if (meshFilter.sharedMesh != null)
							PKFxManager.AppendMeshToBakeList(meshFilter.sharedMesh, meshFilter.transform, meshFilter.sharedMesh.name);
					}
					PKFxManager.BakeMeshList(Path.Combine(path, outputName));
				}
			}
		}

		//----------------------------------------------------------------------------

		[MenuItem("GameObject/PopcornFX/Effect", false, 10)]
		static void CreateEmptyFX(MenuCommand menuCommand)
		{
			// Create a custom game object
			GameObject go = new GameObject("Fx");
			go.AddComponent<PKFxEmitter>();
			// TODO : redo this with new resource loader
			//PKFxFX fx = go.AddComponent<PKFxFX>();
			//string[] pkfxs = Directory.GetFiles("Assets/StreamingAssets/PackFx/", "*.pkfx", SearchOption.AllDirectories);
			//if (pkfxs.Length > 0)
			//{
			//	fx.m_FxName = pkfxs[0].Substring("Assets/StreamingAssets/PackFx/".Length);
			//	UnityEngine.Debug.Log(fx.m_FxName);
			//}
			// Ensure it gets reparented if this was a context click (otherwise does 
			GameObjectUtility.SetParentAndAlign(go, menuCommand.context as GameObject);
			// Register the creation in the undo system
			Undo.RegisterCreatedObjectUndo(go, "Create " + go.name);
			Selection.activeObject = go;
		}

		//----------------------------------------------------------------------------

		[MenuItem("GameObject/PopcornFX/Camera", false, 10)]
		static void CreatePKCamera(MenuCommand menuCommand)
		{
			// Create a custom game object
			GameObject go = new GameObject("PKFxCamera");
			go.AddComponent<Camera>();
			go.AddComponent<PKFxCamera>();
			// Ensure it gets reparented if this was a context click (otherwise does 
			//GameObjectUtility.SetParentAndAlign(go, menuCommand.context as GameObject);
			// Register the creation in the undo system
			Undo.RegisterCreatedObjectUndo(go, "Create " + go.name);
			Selection.activeObject = go;
		}

		[MenuItem("GameObject/PopcornFX/Rendering Manager", false, 10)]
		static void CreatePKRenderingPlugin(MenuCommand menuCommand)
		{
			// Create a custom game object
			GameObject go = new GameObject("PKFxRenderingManager");
			go.AddComponent<PKFxRenderingPlugin>();
			// Ensure it gets reparented if this was a context click (otherwise does 
			//GameObjectUtility.SetParentAndAlign(go, menuCommand.context as GameObject);
			// Register the creation in the undo system
			Undo.RegisterCreatedObjectUndo(go, "Create " + go.name);
			Selection.activeObject = go;
		}

		//========== HELP =====================================================
		//
		//
		//
		//
		//
		//=====================================================================

		[MenuItem("Help/PopcornFX Wiki")]
		static void LinkToWiki()
		{
			Application.OpenURL("http://wiki.popcornfx.com/index.php/Unity_V3");
		}

		//----------------------------------------------------------------------------
	}
}