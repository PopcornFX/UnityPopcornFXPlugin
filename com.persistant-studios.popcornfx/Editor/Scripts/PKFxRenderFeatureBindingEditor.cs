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
			"Emissive",
			"EmissiveRampMap"
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

			ShaderMask.intValue = EditorGUILayout.MaskField("Shader Variation Mask", ShaderMask.intValue, m_ShaderVarationFlagsString);

			{
				m_AdvancedBinding = EditorGUILayout.BeginFoldoutHeaderGroup(m_AdvancedBinding, "Advanced Bindings");

				if (m_AdvancedBinding)
				{
					if (bindingHasTransparent)
					{
						SerializedProperty SourceBlendPropertyName = serializedObject.FindProperty("m_SourceBlendPropertyName");
						SourceBlendPropertyName.stringValue = EditorGUILayout.TextField("Source blend mode (Int - UnityEngine.Rendering.BlendMode): ", SourceBlendPropertyName.stringValue);
						SerializedProperty DestinationBlendPropertyName = serializedObject.FindProperty("m_DestinationBlendPropertyName");
						DestinationBlendPropertyName.stringValue = EditorGUILayout.TextField("Destination blend mode (Int - UnityEngine.Rendering.BlendMode): ", DestinationBlendPropertyName.stringValue);
					}
					if (bindingHasMasked)
					{
						SerializedProperty MaskThresholdPropertyName = serializedObject.FindProperty("m_MaskThresholdPropertyName");
						MaskThresholdPropertyName.stringValue = EditorGUILayout.TextField("Cutout alpha for opaque (Float): ", MaskThresholdPropertyName.stringValue);
					}
					SerializedProperty RotateUVsPropertyName = serializedObject.FindProperty("m_RotateUVsPropertyName");
					RotateUVsPropertyName.stringValue = EditorGUILayout.TextField("RotateUVs (Int 0 or 1): ", RotateUVsPropertyName.stringValue);
					if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_DiffuseMap))
					{
						SerializedProperty DiffuseMapPropertyName = serializedObject.FindProperty("m_DiffuseMapPropertyName");
						DiffuseMapPropertyName.stringValue = EditorGUILayout.TextField("Diffuse (Texture2D): ", DiffuseMapPropertyName.stringValue);
					}
					if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_DistortionMap))
					{
						SerializedProperty DistortionMapPropertyName = serializedObject.FindProperty("m_DistortionMapPropertyName");
						DistortionMapPropertyName.stringValue = EditorGUILayout.TextField("Ditortion (Texture2D): ", DistortionMapPropertyName.stringValue);
					}
					if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_Emissive))
					{
						SerializedProperty EmissiveMapPropertyName = serializedObject.FindProperty("m_EmissiveMapPropertyName");
						EmissiveMapPropertyName.stringValue = EditorGUILayout.TextField("Emissive (Texture2D): ", EmissiveMapPropertyName.stringValue);
					}
					if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_EmissiveRamp))
					{
						SerializedProperty EmissiveRampMapPropertyName = serializedObject.FindProperty("m_EmissiveRampMapPropertyName");
						EmissiveRampMapPropertyName.stringValue = EditorGUILayout.TextField("Emissive ramp (Texture2D): ", EmissiveRampMapPropertyName.stringValue);
					}
					if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_AlphaRemap))
					{
						SerializedProperty AlphaRemapPropertyName = serializedObject.FindProperty("m_AlphaRemapPropertyName");
						AlphaRemapPropertyName.stringValue = EditorGUILayout.TextField("Alpha remap (Texture2D): ", AlphaRemapPropertyName.stringValue);
					}
					if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_DiffuseRamp))
					{
						SerializedProperty DiffuseRampMapPropertyName = serializedObject.FindProperty("m_DiffuseRampMapPropertyName");
						DiffuseRampMapPropertyName.stringValue = EditorGUILayout.TextField("Diffuse ramp (Texture2D): ", DiffuseRampMapPropertyName.stringValue);
					}
					if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_Soft))
					{
						SerializedProperty InvSoftnessDistancePropertyName = serializedObject.FindProperty("m_InvSoftnessDistancePropertyName");
						InvSoftnessDistancePropertyName.stringValue = EditorGUILayout.TextField("Inverse softness distance (Float): ", InvSoftnessDistancePropertyName.stringValue);
					}
					if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_Lighting) ||
						HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_LightingLegacy))
					{
						SerializedProperty NormalMapPropertyName = serializedObject.FindProperty("m_NormalMapPropertyName");
						NormalMapPropertyName.stringValue = EditorGUILayout.TextField("Normal map (Texture2D): ", NormalMapPropertyName.stringValue);
						SerializedProperty RoughMetalMapPropertyName = serializedObject.FindProperty("m_RoughMetalMapPropertyName");
						RoughMetalMapPropertyName.stringValue = EditorGUILayout.TextField("Roughness / Metalness map (Texture2D): ", RoughMetalMapPropertyName.stringValue);
						SerializedProperty RoughnessPropertyName = serializedObject.FindProperty("m_RoughnessPropertyName");
						RoughnessPropertyName.stringValue = EditorGUILayout.TextField("Roughness (Float): ", RoughnessPropertyName.stringValue);
						SerializedProperty MetalnessPropertyName = serializedObject.FindProperty("m_MetalnessPropertyName");
						MetalnessPropertyName.stringValue = EditorGUILayout.TextField("Metalness (Float): ", MetalnessPropertyName.stringValue);
					}
					if (bindingHasMeshRenderer)
					{
						SerializedProperty MeshDiffuseColorPropertyName = serializedObject.FindProperty("m_MeshDiffuseColorPropertyName");
						MeshDiffuseColorPropertyName.stringValue = EditorGUILayout.TextField("Mesh diffuse color (Vec4): ", MeshDiffuseColorPropertyName.stringValue);

						SerializedProperty MeshEmissiveColorPropertyName = serializedObject.FindProperty("m_MeshEmissiveColorPropertyName");
						MeshEmissiveColorPropertyName.stringValue = EditorGUILayout.TextField("Mesh emissive color (Vec4): ", MeshEmissiveColorPropertyName.stringValue);

						SerializedProperty MeshAlphaCursorPropertyName = serializedObject.FindProperty("m_MeshAlphaCursorPropertyName");
						MeshAlphaCursorPropertyName.stringValue = EditorGUILayout.TextField("Mesh alpha cursor (Float): ", MeshAlphaCursorPropertyName.stringValue);

						if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_FluidVAT) ||
							HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_SoftVAT) ||
							HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_RigidVAT))
						{
							SerializedProperty MeshVATCursorPropertyName = serializedObject.FindProperty("m_MeshVATCursorPropertyName");
							MeshVATCursorPropertyName.stringValue = EditorGUILayout.TextField("Mesh VAT cursor (Float): ", MeshVATCursorPropertyName.stringValue);
							SerializedProperty VATPositionMapPropertyName = serializedObject.FindProperty("m_VATPositionMapPropertyName");
							VATPositionMapPropertyName.stringValue = EditorGUILayout.TextField("VAT positions (Texture2D): ", VATPositionMapPropertyName.stringValue);
							SerializedProperty VATNormalMapPropertyName = serializedObject.FindProperty("m_VATNormalMapPropertyName");
							VATNormalMapPropertyName.stringValue = EditorGUILayout.TextField("VAT normals (Texture2D): ", VATNormalMapPropertyName.stringValue);
							SerializedProperty VATColorMapPropertyName = serializedObject.FindProperty("m_VATColorMapPropertyName");
							VATColorMapPropertyName.stringValue = EditorGUILayout.TextField("VAT colors (Texture2D): ", VATColorMapPropertyName.stringValue);
							SerializedProperty VATRotationMapPropertyName = serializedObject.FindProperty("m_VATRotationMapPropertyName");
							VATRotationMapPropertyName.stringValue = EditorGUILayout.TextField("VAT rotations (Texture2D): ", VATRotationMapPropertyName.stringValue);
							SerializedProperty VATNumFramesPropertyName = serializedObject.FindProperty("m_VATNumFramesPropertyName");
							VATNumFramesPropertyName.stringValue = EditorGUILayout.TextField("VAT num frames (Int): ", VATNumFramesPropertyName.stringValue);
							SerializedProperty VATPackedDataPropertyName = serializedObject.FindProperty("m_VATPackedDataPropertyName");
							VATPackedDataPropertyName.stringValue = EditorGUILayout.TextField("VAT packed data (Int): ", VATPackedDataPropertyName.stringValue);
							SerializedProperty VATColorPropertyName = serializedObject.FindProperty("m_VATColorPropertyName");
							VATColorPropertyName.stringValue = EditorGUILayout.TextField("VAT color (Vec4): ", VATColorPropertyName.stringValue);
							SerializedProperty VATNormalizedDataPropertyName = serializedObject.FindProperty("m_VATNormalizedDataPropertyName");
							VATNormalizedDataPropertyName.stringValue = EditorGUILayout.TextField("VAT normalized data (Int 0 or 1): ", VATNormalizedDataPropertyName.stringValue);
							SerializedProperty VATBoundsPositionPropertyName = serializedObject.FindProperty("m_VATBoundsPositionPropertyName");
							VATBoundsPositionPropertyName.stringValue = EditorGUILayout.TextField("VAT bounds positions (Vec2): ", VATBoundsPositionPropertyName.stringValue);
							SerializedProperty VATPadToPowerOf2PropertyName = serializedObject.FindProperty("m_VATPadToPowerOf2PropertyName");
							VATPadToPowerOf2PropertyName.stringValue = EditorGUILayout.TextField("VAT pad to power of 2 (Int 0 or 1): ", VATPadToPowerOf2PropertyName.stringValue);
							SerializedProperty VATPaddedRatioPropertyName = serializedObject.FindProperty("m_VATPaddedRatioPropertyName");
							VATPaddedRatioPropertyName.stringValue = EditorGUILayout.TextField("VAT Padded ratio (Vec2): ", VATPaddedRatioPropertyName.stringValue);
							if (HasShaderVariationFlag(ShaderMask.intValue, EShaderVariationFlags.Has_RigidVAT))
							{
								SerializedProperty VATBoundsPivotPropertyName = serializedObject.FindProperty("m_VATBoundsPivotPropertyName");
								VATBoundsPivotPropertyName.stringValue = EditorGUILayout.TextField("VAT bounds pivot (Vec2): ", VATBoundsPivotPropertyName.stringValue);
							}
						}
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
