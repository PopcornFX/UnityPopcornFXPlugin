//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

namespace PopcornFX
{
	[CustomEditor(typeof(PKFxEffectAsset))]
	public class PKFxAssetEditor : Editor
	{
		private MaterialEditor	m_MaterialEditor = null;
		GUIStyle				m_BGColor = new GUIStyle();

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

		public override void OnInteractivePreviewGUI(Rect rect, GUIStyle background)
		{
			//Handles.DrawCamera(r, previewCamera_);
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

		public override void OnInspectorGUI()
		{
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
					PKFxManager.ReimportAssets(new List<string> { serializedObject.FindProperty("m_AssetVirtualPath").stringValue });
				}
			}
			EditorGUILayout.EndHorizontal();

			SerializedProperty useMeshRenderer = serializedObject.FindProperty("m_UsesMeshRenderer");

			EditorGUILayout.LabelField("Effect info : ");
			EditorGUI.indentLevel++;
			if (useMeshRenderer.boolValue)
				EditorGUILayout.LabelField("Uses at least one mesh renderer");
			else
				EditorGUILayout.LabelField("Does not use any mesh renderer");
			EditorGUI.indentLevel--;

			SerializedProperty rdrs = serializedObject.FindProperty("m_RendererDescs");
			SerializedProperty mats = serializedObject.FindProperty("m_Materials");
			SerializedProperty customMats = serializedObject.FindProperty("m_CustomMaterials");
			int rdrsListSize = rdrs.arraySize;
			if (rdrsListSize > 0)
				EditorGUILayout.LabelField("Renderers : ");

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

			GUIStyle		alernatingColors = new GUIStyle();
			Color			backgroundColor = EditorGUIUtility.isProSkin ? new Color32(56, 56, 56, 255) : new Color32(194, 194, 194, 255);

			Texture2D[]		colors = new Texture2D[] { MakeGUITexture(backgroundColor * 1.2f), MakeGUITexture(backgroundColor * 1.35f) };

			for (int i = 0; i < rdrsListSize; i++)
			{
				alernatingColors.normal.background = colors[i % 2];
				EditorGUILayout.BeginHorizontal(alernatingColors);
				SerializedProperty rdrDesc = rdrs.GetArrayElementAtIndex(i);
				SerializedProperty rdrType = rdrDesc.FindPropertyRelative("m_Type");

				object targetObject = rdrs.serializedObject.targetObject;
				System.Type targetObjectClassType = targetObject.GetType();
				var field = targetObjectClassType.GetField(rdrs.propertyPath);
				if (field != null)
				{
					SerializedProperty matProp = mats.GetArrayElementAtIndex(renderers[i].MaterialIdx);
					if (matProp.objectReferenceValue == null)
					{
						Material mat = PKFxSettings.MaterialFactory.EditorResolveMaterial(renderers[i], asset, false);
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
						m_MaterialEditor = (MaterialEditor)CreateEditor(asset.m_Materials[renderers[i].MaterialIdx]);
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
								asset.AddCustomMaterial(customMats, renderers[i], newMat, renderers[i].MaterialIdx);
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
								customMatInfo.BindMaterialProperties(renderers[i], customMatInfo.m_CustomMaterial, asset, true);
							}
						}
						EditorGUI.indentLevel--;
					}
					EditorGUILayout.EndVertical();
				}

				EditorGUILayout.EndHorizontal();
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
							int					customMatIdx = asset.ResetParticleMaterial(renderers[idxCpy]);
							SerializedProperty	matProp = mats.GetArrayElementAtIndex(customMatIdx);

							matProp.objectReferenceValue = asset.m_Materials[renderers[idxCpy].MaterialIdx];
							if (customMatIdx >= 0)
								customMats.DeleteArrayElementAtIndex(customMatIdx);
							serializedObject.ApplyModifiedProperties();
							Repaint();
						});
						menu.AddItem(new GUIContent("Create material Override"), false, () =>
						{
							Material matOverride = new Material(asset.m_Materials[renderers[idxCpy].MaterialIdx]);

							AssetDatabase.CreateAsset(matOverride, "Assets/Resources/" + Path.GetFileNameWithoutExtension(asset.name) + "_" + idxCpy + ".mat");
							SerializedProperty matProp = mats.GetArrayElementAtIndex(renderers[idxCpy].MaterialIdx);

							asset.AddCustomMaterial(customMats, renderers[idxCpy], matOverride, renderers[idxCpy].MaterialIdx);
							matProp.objectReferenceValue = matOverride;
							serializedObject.ApplyModifiedProperties();
							Repaint();
						});
						menu.ShowAsContext();

						current.Use();

					}
				}
			}
			EditorGUI.indentLevel--;
			using (new EditorGUI.DisabledScope(asset.m_CustomMaterials.Count == 0))
			{
				if (GUILayout.Button("Reset all customs materials"))
				{
					asset.ResetAllCustomMaterials(mats, customMats);
					serializedObject.ApplyModifiedProperties();
					Repaint();
				}
			}

			EditorGUI.indentLevel++;
			SerializedProperty events = serializedObject.FindProperty("m_EventDescs");
			int eventsListSize = events.arraySize;
			if (eventsListSize > 0)
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

			EditorGUILayout.LabelField("Dependencies : ");
			EditorGUI.indentLevel++;
			SerializedProperty deps = serializedObject.FindProperty("m_Dependencies");
			int depsListSize = deps.arraySize;

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

			EditorGUILayout.LabelField("Attributes : ");
			EditorGUI.indentLevel++;
			SerializedProperty attrs = serializedObject.FindProperty("m_AttributeDescs");
			int attrsListSize = attrs.arraySize;

			for (int i = 0; i < attrsListSize; i++)
			{
				SerializedProperty attrDesc = attrs.GetArrayElementAtIndex(i);
				SerializedProperty attrName = attrDesc.FindPropertyRelative("m_Name");
				SerializedProperty attrType = attrDesc.FindPropertyRelative("m_Type");
				SerializedProperty minMaxFlag = attrDesc.FindPropertyRelative("m_MinMaxFlag");
				EAttributeType baseType = (EAttributeType)attrType.intValue;

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
						minValDesc = FormatLimitValue(minVal, baseType);
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
						maxValDesc = FormatLimitValue(maxVal, baseType);
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
				defaultValStr = FormatLimitValue(defaultVectorValue, baseType);

				EditorGUI.indentLevel++;
				EditorGUILayout.LabelField(attrName.stringValue);
				EditorGUI.indentLevel++;
				EditorGUILayout.LabelField(attrType.enumNames[attrType.enumValueIndex]);
				if (showMinMax)
				{
					EditorGUILayout.LabelField("Min/Max: " + minValDesc + "-" + maxValDesc);
				}
				EditorGUILayout.LabelField("Default: " + defaultValStr);
				EditorGUI.indentLevel--;
				EditorGUI.indentLevel--;
			}
			EditorGUI.indentLevel--;

			EditorGUILayout.LabelField("Samplers : ");
			EditorGUI.indentLevel++;
			SerializedProperty smplrs = serializedObject.FindProperty("m_SamplerDescs");
			int smplrsListSize = smplrs.arraySize;

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

		//----------------------------------------------------------------------------

		private string FormatLimitValue(Vector4 value, EAttributeType baseType)
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
				retStr = "[" + value.x + ", " + value.y + ", " + value.z + "]";
			}
			else if (baseType == EAttributeType.Float4)
			{
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