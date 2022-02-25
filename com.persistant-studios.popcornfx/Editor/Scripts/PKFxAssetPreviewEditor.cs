using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;


namespace PopcornFX
{

	[ExecuteInEditMode]
	public class PKFxAssetPreviewEditor : EditorWindow
	{
		public struct SPreviewBatch
		{
			public Material m_Material;
			public GameObject m_GO;
			public int m_UID;

		}


		private bool					m_Initialized = false;
		private PreviewRenderUtility	m_PreviewRenderer;
		private List<SPreviewBatch>		m_PreviewBatches;

		PKFxEmitter	m_Emitter;
		Editor		m_EmitterEditor;

		GameObject	m_PreviewGameObject = null;
		GameObject prefab;
		[MenuItem("Example/GameObject Editor")]
		static void ShowWindow()
		{
			GetWindowWithRect<PKFxAssetPreviewEditor>(new Rect(0, 0, 256, 256));
		}

		public void Initalize()
		{
			m_PreviewRenderer = new PreviewRenderUtility();
			m_Initialized = true;
		}

		public GameObject GetNewRenderingObject(string name)
		{
			if (m_PreviewGameObject == null)
			{
				m_PreviewGameObject = new GameObject("PopcornFX Renderers");
				m_PreviewGameObject.transform.position = Vector3.zero;
			}
			GameObject gameObject = new GameObject(name);
			gameObject.transform.parent = m_PreviewGameObject.transform;
			return gameObject;
		}

		public void SetEmitter(PKFxEmitter emitter)
		{
			PKFxManagerImpl.ClearRenderers();
			m_Emitter = emitter;

			m_EmitterEditor = Editor.CreateEditor(m_Emitter);

			m_PreviewBatches = new List<SPreviewBatch>();

			if (m_PreviewGameObject)
			{
				DestroyImmediate(m_PreviewGameObject);
				m_PreviewGameObject = null;
			}

			foreach (SBatchDesc batchDesc in m_Emitter.m_FxAsset.m_RendererDescs) 
			{
				SPreviewBatch previewBatch = new SPreviewBatch();
				previewBatch.m_Material = PKFxSettings.MaterialFactory.ResolveParticleMaterial(batchDesc, m_Emitter.EffectAsset);

				previewBatch.m_GO = GetNewRenderingObject(batchDesc.m_GeneratedName);
				previewBatch.m_UID = PKFxManagerImpl.SetupRenderingObject(previewBatch.m_GO, batchDesc, previewBatch.m_Material);

				m_PreviewBatches.Add(previewBatch);
			}
		}

		void BeginDraw(Rect r)
		{
			if (m_PreviewRenderer == null)
				m_PreviewRenderer = new PreviewRenderUtility();

			m_PreviewRenderer.camera.transform.position = new Vector3(0, 0, 5);
			m_PreviewRenderer.camera.transform.LookAt(Vector3.zero, Vector3.up);
			m_PreviewRenderer.camera.farClipPlane = 30;
			m_PreviewRenderer.camera.clearFlags = CameraClearFlags.Skybox;

			m_PreviewRenderer.lights[0].intensity = 0.5f;
			m_PreviewRenderer.lights[0].transform.rotation = Quaternion.Euler(30f, 30f, 0f);
			m_PreviewRenderer.lights[1].intensity = 0.5f;

			m_PreviewRenderer.BeginPreview(r, GUIStyle.none);

		}

		void EndDraw(Rect r)
		{
			bool fog = RenderSettings.fog;
			Unsupported.SetRenderSettingsUseFogNoDirty(false);
			m_PreviewRenderer.camera.Render();
			Unsupported.SetRenderSettingsUseFogNoDirty(fog);

			Texture texture = m_PreviewRenderer.EndPreview();
			GUI.DrawTexture(r, texture);
		}

		public void DrawRenderPreview()
		{
			if (m_Emitter != null)
			{
				if (m_PreviewBatches == null)
					SetEmitter(m_Emitter);
				MeshFilter[] meshFilters;
				foreach (SPreviewBatch batch in m_PreviewBatches)
				{
					meshFilters = batch.m_GO.GetComponentsInChildren<MeshFilter>();
					for (int i = 0; i < meshFilters.Length; i++)
					{
						if (meshFilters[i].sharedMesh)
						{
							MeshRenderer meshRenderer = meshFilters[i].gameObject.GetComponent<MeshRenderer>();
							for (int j = 0; j < meshFilters[i].sharedMesh.subMeshCount; j++)
								if (meshRenderer != null)
								{
									m_PreviewRenderer.DrawMesh(
										meshFilters[i].sharedMesh,
										meshRenderer.transform.localToWorldMatrix,
										meshRenderer.sharedMaterials[j],
										j);
								}
						}
					}
				}
			}

			if (prefab != null)
			{
				MeshFilter[] meshFilters = prefab.GetComponentsInChildren<MeshFilter>();
				for (int i = 0; i < meshFilters.Length; i++)
				{
					if (meshFilters[i].sharedMesh)
					{
						MeshRenderer meshRenderer = meshFilters[i].gameObject.GetComponent<MeshRenderer>();
						for (int j = 0; j < meshFilters[i].sharedMesh.subMeshCount; j++)
							if (meshRenderer != null)
							{
								m_PreviewRenderer.DrawMesh(
									meshFilters[i].sharedMesh,
									meshRenderer.transform.localToWorldMatrix,
									meshRenderer.sharedMaterials[j],
									j);
							}
					}
				}
			}
		}

		void OnGUI()
		{
			PKFxEmitter emitter = (PKFxEmitter)EditorGUILayout.ObjectField(m_Emitter, typeof(PKFxEmitter), true);
			prefab = (GameObject)EditorGUILayout.ObjectField(prefab, typeof(GameObject), true);
			if (emitter != m_Emitter)
			{
				SetEmitter(emitter);
			}

			GUIStyle bgColor = new GUIStyle();

			if (!m_Initialized)
				Initalize();


			if (m_Emitter != null)
			{
				Rect r2 = GUILayoutUtility.GetRect(256.0f, 256.0f);
				BeginDraw(r2);// GUILayoutUtility.GetRect(256.0f, 256.0f));// position.width, position.height));
				DrawRenderPreview();
				EndDraw(r2);//GUILayoutUtility.GetRect(256.0f, 256.0f));//position.width, position.height));
			}
		}
	}
}