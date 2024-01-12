//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.Linq;

namespace PopcornFX
{
	[CustomEditor(typeof(PKFxEffectAsset))]
	public class PKFxAssetEditor : Editor
	{
		private MaterialEditor	m_MaterialEditor = null;
		GUIStyle				m_BGColor = new GUIStyle();

		static bool[]			m_ShowQualities = null;

		private double			m_LastFrameEditorTime;
		private float			m_CurrentTime;
		private Material		m_AnimatedThumbnailMaterial;
		static readonly int		s_ShaderColorMask = Shader.PropertyToID("_ColorMaskBits");
		static readonly int		s_ShaderSliceIndex = Shader.PropertyToID("_SliceIndex");
		static readonly int		s_ShaderToSrgb = Shader.PropertyToID("_ToSRGB");
		static readonly int		s_ShaderIsNormalMap = Shader.PropertyToID("_IsNormalMap");

		//----------------------------------------------------------------------------

		public static int AttrBoundsGetIntValueSerialized(SerializedProperty bound, string variable)
		{
			return bound.FindPropertyRelative(variable).FindPropertyRelative("i1").intValue;
		}

		//----------------------------------------------------------------------------

		public static float AttrBoundsGetFloatValueSerialized(SerializedProperty bound, string variable)
		{
			return bound.FindPropertyRelative(variable).FindPropertyRelative("f1").floatValue;
		}

		//----------------------------------------------------------------------------

		private Texture2D MakeGUITexture(Color color)
		{
			Color[] pix = new Color[1];

			pix[0] = color;

			Texture2D result = new Texture2D(1, 1);
			result.SetPixels(pix);
			result.Apply();

			return result;
		}

		//----------------------------------------------------------------------------

		public override Texture2D RenderStaticPreview(string assetPath, UnityEngine.Object[] subAssets, int width, int height)
		{
			PKFxEffectAsset asset = target as PKFxEffectAsset;

			if (asset == null || asset.m_EditorThumbnail == null)
				return null;

			// asset.m_EditorThumbnail must be a supported format: ARGB32, RGBA32, RGB24,
			// Alpha8 or one of float formats
			Texture2D tex = new Texture2D(width, height);
			EditorUtility.CopySerialized(asset.m_EditorThumbnail, tex);

			return tex;
		}

		//----------------------------------------------------------------------------

		public override void OnPreviewGUI(Rect r, GUIStyle background)
		{
			Event evt = Event.current;

			if (evt.type != EventType.Repaint)
				return;

			PKFxEffectAsset asset = target as PKFxEffectAsset;

			if (asset == null)
				return;

			if (asset.m_EditorAnimatedThumbnail != null)
			{
				if (m_AnimatedThumbnailMaterial == null)
					m_AnimatedThumbnailMaterial = (Material)EditorGUIUtility.LoadRequired("Previews/Preview2DTextureArrayMaterial.mat");

				double timeSinceStartup = EditorApplication.timeSinceStartup;
				float deltaTime = (float)(timeSinceStartup - m_LastFrameEditorTime);
				m_LastFrameEditorTime = timeSinceStartup;
				m_CurrentTime = Mathf.Repeat(m_CurrentTime + deltaTime, 4.0f); // 4sec
				int slice = Mathf.FloorToInt(m_CurrentTime * 12.0f); //12 fps

				m_AnimatedThumbnailMaterial.mainTexture = asset.m_EditorAnimatedThumbnail;
				m_AnimatedThumbnailMaterial.SetFloat(s_ShaderSliceIndex, (float)slice);
				m_AnimatedThumbnailMaterial.SetFloat(s_ShaderToSrgb, QualitySettings.activeColorSpace == ColorSpace.Linear ? 1.0f : 0.0f);
				m_AnimatedThumbnailMaterial.SetFloat(s_ShaderIsNormalMap, 0.0f);
				m_AnimatedThumbnailMaterial.SetFloat(s_ShaderColorMask, (float)7);

				EditorGUI.DrawPreviewTexture(r, asset.m_EditorAnimatedThumbnail, m_AnimatedThumbnailMaterial, ScaleMode.ScaleToFit, 0, 0, UnityEngine.Rendering.ColorWriteMask.All);
			}
			else if (asset.m_EditorThumbnail != null)
			{
				EditorGUI.DrawPreviewTexture(r, asset.m_EditorThumbnail, null, ScaleMode.ScaleToFit, 0, 0, UnityEngine.Rendering.ColorWriteMask.All);
			}
		}

		//----------------------------------------------------------------------------

		public override bool HasPreviewGUI()
		{
			PKFxEffectAsset asset = target as PKFxEffectAsset;
			return asset != null && (asset.m_EditorAnimatedThumbnail != null || asset.m_EditorThumbnail != null);
		}

		//----------------------------------------------------------------------------

		public override bool RequiresConstantRepaint()
		{
			PKFxEffectAsset asset = target as PKFxEffectAsset;
			return asset != null && (asset.m_EditorAnimatedThumbnail != null);
		}

		//----------------------------------------------------------------------------

		[ExecuteInEditMode]
		public void OnEnable()
		{
			if (PKFxManager.QualitiesLevelDescription != null && (m_ShowQualities == null || m_ShowQualities.Length != PKFxManager.QualitiesLevelDescription.Length))
			{
				m_ShowQualities = new bool[PKFxManager.QualitiesLevelDescription.Length];
				m_ShowQualities[Array.FindIndex(PKFxManager.QualitiesLevelDescription, ele => ele == PKFxManager.StoredQualityLevel)] = true;
			}
		}

		public override void OnInspectorGUI()
		{
			if (PKFxManager.QualitiesLevelDescription != null && (m_ShowQualities == null || m_ShowQualities.Length != PKFxManager.QualitiesLevelDescription.Length))
			{
				m_ShowQualities = new bool[PKFxManager.QualitiesLevelDescription.Length];
				m_ShowQualities[Array.FindIndex(PKFxManager.QualitiesLevelDescription, ele => ele == PKFxManager.StoredQualityLevel)] = true;
			}
			m_BGColor.normal.background = EditorGUIUtility.whiteTexture;
			EditorStyles.textField.wordWrap = true;

			PKFxEffectAsset asset = target as PKFxEffectAsset;
			List<SBatchDesc> renderers = asset.m_RendererDescs;

			EditorGUILayout.BeginHorizontal();
			EditorGUILayout.LabelField(serializedObject.FindProperty("m_AssetFullPath").stringValue);
			using (new EditorGUI.DisabledScope(Application.isPlaying))
			{
				if (GUILayout.Button("Reimport"))
				{
					PKFxManager.ReimportAssets(new List<string> { serializedObject.FindProperty("m_AssetVirtualPath").stringValue }, PKFxSettings.CurrentPlatformName);
				}
			}
			EditorGUILayout.EndHorizontal();

			SerializedProperty rdrs = serializedObject.FindProperty("m_RendererDescs");
			SerializedProperty mats = serializedObject.FindProperty("m_Materials");
			SerializedProperty customMats = serializedObject.FindProperty("m_CustomMaterials");

			int rdrsListSize = rdrs.arraySize;

			// Upgrade if material wasn't set in import (2.12.7 ++)
			if (mats.arraySize < rdrsListSize)
			{
				for (int i = 0; i < rdrsListSize; i++)
				{
					mats.InsertArrayElementAtIndex(i);
					Material mat = PKFxSettings.MaterialFactory.EditorResolveMaterial(renderers[i], asset);
					SerializedProperty newMatProp = mats.GetArrayElementAtIndex(i);
					newMatProp.objectReferenceValue = mat;
				}
				serializedObject.ApplyModifiedProperties();
			}

			bool reimport = false;

			if (rdrsListSize > 0)
			{
				EditorGUILayout.BeginHorizontal();
				EditorGUILayout.LabelField("Renderers : ");
				using (new EditorGUI.DisabledScope(Application.isPlaying))
				{
					if (GUILayout.Button("Reimport Materials"))
						reimport = true;
				}
				EditorGUILayout.EndHorizontal();
			}

			GUIStyle alernatingColors = new GUIStyle();
			Color backgroundColor = EditorGUIUtility.isProSkin ? new Color32(56, 56, 56, 255) : new Color32(194, 194, 194, 255);

			Texture2D[] colors = new Texture2D[] { MakeGUITexture(backgroundColor * 1.2f), MakeGUITexture(backgroundColor * 1.35f) };

			//
			int[]	usedRenderers = new int[rdrsListSize];
			for (int q = 0; q < PKFxManager.QualitiesLevelDescription.Length; ++q) // Quality level counts
			{
				m_ShowQualities[q] = EditorGUILayout.Foldout(m_ShowQualities[q], PKFxManager.QualitiesLevelDescription[q]);

				int qualityRendererCount = 0;
				for (int i = 0; i < rdrsListSize; i++)
				{
					alernatingColors.normal.background = colors[i % 2];
					SerializedProperty rdrDesc = rdrs.GetArrayElementAtIndex(i);
					SerializedProperty rdrType = rdrDesc.FindPropertyRelative("m_Type");

					object targetObject = rdrs.serializedObject.targetObject;
					System.Type targetObjectClassType = targetObject.GetType();
					var field = targetObjectClassType.GetField(rdrs.propertyPath);
					if (field != null)
					{
						PKFxEffectAsset.MaterialUIDToIndex index = asset.m_MaterialIndexes.Find(item => item.m_UID == renderers[i].m_UID && item.m_Quality == PKFxManager.QualitiesLevelDescription[q]);
						if (index != null && index.m_Idx < mats.arraySize)
						{
							++usedRenderers[i]; //Flag renderers used in at least one quality level
							++qualityRendererCount;
							if (m_ShowQualities[q])
							{
								EditorGUILayout.BeginHorizontal(alernatingColors);
								if (renderers[i].m_Type == ERendererType.Light || renderers[i].m_Type == ERendererType.Sound)
								{
									EditorGUILayout.BeginVertical();
									if (rdrType.intValue == (int)ERendererType.Light)
										EditorGUILayout.LabelField("Light Renderer");
									else if (rdrType.intValue == (int)ERendererType.Sound)
										EditorGUILayout.LabelField("Sound Renderer");
									EditorGUILayout.EndVertical();
								}
								else
								{
									SerializedProperty matProp = mats.GetArrayElementAtIndex(index.m_Idx);
									if (reimport || matProp.objectReferenceValue == null)
									{
										Material mat = PKFxSettings.MaterialFactory.EditorResolveMaterial(renderers[i], asset, reimport, true, false);
										matProp.objectReferenceValue = mat;
										serializedObject.ApplyModifiedProperties();
									}
									Material matForName = matProp.objectReferenceValue as Material;

									EditorGUILayout.BeginVertical(GUILayout.MaxWidth(64));
									{
										if (rdrType.intValue == (int)ERendererType.Billboard)
											EditorGUILayout.LabelField("Billboards", GUILayout.Width(64));
										else if (rdrType.intValue == (int)ERendererType.Ribbon)
											EditorGUILayout.LabelField("Ribbons", GUILayout.Width(64));
										else if (rdrType.intValue == (int)ERendererType.Mesh)
											EditorGUILayout.LabelField("Meshs", GUILayout.Width(64));
										else if (rdrType.intValue == (int)ERendererType.Triangle)
											EditorGUILayout.LabelField("Triangles", GUILayout.Width(64));

										if (m_MaterialEditor != null)
											DestroyImmediate(m_MaterialEditor);
										m_MaterialEditor = (MaterialEditor)CreateEditor(asset.m_Materials[index.m_Idx]);
										if (m_MaterialEditor != null)
											m_MaterialEditor.OnInteractivePreviewGUI(GUILayoutUtility.GetRect(64, 64), m_BGColor);

									}
									EditorGUILayout.EndVertical();

									EditorGUILayout.BeginVertical();
									{
										{
											string generatedName = rdrDesc.FindPropertyRelative("m_GeneratedName").stringValue;
											EditorGUILayout.LabelField("Name: " + generatedName);

											if (matForName != null && matForName.shader != null)
												EditorGUILayout.LabelField(matForName.shader.name);
											Material newMat = EditorGUILayout.ObjectField(matProp.objectReferenceValue, typeof(Material), false) as Material;

											if (matProp.objectReferenceValue != newMat)
											{
												asset.AddCustomMaterial(customMats, renderers[i], newMat, index.m_UID);
												matProp.objectReferenceValue = newMat;
												serializedObject.ApplyModifiedProperties();
											}
										}
										EditorGUI.indentLevel++;
										PKFxCustomMaterialInfo customMatInfo = asset.FindCustomMaterialInfo(renderers[i]);
										if (customMatInfo != null)
										{
											if (customMatInfo.DrawEditorShaderInputBindings(renderers[i])) // Change in the bindings, update materials
											{
												customMatInfo.SetMaterialKeywords(renderers[i], customMatInfo.m_CustomMaterial);
												customMatInfo.BindMaterialProperties(renderers[i], customMatInfo.m_CustomMaterial, asset);
											}
										}
										EditorGUI.indentLevel--;

									}
									EditorGUILayout.EndVertical();
								}
								EditorGUILayout.EndHorizontal();
							}
						}
					}
					//-Event
					Rect clickArea = GUILayoutUtility.GetLastRect();
					Event current = Event.current;
					int idxCpy = i;
					if (rdrType.intValue != (int)ERendererType.Ribbon)
					{
						if (clickArea.Contains(current.mousePosition) && current.type == EventType.ContextClick)
						{
							GenericMenu menu = new GenericMenu();

							menu.AddItem(new GUIContent("Reset"), false, () =>
							{
								int customMatIdx = asset.ResetParticleMaterial(renderers[idxCpy]);
								if (customMatIdx != -1)
								{
									SerializedProperty matProp = mats.GetArrayElementAtIndex(customMatIdx);
									PKFxEffectAsset.MaterialUIDToIndex index = asset.m_MaterialIndexes.Find(item => item.m_UID == renderers[idxCpy].m_UID && item.m_Quality == PKFxManager.StoredQualityLevel);
									matProp.objectReferenceValue = asset.m_Materials[index.m_Idx];
									if (customMatIdx >= 0)
										customMats.DeleteArrayElementAtIndex(customMatIdx);
									serializedObject.ApplyModifiedProperties();
									Repaint();
								}
							});
							menu.AddItem(new GUIContent("Create material Override"), false, () =>
							{
								PKFxEffectAsset.MaterialUIDToIndex index = asset.m_MaterialIndexes.Find(item => item.m_UID == renderers[idxCpy].m_UID && item.m_Quality == PKFxManager.StoredQualityLevel);
								Material matOverride = new Material(asset.m_Materials[index.m_Idx]);

								AssetDatabase.CreateAsset(matOverride, "Assets/Resources/" + Path.GetFileNameWithoutExtension(asset.name) + "_" + idxCpy + ".mat");
								SerializedProperty matProp = mats.GetArrayElementAtIndex(index.m_Idx);

								asset.AddCustomMaterial(customMats, renderers[idxCpy], matOverride, index.m_UID);
								matProp.objectReferenceValue = matOverride;
								serializedObject.ApplyModifiedProperties();
								Repaint();
							});
							menu.ShowAsContext();

							current.Use();

						}
					}
				}
				if (qualityRendererCount == 0)
					EditorGUILayout.LabelField("No renderer for this Quality level.");

			}

			//Show renderers not presents in quality levels
			bool showUnusedRendererWarning = false;
			for (int i = 0; i < rdrsListSize; i++)
			{
				if (usedRenderers[i] == 0)
					showUnusedRendererWarning = true;
			}
			if (showUnusedRendererWarning)
			{
				GUIStyle	warnStyle = new GUIStyle();
				Texture2D	colorWarn = MakeGUITexture(new Color32(200, 56, 56, 255));

				warnStyle.normal.background = colorWarn;
				warnStyle.alignment = TextAnchor.MiddleCenter;
				EditorGUILayout.LabelField("Warning: Asset contains at least one renderer defined for an absent quality settings.", warnStyle);
			}
				
			EditorGUI.indentLevel--;

			UnityEngine.Object[] data = AssetDatabase.LoadAllAssetsAtPath("Assets/" + PKFxSettings.UnityPackFxPath + "/" + asset.AssetVirtualPath + ".asset");
			using (new EditorGUI.DisabledScope(asset.m_CustomMaterials.Count == 0 && data.Length > 1))
			{
				if (GUILayout.Button("Reset all customs materials"))
				{
					asset.ResetAllCustomMaterials(mats, customMats);
					serializedObject.ApplyModifiedProperties();
					Repaint();
				}
			}

			//Effect Dependencies
			SerializedProperty deps = serializedObject.FindProperty("m_Dependencies");
			int depsListSize = deps.arraySize;
			if (depsListSize > 0)
			{
				EditorGUILayout.LabelField("Dependencies : ");
				EditorGUI.indentLevel++;

				for (int i = 0; i < depsListSize; i++)
				{
					SerializedProperty depDesc = deps.GetArrayElementAtIndex(i);
					SerializedProperty depPath = depDesc.FindPropertyRelative("m_Path");
					SerializedProperty depUsageProp = depDesc.FindPropertyRelative("m_UsageFlags");
					int depUsage = depUsageProp.intValue;
					SerializedProperty depObject = depDesc.FindPropertyRelative("m_Object");

					EditorGUILayout.LabelField(depPath.stringValue);
					EditorGUI.indentLevel++;

					if ((depUsage & (int)EUseInfoFlag.IsLinearTextureRenderer) != 0)
						EditorGUILayout.LabelField("isLinearTexture");
					if ((depUsage & (int)EUseInfoFlag.IsTextureRenderer) != 0)
						EditorGUILayout.LabelField("IsTextureRenderer");
					if ((depUsage & (int)EUseInfoFlag.IsMeshRenderer) != 0)
						EditorGUILayout.LabelField("IsMeshRenderer");
					if ((depUsage & (int)EUseInfoFlag.IsMeshSampler) != 0)
						EditorGUILayout.LabelField("IsMeshSampler");
					if ((depUsage & (int)EUseInfoFlag.IsTextureSampler) != 0)
						EditorGUILayout.LabelField("IsTextureSampler");

					using (new EditorGUI.DisabledScope(true))
						EditorGUILayout.ObjectField(depObject);
					EditorGUI.indentLevel--;
				}
				EditorGUI.indentLevel--;
			}

			//Effect Attributes
			SerializedProperty attrs = serializedObject.FindProperty("m_AttributeDescs");
			int attrsListSize = attrs.arraySize;
			if (attrsListSize > 0)
			{
				EditorGUILayout.LabelField("Attributes : ");
				EditorGUI.indentLevel++;

				for (int i = 0; i < attrsListSize; i++)
				{
					SerializedProperty attrDesc = attrs.GetArrayElementAtIndex(i);
					SerializedProperty attrName = attrDesc.FindPropertyRelative("m_Name");
					SerializedProperty attrType = attrDesc.FindPropertyRelative("m_Type");
					SerializedProperty attrSemantic = attrDesc.FindPropertyRelative("m_Semantic");
					SerializedProperty minMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
					EAttributeType baseType = (EAttributeType)attrType.intValue;
					EAttributeSemantic semantic = (EAttributeSemantic)attrSemantic.intValue;

					bool showMinMax = false;
					string minValDesc = "";
					string maxValDesc = "";
					string defaultValStr = "";

					if (baseType == EAttributeType.Bool ||
						baseType == EAttributeType.Bool2 ||
						baseType == EAttributeType.Bool3 ||
						baseType == EAttributeType.Bool4)
					{
						showMinMax = false;
					}
					else
					{
						showMinMax = true;
						if ((minMaxFlag.intValue & (int)PKFxEffectAsset.AttributeDesc.EAttrDescFlag.HasMin) != 0)
						{
							SerializedProperty minValProp = attrDesc.FindPropertyRelative("m_MinValue");
							Vector4 minVal = new Vector4(minValProp.FindPropertyRelative("x").FindPropertyRelative("f1").floatValue,
														 minValProp.FindPropertyRelative("y").FindPropertyRelative("f1").floatValue,
														 minValProp.FindPropertyRelative("z").FindPropertyRelative("f1").floatValue,
														 minValProp.FindPropertyRelative("w").FindPropertyRelative("f1").floatValue);
							minValDesc = FormatLimitValue(minVal, baseType, EAttributeSemantic.None);
						}
						else
						{
							minValDesc = "[-infinity]";
						}
						if ((minMaxFlag.intValue & (int)PKFxEffectAsset.AttributeDesc.EAttrDescFlag.HasMax) != 0)
						{
							SerializedProperty maxValProp = attrDesc.FindPropertyRelative("m_MaxValue");
							Vector4 maxVal = new Vector4(maxValProp.FindPropertyRelative("x").FindPropertyRelative("f1").floatValue,
														 maxValProp.FindPropertyRelative("y").FindPropertyRelative("f1").floatValue,
														 maxValProp.FindPropertyRelative("z").FindPropertyRelative("f1").floatValue,
														 maxValProp.FindPropertyRelative("w").FindPropertyRelative("f1").floatValue);
							maxValDesc = FormatLimitValue(maxVal, baseType, EAttributeSemantic.None);
						}
						else
						{
							maxValDesc = "[+infinity]";
						}
					}

					SerializedProperty defaultValue = attrDesc.FindPropertyRelative("m_DefaultValue");
					Vector4 defaultVectorValue = new Vector4(defaultValue.FindPropertyRelative("x").FindPropertyRelative("f1").floatValue,
															 defaultValue.FindPropertyRelative("y").FindPropertyRelative("f1").floatValue,
															 defaultValue.FindPropertyRelative("z").FindPropertyRelative("f1").floatValue,
															 defaultValue.FindPropertyRelative("w").FindPropertyRelative("f1").floatValue);
					defaultValStr = FormatLimitValue(defaultVectorValue, baseType, semantic);

					EditorGUI.indentLevel++;
					EditorGUILayout.LabelField(attrName.stringValue);
					EditorGUI.indentLevel++;
					EditorGUILayout.LabelField(attrType.enumNames[attrType.enumValueIndex]);
					if (showMinMax)
					{
						EditorGUILayout.LabelField("Min/Max: " + minValDesc + "-" + maxValDesc);
					}
					EditorGUILayout.LabelField("Default: " + defaultValStr);
					if (semantic != EAttributeSemantic.None)
					{
						EditorGUILayout.LabelField("Semantic: " + semantic.ToString());
					}
					EditorGUI.indentLevel--;
					EditorGUI.indentLevel--;
				}
				EditorGUI.indentLevel--;
			}

			//Effect Samplers
			SerializedProperty smplrs = serializedObject.FindProperty("m_SamplerDescs");
			int smplrsListSize = smplrs.arraySize;
			if (smplrs.arraySize > 0)
			{
				EditorGUILayout.LabelField("Samplers : ");
				EditorGUI.indentLevel++;

				for (int i = 0; i < smplrsListSize; i++)
				{
					SerializedProperty smplrsDesc = smplrs.GetArrayElementAtIndex(i);
					SerializedProperty smplrsName = smplrsDesc.FindPropertyRelative("m_Name");
					SerializedProperty smplrsType = smplrsDesc.FindPropertyRelative("m_Type");
					SerializedProperty usageFlags = smplrsDesc.FindPropertyRelative("m_UsageFlags");

					EditorGUI.indentLevel++;
					EditorGUILayout.LabelField(smplrsType.enumNames[smplrsType.enumValueIndex] + " " + smplrsName.stringValue);
					EditorGUILayout.LabelField("Usage: " + SamplerDesc.UsageFlagsToString(usageFlags.intValue));
					EditorGUI.indentLevel--;
				}
				EditorGUI.indentLevel--;
			}

			//Effect Broadcasted Event
			SerializedProperty events = serializedObject.FindProperty("m_EventDescs");
			int eventsListSize = events.arraySize;
			if (eventsListSize > 0)
			{
				EditorGUI.indentLevel++;
				EditorGUILayout.LabelField("Broadcasted events : ");
				for (int i = 0; i < eventsListSize; i++)
				{
					SerializedProperty eventDesc = events.GetArrayElementAtIndex(i);

					string name = eventDesc.FindPropertyRelative("m_Name").stringValue;

					int slot = eventDesc.FindPropertyRelative("m_Slot").intValue;

					EditorGUI.indentLevel++;
					EditorGUILayout.BeginHorizontal();
					EditorGUILayout.LabelField("Event Name: " + name);
					EditorGUILayout.Space();
					EditorGUILayout.LabelField("Slot: " + slot);
					EditorGUILayout.EndHorizontal();
					EditorGUI.indentLevel--;
				}
				EditorGUI.indentLevel--;
			}


			//Effect Info
			{
				SerializedProperty useMeshRenderer = serializedObject.FindProperty("m_UsesMeshRenderer");

				EditorGUILayout.LabelField("Effect info : ");
				EditorGUI.indentLevel++;

				int meshCount = asset.m_RendererDescs.Where(b => b != null && b.m_Type == ERendererType.Mesh).Count();
				EditorGUILayout.LabelField(" " + meshCount + " mesh renderer(s)");
				EditorGUILayout.LabelField(" " + (asset.m_RendererDescs.Count - meshCount) + " billboard/ribbon/triangle renderer(s)");
				EditorGUILayout.LabelField(" " + depsListSize + " Asset dependencies");
				EditorGUILayout.LabelField(" " + attrsListSize + " Attribute(s)");
				EditorGUILayout.LabelField(" " + smplrsListSize + " Attribute(s) sampler(s)");
				EditorGUILayout.LabelField(" " + eventsListSize + " Broadcasted event(s)");

				EditorGUI.indentLevel--;
			}
		}

		//----------------------------------------------------------------------------

		private string FormatLimitValue(Vector4 value, EAttributeType baseType, EAttributeSemantic semantic)
		{
			string retStr = "";

			byte[] floatBytes = System.BitConverter.GetBytes(value.x);

			if (baseType == EAttributeType.Bool)
			{
				retStr = "[" + (floatBytes[0] == 0 ? "false" : "true") + "]";
			}
			else if (baseType == EAttributeType.Bool2)
			{
				retStr = "[" + (floatBytes[0] == 0 ? "false" : "true") + ", " +
								(floatBytes[1] == 0 ? "false" : "true") + "]";
			}
			else if (baseType == EAttributeType.Bool3)
			{
				retStr = "[" + (floatBytes[0] == 0 ? "false" : "true") + ", " +
								(floatBytes[1] == 0 ? "false" : "true") + ", " +
								(floatBytes[2] == 0 ? "false" : "true") + "]";
			}
			else if (baseType == EAttributeType.Bool4)
			{
				retStr = "[" + (floatBytes[0] == 0 ? "false" : "true") + ", " +
								(floatBytes[1] == 0 ? "false" : "true") + ", " +
								(floatBytes[2] == 0 ? "false" : "true") + ", " +
								(floatBytes[3] == 0 ? "false" : "true") + "]";
			}
			else if (baseType == EAttributeType.Float)
			{
				retStr = "[" + value.x + "]";
			}
			else if (baseType == EAttributeType.Float2)
			{
				retStr = "[" + value.x + ", " + value.y + "]";
			}
			else if (baseType == EAttributeType.Float3)
			{
				if (semantic == EAttributeSemantic.Color)
					retStr = "[" + PKFxUtils.Linear2sRGB(value.x) + ", " + PKFxUtils.Linear2sRGB(value.y) + ", " + PKFxUtils.Linear2sRGB(value.z) + "]";
				else
					retStr = "[" + value.x + ", " + value.y + ", " + value.z + "]";
			}
			else if (baseType == EAttributeType.Float4)
			{
				if (semantic == EAttributeSemantic.Color)
					retStr = "[" + PKFxUtils.Linear2sRGB(value.x) + ", " + PKFxUtils.Linear2sRGB(value.y) + ", " + PKFxUtils.Linear2sRGB(value.z) + ", " + value.w + "]";
				else
					retStr = "[" + value.x + ", " + value.y + ", " + value.z + ", " + value.w + "]";
			}
			else if (baseType == EAttributeType.Int)
			{
				retStr = "[" + PKFxUtils.Float2Int(value.x) + "]";
			}
			else if (baseType == EAttributeType.Int2)
			{
				retStr = "[" + PKFxUtils.Float2Int(value.x) + ", " + PKFxUtils.Float2Int(value.y) + "]";
			}
			else if (baseType == EAttributeType.Int3)
			{
				retStr = "[" + PKFxUtils.Float2Int(value.x) + ", " + PKFxUtils.Float2Int(value.y) + ", " + PKFxUtils.Float2Int(value.z) + "]";
			}
			else if (baseType == EAttributeType.Int4)
			{
				retStr = "[" + PKFxUtils.Float2Int(value.x) + ", " + PKFxUtils.Float2Int(value.y) + ", " + PKFxUtils.Float2Int(value.z) + ", " + PKFxUtils.Float2Int(value.w) + "]";
			}
			return retStr;
		}
	}
}