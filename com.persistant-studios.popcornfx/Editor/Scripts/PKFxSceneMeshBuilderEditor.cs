//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEditor;
using UnityEngine;
using System.IO;
using System.Collections;

namespace PopcornFX
{

	[CustomEditor(typeof(PKFxSceneMeshBuilder))]
	public class PKFxSceneMeshBuilderEditor : Editor
	{
		SerializedProperty m_OutputPkmmPath;
		SerializedProperty m_GameObjectsToSearch;
		SerializedProperty m_MeshGameObjects;

		//----------------------------------------------------------------------------

		void OnEnable()
		{
			m_OutputPkmmPath = serializedObject.FindProperty("m_OutputPkmmPath");
			m_GameObjectsToSearch = serializedObject.FindProperty("m_GameObjectsToSearch");
			m_MeshGameObjects = serializedObject.FindProperty("m_MeshGameObjects");
		}

		//----------------------------------------------------------------------------

		public override void OnInspectorGUI()
		{
			serializedObject.Update();
			DrawDefaultInspector();
			EditorGUI.BeginDisabledGroup(Application.isPlaying);
			if (GUILayout.Button("Build meshes"))
			{
				FindMeshes();
				BuildMeshes();
			}
			EditorGUI.EndDisabledGroup();
			serializedObject.ApplyModifiedProperties();
		}

		//----------------------------------------------------------------------------

		private void FindMeshes()
		{
			this.m_MeshGameObjects.ClearArray();
			for (int i = 0; i < m_GameObjectsToSearch.arraySize; i++)
			{
				FillMeshesWithChildren(m_GameObjectsToSearch.GetArrayElementAtIndex(i).objectReferenceValue as GameObject);
			}
		}

		//----------------------------------------------------------------------------

		private void FillMeshesWithChildren(GameObject o)
		{
			foreach (Transform t in o.transform)
			{
				FillMeshesWithChildren(t.gameObject);
			}

			if (o.GetComponent<MeshFilter>() != null)
			{
				// add gameObject to meshGameObject
				this.m_MeshGameObjects.InsertArrayElementAtIndex(this.m_MeshGameObjects.arraySize);
				this.m_MeshGameObjects.GetArrayElementAtIndex(this.m_MeshGameObjects.arraySize - 1).objectReferenceValue = o as Object;
			}
		}

		//----------------------------------------------------------------------------

		private void BuildMeshes()
		{
			string outputPkmm = m_OutputPkmmPath.stringValue;
			if (outputPkmm.Length <= 0)
			{
				Debug.LogError("[PopcornFX] SceneMeshBuilder: invalid mesh path", this);
				return;
			}
			PKFxManager.StartupPopcorn(false);
			PKFxManager.SetSceneMesh(null);

			// We just need the positions and the normal for the scene mesh:
			int sceneMeshVertexAttributes = (1 << (int)EMeshVertexAttributes.Attrib_Normal);

			for (int meshi = 0; meshi < m_MeshGameObjects.arraySize; meshi++)
			{
				GameObject obj = m_MeshGameObjects.GetArrayElementAtIndex(meshi).objectReferenceValue as GameObject;
				Mesh mesh = obj.GetComponent<MeshFilter>().sharedMesh;

				if (!PKFxManager.AppendMeshToBakeList(mesh, obj.transform, "Default", sceneMeshVertexAttributes, meshi == 0))
				{
					Debug.LogError("[PopcornFX] SceneMeshBuilder: failed to load mesh " + obj.name + "", this);
				}
			}
			// Disabled for the moment as we do not have a PackFx folder in the streaming assets
			bool success = PKFxManager.BakeMeshList(outputPkmm);
			if (!success)
			{
				Debug.LogError("[PopcornFX] SceneMeshBuilder: failed to save scene mesh " + this.name + "", this);
			}
			else
			{
				Debug.Log("[PopcornFX] SceneMeshBuilder: mesh ok " + this.name);
			}
		}
	}
}