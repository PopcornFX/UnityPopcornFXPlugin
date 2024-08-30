using System.Collections.Generic;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;

namespace PopcornFX
{
#if UNITY_2021_2_OR_NEWER
	static class PKFxDragDropHandler
	{
		[InitializeOnLoadMethod]
		static void OnLoad()
		{
			DragAndDrop.AddDropHandler(OnSceneDrop);
			DragAndDrop.AddDropHandler(OnHierarchyDrop);
			DragAndDrop.AddDropHandler(OnInspectorDrop);
		}

		private static List<GameObject> _gameObjectsToSelect = new List<GameObject>();

		private static List<PKFxEffectAsset> _GetEffectAssets()
		{
			List<PKFxEffectAsset> effectAssets = new List<PKFxEffectAsset>();
			foreach (Object obj in DragAndDrop.objectReferences)
			{
				PKFxEffectAsset effectAsset = obj as PKFxEffectAsset;
				if (effectAsset != null)
				{
					effectAssets.Add(effectAsset);
				}
			}
			return effectAssets;
		}

		private static void _SelectGameObjectsAndMarkPrefabStageDirtyIFN()
		{
			if (_gameObjectsToSelect.Count > 0)
			{
				Selection.objects = _gameObjectsToSelect.ToArray();
				_gameObjectsToSelect.Clear();
			}

			PrefabStage prefabStage = PrefabStageUtility.GetCurrentPrefabStage();
			if (prefabStage != null)
				EditorSceneManager.MarkSceneDirty(prefabStage.scene);
		}

		private static GameObject _CreateEmitter(PKFxEffectAsset effectAsset, Transform parent)
		{
			GameObject go = new GameObject(effectAsset.name.Replace(".pkfx", ""));
			PKFxEmitter emitter = go.AddComponent<PKFxEmitter>();
			emitter.UpdateEffectAsset(effectAsset, false);
			if (parent != null)
				GameObjectUtility.SetParentAndAlign(go, parent.gameObject);
			Undo.RegisterCreatedObjectUndo(go, "Create " + go.name);
			_gameObjectsToSelect.Add(go);
			return go;
		}

		static DragAndDropVisualMode OnSceneDrop(Object dropUpon, Vector3 worldPosition, Vector2 viewportPosition, Transform parentForDraggedObjects, bool perform)
		{
			List<PKFxEffectAsset> effectAssets = _GetEffectAssets();
			if (effectAssets.Count == 0)
				return DragAndDropVisualMode.None;

			if (perform)
			{
				foreach (PKFxEffectAsset effectAsset in effectAssets)
				{
					GameObject go = _CreateEmitter(effectAsset, parentForDraggedObjects);
					go.transform.position = worldPosition;
				}

				_SelectGameObjectsAndMarkPrefabStageDirtyIFN();
			}
			return DragAndDropVisualMode.Move;
		}

		static DragAndDropVisualMode OnHierarchyDrop(int dropTargetInstanceID, HierarchyDropFlags dropMode, Transform parentForDraggedObjects, bool perform)
		{
			List<PKFxEffectAsset> effectAssets = _GetEffectAssets();
			if (effectAssets.Count == 0)
				return DragAndDropVisualMode.None;

			if (perform)
			{
				foreach (PKFxEffectAsset effectAsset in effectAssets)
				{
					Transform parent = null;
					int siblingIndex = -1;
					if (parentForDraggedObjects != null) // Only in edit prefab mode
						parent = parentForDraggedObjects;
					else
					{
						GameObject dropTarget = EditorUtility.InstanceIDToObject(dropTargetInstanceID) as GameObject;
						if (dropTarget != null)
						{
							if (dropMode.HasFlag(HierarchyDropFlags.DropUpon))
								parent = dropTarget.transform;
							else if (dropMode.HasFlag(HierarchyDropFlags.DropAfterParent))
							{
								parent = dropTarget.transform.parent;
								siblingIndex = 0;
							}
							else if (dropMode.HasFlag(HierarchyDropFlags.DropAbove))
							{
								parent = dropTarget.transform.parent;
								siblingIndex = dropTarget.transform.GetSiblingIndex();
							}
							else if (dropMode.HasFlag(HierarchyDropFlags.DropBetween))
							{
								parent = dropTarget.transform.parent;
								siblingIndex = dropTarget.transform.GetSiblingIndex() + 1;
							}
						}
					}

					if (parent != null &&
						!dropMode.HasFlag(HierarchyDropFlags.DropUpon) &&
						PrefabUtility.IsAnyPrefabInstanceRoot(parent.gameObject))
					{
						if (EditorUtility.DisplayDialog("Cannot restructure Prefab instance", "Children of a Prefab instance cannot be deleted or moved, and components cannot be reordered.\n\nYou can open the Prefab in Prefab Mode to restructure the Prefab Asset itself, or unpack the Prefab instance to remove its Prefab connection.", "Open Prefab", "Cancel"))
						{
							AssetDatabase.OpenAsset(PrefabUtility.GetCorrespondingObjectFromOriginalSource(parent.gameObject));
						}
						return DragAndDropVisualMode.Move;
					}

					GameObject go = _CreateEmitter(effectAsset, parent);
					if (siblingIndex != -1)
						go.transform.SetSiblingIndex(siblingIndex);
				}

				_SelectGameObjectsAndMarkPrefabStageDirtyIFN();
			}

			return DragAndDropVisualMode.Move;
		}

		static DragAndDropVisualMode OnInspectorDrop(Object[] targets, bool perform)
		{
			List<PKFxEffectAsset> effectAssets = _GetEffectAssets();
			if (effectAssets.Count == 0)
				return DragAndDropVisualMode.None;

			List<GameObject> targetGameObjects = new List<GameObject>();
			foreach (Object target in targets)
			{
				GameObject go = target as GameObject;
				if (go != null)
					targetGameObjects.Add(go);
			}
			if (targetGameObjects.Count == 0)
				return DragAndDropVisualMode.None;

			if (perform)
			{
				foreach (GameObject targetGameObject in targetGameObjects)
				{
					PKFxEmitter[] emitters = targetGameObject.GetComponents<PKFxEmitter>();
					int effectAssetId = 0;
					foreach (PKFxEffectAsset effectAsset in effectAssets)
					{
						PKFxEmitter emitter = null;
						if (effectAssetId < emitters.Length)
							emitter = emitters[effectAssetId];
						else
							emitter = targetGameObject.AddComponent<PKFxEmitter>();
						emitter.UpdateEffectAsset(effectAsset, false);
						effectAssetId++;
					}
				}
			}

			return DragAndDropVisualMode.Link;
		}
	}
#endif
}