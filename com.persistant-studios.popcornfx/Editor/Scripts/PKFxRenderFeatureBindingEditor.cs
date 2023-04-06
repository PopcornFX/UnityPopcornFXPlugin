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
		bool m_ShaderGraphFoldout = true;

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
			"Emissive",
			"EmissiveRampMap",
			"SkeletonAnimation",
			"SkeletonAnimationInterpolate",
			"SkeletonAnimationInterpolateTracks"
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
			SerializedProperty	doubleSided = serializedObject.FindProperty("m_DoubleSided");
			SerializedProperty	SupportedShaderMask = serializedObject.FindProperty("m_SupportedShaderMask");
			SerializedProperty	MandatoryShaderMask = serializedObject.FindProperty("m_MandatoryShaderMask");
			SerializedProperty	BlendMode = serializedObject.FindProperty("m_BlendMode");
			SerializedProperty	BillboardingLocation = serializedObject.FindProperty("m_BillboardingLocation");

			UseShader.boolValue = EditorGUILayout.Toggle("Use Shader", UseShader.boolValue);
			if (UseShader.boolValue)
				Shader.objectReferenceValue = EditorGUILayout.ObjectField(Shader.objectReferenceValue, typeof(Shader), false) as Shader;
			else
				Material.objectReferenceValue = EditorGUILayout.ObjectField(Material.objectReferenceValue, typeof(Material), false) as Material;

			bool	bindingHasMeshRenderer = false;
			bool	bindingHasTransparent = false;
			bool	bindingHasMasked = false;

			m_RendererFoldout = EditorGUILayout.BeginFoldoutHeaderGroup(m_RendererFoldout, "Renderer Types");
			if (m_RendererFoldout)
			{
				int ArraySize = RenderTypes.arraySize;
				for (int i = 0; i < ArraySize; i++)
				{
					SerializedProperty type = RenderTypes.GetArrayElementAtIndex(i);
					ERendererType rendererType = (ERendererType)i;
					if (rendererType == ERendererType.Mesh)
					{
						if (type.boolValue)
							bindingHasMeshRenderer = true;
					}
				}
				for (int i = 0; i < ArraySize; i++)
				{
					SerializedProperty type = RenderTypes.GetArrayElementAtIndex(i);

					EditorGUILayout.BeginHorizontal();
					ERendererType rendererType = (ERendererType)i;
					bool disabledGroup = bindingHasMeshRenderer && rendererType != ERendererType.Mesh;
					EditorGUI.BeginDisabledGroup(disabledGroup);
					EditorGUILayout.LabelField(_RendererTypeToString(rendererType));
					if (disabledGroup)
						type.boolValue = false;
					type.boolValue = EditorGUILayout.Toggle(type.boolValue);
					EditorGUI.EndDisabledGroup();
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
						EBlendMode blendMode = (EBlendMode)i;
						EditorGUILayout.LabelField(_BlendModeToString(blendMode));
						type.boolValue = EditorGUILayout.Toggle(type.boolValue);
						if (blendMode == EBlendMode.Masked)
						{
							if (type.boolValue)
								bindingHasMasked = true;
						}
						else if (blendMode != EBlendMode.Solid)
						{
							if (type.boolValue)
								bindingHasTransparent = true;
						}
						EditorGUILayout.EndHorizontal();
					}
				}
				EditorGUILayout.EndFoldoutHeaderGroup();

			}

			SupportedShaderMask.intValue = EditorGUILayout.MaskField("Supported Shader Variation Mask", SupportedShaderMask.intValue, m_ShaderVarationFlagsString);
			MandatoryShaderMask.intValue = EditorGUILayout.MaskField("Mandatory Shader Variation Mask", MandatoryShaderMask.intValue, m_ShaderVarationFlagsString);
			SupportedShaderMask.intValue |= MandatoryShaderMask.intValue;
			PKFxShaderInputBindings shaderInputs = serializedObject.targetObject as PKFxShaderInputBindings;
			shaderInputs.DrawEditorShaderInputBindings(	SupportedShaderMask.intValue,
														bindingHasTransparent,
														bindingHasMasked,
														bindingHasMeshRenderer,
														true);
			bool usesShaderGraphs = (UnityEngine.Rendering.GraphicsSettings.currentRenderPipeline != null) && (UnityEngine.Rendering.GraphicsSettings.currentRenderPipeline.name == "UniversalRenderPipelineAsset" || UnityEngine.Rendering.GraphicsSettings.currentRenderPipeline.name == "HDRenderPipelineAsset");
			if (bindingHasMeshRenderer && usesShaderGraphs)
			{
				m_ShaderGraphFoldout = EditorGUILayout.BeginFoldoutHeaderGroup(m_ShaderGraphFoldout, "Shader Graph Integration");
				if (m_ShaderGraphFoldout)
				{
					EditorGUILayout.BeginHorizontal();
					EditorGUILayout.LabelField("DoubleSided");
					doubleSided.boolValue = EditorGUILayout.Toggle(doubleSided.boolValue);
					EditorGUILayout.EndHorizontal();
				}
				EditorGUILayout.EndFoldoutHeaderGroup();
			}
			serializedObject.ApplyModifiedProperties();
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
