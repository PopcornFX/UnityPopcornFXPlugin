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

namespace PopcornFX
{
	public class PKFxMenus : MonoBehaviour
	{
		private static void WarnRestart()
		{
			Debug.LogWarning("[PopcornFX] Settings : For this setting to take effect, you'll need to re-open your project.");
		}

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

		private static bool UpdateMonobehaviorComponentReferences(Object obj)
		{
			const BindingFlags flags = /*BindingFlags.NonPublic | */BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static;
			FieldInfo[] fields = obj.GetType().GetFields(flags);
			bool fieldsPatched = false;

			foreach (FieldInfo fieldInfo in fields)
			{
				if (fieldInfo.ReflectedType == typeof(PKFxEffectAsset))
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

		public static void UpdateFxsOnAllScenesAndPrefabs()
		{
			Scene startingScene = EditorSceneManager.GetActiveScene();
			string startingScenePath = null;

			if (!EditorSceneManager.SaveCurrentModifiedScenesIfUserWantsTo())
				return; //abort

			if (!string.IsNullOrEmpty(startingScene.path))
				startingScenePath = startingScene.path;

			string[] foundScenes = AssetDatabase.FindAssets("t:SceneAsset", null);
			string[] foundPrefabs = AssetDatabase.FindAssets("t:Prefab");

			int referenceUpdateCount = 0;
			int totalElemCount = foundScenes.Length + foundPrefabs.Length;
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

			bool prefabsUpdated = false;
			List<string> filesToReimport = new List<string>();
			AssetList prefabAssets = new AssetList();

			foreach (string guid in foundPrefabs)
			{
				string path = AssetDatabase.GUIDToAssetPath(guid);

				EditorUtility.DisplayProgressBar("Update PKFxFX References", path, (float)currElemCount / (float)totalElemCount);

				Object[] objs = AssetDatabase.LoadAllAssetsAtPath(path);
				foreach (var obj in objs)
				{
					if (UpdateAssetsReferences(obj))
					{
						referenceUpdateCount++;
						prefabsUpdated = true;
						EditorUtility.SetDirty(obj);
						filesToReimport.Add(path);
						prefabAssets.Add(new Asset(path));
					}
				}
				currElemCount++;
			}
			if (prefabsUpdated)
			{
				if (Provider.isActive)
				{
					Task task = Provider.Checkout(prefabAssets, CheckoutMode.Both);
					task.Wait();
				}
				AssetDatabase.SaveAssets();
			}

			EditorUtility.ClearProgressBar();

			foreach (var path in filesToReimport)
				AssetDatabase.ImportAsset(path, ImportAssetOptions.ForceUpdate);
			Debug.Log("[PopcornFX] " + referenceUpdateCount + " PKFxFX references updated.");
		}

		[MenuItem("Assets/PopcornFX/Update PKFxFX References")]
		static void UpdatePKFxFXReferences()
		{
			UpdateFxsOnAllScenesAndPrefabs();
		}

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