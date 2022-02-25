//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

namespace PopcornFX
{
	[CustomEditor(typeof(PKFxRenderFeatureBinding))]
	public class PKFxRenderFeatureBindingEditor : Editor
	{
		bool m_RendererFoldout = true;
		bool m_BlendFoldout = true;
		bool m_AdvancedBinding = true;

		//bool m_ShaderBindingFoldout = true;


		static int[] m_BillboardingLocationsInt = new int[]
		{
			0,
			1,
		};
		static string[] m_BillboardingLocationsString = new string[]
		{
			"CPU",
			"Vertex",
		};
		static string[] m_ShaderVarationFlagsString = new string[]
		{
			"RibbonComplex",
			"AnimBlend",
			"AlphaRemap",
			"Lighting",
			"LightingLegacy",
			"Soft",
			"DistortionMap",
			"Color",
			"DiffuseMap",
			"DoubleSided",
			"CastShadow",
			"Atlas",
			"Size2",
			"DiffuseRampMap",
			"FluidVAT",
			"SoftVAT",
			"RigidVAT",
		};

		public static string GetSelectedPathOrFallback()
		{
			string path = "Assets";

			foreach (UnityEngine.Object obj in Selection.GetFiltered(typeof(UnityEngine.Object), SelectionMode.Assets))
			{
				path = AssetDatabase.GetAssetPath(obj);
				if (!string.IsNullOrEmpty(path) && File.Exists(path))
				{
					path = Path.GetDirectoryName(path);
					break;
				}
			}
			return path;
		}

		[MenuItem("Assets/Create/PopcornFX/Material Factory/Render Feature Binding")]
		static void CreatePKFxRenderFeatureBinding()
		{
			string folderPath = GetSelectedPathOrFallback();
			string path = Path.Combine(folderPath, "MyRenderFeatureBinding.asset");
		
			PKFxRenderFeatureBinding binding = CreateInstance<PKFxRenderFeatureBinding>();

			Directory.CreateDirectory(path);
			AssetDatabase.CreateAsset(binding, path);
			AssetDatabase.SaveAssets();

			EditorUtility.FocusProjectWindow();

			Selection.activeObject = binding;
			// Print the path of the created asset
			Debug.Log(AssetDatabase.GetAssetPath(binding));
		}
		public override void OnInspectorGUI()
		{
			SerializedProperty	UseShader = serializedObject.FindProperty("m_UseShader");
			SerializedProperty	Material = serializedObject.FindProperty("m_Material");
			SerializedProperty	Shader = serializedObject.FindProperty("m_Shader");
			SerializedProperty	RenderTypes = serializedObject.FindProperty("m_RenderTypes");
			SerializedProperty	ShaderMask = serializedObject.FindProperty("m_ShaderMask");
			SerializedProperty	BlendMode = serializedObject.FindProperty("m_BlendMode");
			SerializedProperty	BillboardingLocation = serializedObject.FindProperty("m_BillboardingLocation");
			SerializedProperty	MeshColorPropertyName = serializedObject.FindProperty("m_MeshColorPropertyName");
			SerializedProperty	NormalMapPropertyName = serializedObject.FindProperty("m_NormalMapPropertyName");

			UseShader.boolValue = EditorGUILayout.Toggle("Use Shader", UseShader.boolValue);
			if (UseShader.boolValue)
				Shader.objectReferenceValue = EditorGUILayout.ObjectField(Shader.objectReferenceValue, typeof(Shader), false) as Shader;
			else
				Material.objectReferenceValue = EditorGUILayout.ObjectField(Material.objectReferenceValue, typeof(Material), false) as Material;
			

			m_RendererFoldout = EditorGUILayout.BeginFoldoutHeaderGroup(m_RendererFoldout, "Renderer Types");
			if (m_RendererFoldout)
			{
				int ArraySize = RenderTypes.arraySize;
				for (int i = 0; i < ArraySize; i++)
				{
					SerializedProperty type = RenderTypes.GetArrayElementAtIndex(i);

					EditorGUILayout.BeginHorizontal();
					EditorGUILayout.LabelField(_RendererTypeToString((ERendererType)i));
					type.boolValue = EditorGUILayout.Toggle(type.boolValue);
					EditorGUILayout.EndHorizontal();
				}
				if (RenderTypes.GetArrayElementAtIndex((int)ERendererType.Billboard).boolValue)
				{
					BillboardingLocation.intValue = EditorGUILayout.IntPopup("Billboarding Location: ", BillboardingLocation.intValue, m_BillboardingLocationsString, m_BillboardingLocationsInt);
				}
			}

			EditorGUILayout.EndFoldoutHeaderGroup();


			if (BlendMode.arraySize > 0)
			{
				m_BlendFoldout = EditorGUILayout.BeginFoldoutHeaderGroup(m_BlendFoldout, "Blend Modes");
				if (m_BlendFoldout)
				{
					int ArraySize = BlendMode.arraySize;
					for (int i = 0; i < ArraySize; i++)
					{
						SerializedProperty type = BlendMode.GetArrayElementAtIndex(i);

						EditorGUILayout.BeginHorizontal();
						EditorGUILayout.LabelField(_BlendModeToString((EBlendMode)i));
						type.boolValue = EditorGUILayout.Toggle(type.boolValue);
						EditorGUILayout.EndHorizontal();
					}
				}
				EditorGUILayout.EndFoldoutHeaderGroup();
			}

			ShaderMask.intValue = EditorGUILayout.MaskField("Shader Variation Mask", ShaderMask.intValue, m_ShaderVarationFlagsString);

			if (RenderTypes.GetArrayElementAtIndex((int)ERendererType.Mesh).boolValue ||
				HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_Lighting))
			{
				m_AdvancedBinding = EditorGUILayout.BeginFoldoutHeaderGroup(m_AdvancedBinding, "Advanced Bindings");
				if (m_AdvancedBinding)
				{
					if (RenderTypes.GetArrayElementAtIndex((int)ERendererType.Mesh).boolValue)
					{
						MeshColorPropertyName.stringValue = EditorGUILayout.TextField("Mesh Color Property Name: ", MeshColorPropertyName.stringValue);
					}

					if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_Lighting))
					{
						NormalMapPropertyName.stringValue = EditorGUILayout.TextField("Normal Map Property Name: ", NormalMapPropertyName.stringValue);
					}
				}
				EditorGUILayout.EndFoldoutHeaderGroup();
			}
			serializedObject.ApplyModifiedProperties();
		}

		public bool HasShaderVariationFlag(int mask, EShaderVariationFlags flag)
		{
			return (mask & (int)flag) == (int)flag;
		}

		public string _BlendModeToString(EBlendMode type)
		{
			string result;
			switch (type)
			{
				case EBlendMode.Additive:
					result = "Additive";
					break;
				case EBlendMode.AdditiveNoAlpha:
					result = "AdditiveNoAlpha";
					break;
				case EBlendMode.AlphaBlend:
					result = "AlphaBlend";
					break;
				case EBlendMode.PremultipliedAlpha:
					result = "PremultipliedAlpha";
					break;
				case EBlendMode.Solid:
					result = "Solid";
					break;
				case EBlendMode.Masked:
					result = "Masked";
					break;
				default:
					result = "Blend mode not supported";
					break;
			}
			return result;

		}
		public string _RendererTypeToString(ERendererType type)
		{
			string result;
			switch (type)
			{
			case ERendererType.Billboard:
				result = "Billboard";
				break;
			case ERendererType.Ribbon:
				result = "Ribbon";
				break;
			case ERendererType.Mesh:
				result = "Mesh";
				break;
			case ERendererType.Triangle:
				result = "Triangle";
				break;
			default:
				result = "Renderer not supported";
				break;
			}
			return result;
		}

	}
}
