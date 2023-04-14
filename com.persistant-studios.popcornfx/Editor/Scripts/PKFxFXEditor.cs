//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEditor;
using UnityEngine;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using System.Linq;

namespace PopcornFX
{
	[CustomEditor(typeof(PKFxEmitter))]
	[CanEditMultipleObjects]
	public class PKFxFXEditor : Editor
	{
		const int m_PlaybackControlsMaxHeight = 20;
		const int m_PlaybackControlsMaxWidth = 75;

		GUIStyle m_oddStyle = new GUIStyle();
		GUIStyle m_pairStyle = new GUIStyle();

		SerializedProperty m_FxName;
		SerializedProperty m_FxAsset;

		SerializedProperty m_FxAttributesStartValues;
		SerializedProperty m_FxAttributesDesc;

		SerializedProperty m_FxSamplers;
		SerializedProperty m_PlayOnStart;
		SerializedProperty m_TriggerAndForget;
		SerializedProperty m_KillEffectOnDestroy;

		SerializedProperty m_PreWarm;
		SerializedProperty m_Events;

		Texture2D m_CtrlPlayIcon;
		Texture2D m_CtrlStopIcon;
		Texture2D m_CtrlRestartIcon;
		Texture2D m_CtrlKillIcon;

		bool m_RequiresApplyModifiedProperties = false;

		//----------------------------------------------------------------------------

		void OnEnable()
		{
			m_FxName = serializedObject.FindProperty("m_FxName");
			m_FxAsset = serializedObject.FindProperty("m_FxAsset");
			m_FxSamplers = serializedObject.FindProperty("m_FxSamplersList");
			m_PlayOnStart = serializedObject.FindProperty("m_PlayOnStart");
			m_TriggerAndForget = serializedObject.FindProperty("m_TriggerAndForget");
			m_KillEffectOnDestroy = serializedObject.FindProperty("m_KillEffectOnDestroy");
			m_FxAttributesStartValues = serializedObject.FindProperty("m_FxAttributesStartValues");
			m_FxAttributesDesc = serializedObject.FindProperty("m_FxAttributesDesc");
			m_PreWarm = serializedObject.FindProperty("m_PreWarm");
			m_Events = serializedObject.FindProperty("m_Events");

			m_CtrlPlayIcon = Resources.Load<Texture2D>("Icons/CtrlPlay");
			m_CtrlStopIcon = Resources.Load<Texture2D>("Icons/CtrlStop");
			m_CtrlRestartIcon = Resources.Load<Texture2D>("Icons/CtrlRestart");
			m_CtrlKillIcon = Resources.Load<Texture2D>("Icons/CtrlKill");

			Color pairLineBackgroundColor = PKFxEditorTools.EditorBoxBackgroundColor;
			m_pairStyle.normal.background = MakeTex(1, 1, pairLineBackgroundColor);
		}

		private void ForceSerializedObjectUpdate(PKFxEmitter fx)
		{
			SerializedObject updatedObject = new SerializedObject(fx);

			serializedObject.CopyFromSerializedProperty(updatedObject.FindProperty("m_FxAsset"));
			serializedObject.CopyFromSerializedProperty(updatedObject.FindProperty("m_FxName"));
			serializedObject.CopyFromSerializedProperty(updatedObject.FindProperty("m_FxSamplersList"));
			serializedObject.CopyFromSerializedProperty(updatedObject.FindProperty("m_FxAttributesStartValues"));
			serializedObject.CopyFromSerializedProperty(updatedObject.FindProperty("m_FxAttributesDesc"));
			serializedObject.CopyFromSerializedProperty(updatedObject.FindProperty("m_FxAttributesDescHash"));
			serializedObject.CopyFromSerializedProperty(updatedObject.FindProperty("m_FxSamplersDescHash"));
			serializedObject.CopyFromSerializedProperty(updatedObject.FindProperty("m_PreWarm"));
			serializedObject.CopyFromSerializedProperty(updatedObject.FindProperty("m_Events"));
			m_RequiresApplyModifiedProperties = true;
		}

		//----------------------------------------------------------------------------

		public void OnSceneGUI()
		{
			if (target != null)
			{
				PKFxEmitter fx = target as PKFxEmitter;
				if (fx != null && fx.m_FxSamplersList != null)
				{
					for (int i = 0; i < fx.m_FxSamplersList.Count; i++)
					{
						Sampler sampler = fx.m_FxSamplersList[i];
						if (sampler != null && sampler.m_Descriptor.m_Type == ESamplerType.SamplerShape)
						{
							// Show the shapes in world space instead of local space (just for the editor workflow, does not change the actual sampling)
							bool worldSpaceSampling = sampler.m_WorldSpaceSampling;

							if (sampler.m_ShapeType == Sampler.EShapeType.BoxShape)
								DrawCube(i, sampler, worldSpaceSampling);
							else if (sampler.m_ShapeType == Sampler.EShapeType.SphereShape)
								DrawSphere(i, sampler, worldSpaceSampling);
							else if (sampler.m_ShapeType == Sampler.EShapeType.CylinderShape)
								DrawCylinder(i, sampler, worldSpaceSampling);
							else if (sampler.m_ShapeType == Sampler.EShapeType.CapsuleShape)
								DrawCapsule(i, sampler, worldSpaceSampling);
						}
					}
				}
			}
		}

		public override void OnInteractivePreviewGUI(Rect r, GUIStyle background)
		{
			base.OnInteractivePreviewGUI(r, background);
		}

		//----------------------------------------------------------------------------

		public override void OnInspectorGUI()
		{
			if (serializedObject.targetObjects.Length == 1)
			{
				using (var changeCheck = new EditorGUI.ChangeCheckScope())
				{
					EditorGUILayout.PropertyField(m_FxAsset, new GUIContent("Effect Asset"));
					if (changeCheck.changed)
					{
						serializedObject.ApplyModifiedProperties();
						serializedObject.Update();

						PKFxEmitter fx = target as PKFxEmitter;
						if (EditorUpdatePkFX(fx, false))
						{
							ForceSerializedObjectUpdate(fx);
						}
					}
				}
			}

			bool isFxAssetSet = (m_FxAsset.objectReferenceValue != null);

			if (isFxAssetSet)
				DrawPlaybackControls();

			DrawEmitterProperties();

			if (serializedObject.targetObjects.Length == 1)
			{
				if (isFxAssetSet)
				{
					DrawAttributes();
					DrawEvents();
				}

			}

			if (m_RequiresApplyModifiedProperties)
			{
				serializedObject.ApplyModifiedProperties();
				m_RequiresApplyModifiedProperties = false;
			}
		}

		private void DrawEmitterProperties()
		{
			using (var category = new PKFxEditorCategory(m_PlayOnStart, "Emitter"))
			{
				if (category.IsExpanded())
				{
					EditorGUI.BeginChangeCheck();
					{
						EditorGUILayout.PropertyField(m_PlayOnStart);
						EditorGUILayout.PropertyField(m_TriggerAndForget);
						EditorGUILayout.PropertyField(m_KillEffectOnDestroy);
						EditorGUILayout.PropertyField(m_PreWarm);
					}
					if (EditorGUI.EndChangeCheck())
					{
						m_RequiresApplyModifiedProperties = true;
					}
				}
			}
		}

		private void DrawEvents()
		{
			if (m_Events.arraySize == 0)
				return;

			using (var category = new PKFxEditorCategory(DrawEventsHeader))
			{
				if (!category.IsExpanded())
					return;
				EditorGUI.BeginChangeCheck();
				{
					if (serializedObject.targetObjects.Length == 1)
					{
						for (int i = 0; i < m_Events.arraySize; i++)
						{
							SerializedProperty userEventsDesc = m_Events.GetArrayElementAtIndex(i);

							SerializedProperty desc = userEventsDesc.FindPropertyRelative("m_Desc");
							SerializedProperty events = userEventsDesc.FindPropertyRelative("m_Event");

							if (desc != null)
							{
								SerializedProperty slot = desc.FindPropertyRelative("m_Slot");
								SerializedProperty name = desc.FindPropertyRelative("m_Name");

								EditorGUILayout.BeginHorizontal();
								EditorGUILayout.LabelField("Slot: " + slot.intValue + " Event Name: \"" + name.stringValue + "\"");
								EditorGUILayout.EndHorizontal();
								EditorGUILayout.PropertyField(events);
							}
						}
					}
				}

				if (EditorGUI.EndChangeCheck())
				{
					m_RequiresApplyModifiedProperties = true;
				}
			}
		}

		private void DrawAttributes()
		{
			PKFxEmitter fx = target as PKFxEmitter;

			//Asset as been updated by reimport.
			if (fx.EffectAsset.m_SamplerDescsHash != fx.FXSamplersDescHash ||
				fx.EffectAsset.m_AttributeDescsHash != fx.FXAttributesDescHash)
			{
				fx.UpdateEffectAsset(fx.EffectAsset, false);
				m_RequiresApplyModifiedProperties = true;
			}
			if (m_FxAttributesDesc.arraySize == 0 && m_FxSamplers.arraySize == 0)
				return;
			using (var category = new PKFxEditorCategory(DrawAttributesHeader))
			{
				if (!category.IsExpanded())
					return;
				EditorGUI.BeginChangeCheck();
				{
					int numLine = 0;

					if (serializedObject.targetObjects.Length == 1)
					{
						PKFxAttributesContainer attribContainer = fx.AttributesContainer;

						PKFxEmitter inspectedObject = m_FxAttributesStartValues.serializedObject.targetObject as PKFxEmitter;

						for (int i = 0; i < m_FxAttributesDesc.arraySize; i++)
						{
							SerializedProperty attrDesc = m_FxAttributesDesc.GetArrayElementAtIndex(i);
							SerializedProperty propertyX = m_FxAttributesStartValues.GetArrayElementAtIndex(i * 4 + 0);
							SerializedProperty propertyY = m_FxAttributesStartValues.GetArrayElementAtIndex(i * 4 + 1);
							SerializedProperty propertyZ = m_FxAttributesStartValues.GetArrayElementAtIndex(i * 4 + 2);
							SerializedProperty propertyW = m_FxAttributesStartValues.GetArrayElementAtIndex(i * 4 + 3);

							SetColorBackgroundByParity(numLine);

							SerializedProperty m_Type = attrDesc.FindPropertyRelative("m_Type");
							SerializedProperty m_DropMode = attrDesc.FindPropertyRelative("m_DropMode");
							SerializedProperty x, y, z, w;
							if (m_DropMode.intValue == (int)EAttributeDropMode.None)
							{
								switch ((EAttributeType)m_Type.intValue)
								{
									case EAttributeType.Bool:
									case EAttributeType.Bool2:
									case EAttributeType.Bool3:
									case EAttributeType.Bool4:
										//To fix
										x = propertyX.FindPropertyRelative("f1");

										if (PKFxAttributePropertyDrawer.DrawAttributeBool(attrDesc, x))
										{
											if (attribContainer != null)
												attribContainer.SetAttributeUnsafe(i, x.floatValue, 0, 0, 0);
										}

										break;
									case EAttributeType.Float:
									case EAttributeType.Float2:
									case EAttributeType.Float3:
									case EAttributeType.Float4:
										x = propertyX.FindPropertyRelative("f1");
										y = propertyY.FindPropertyRelative("f1");
										z = propertyZ.FindPropertyRelative("f1");
										w = propertyW.FindPropertyRelative("f1");

										if (PKFxAttributePropertyDrawer.DrawAttributeFloat(attrDesc, x, y, z, w))
										{
											if (attribContainer != null)
												attribContainer.SetAttributeUnsafe(i, x.floatValue, y.floatValue, z.floatValue, w.floatValue);
										}
										break;
									case EAttributeType.Int:
									case EAttributeType.Int2:
									case EAttributeType.Int3:
									case EAttributeType.Int4:
										x = propertyX.FindPropertyRelative("f1");
										y = propertyY.FindPropertyRelative("f1");
										z = propertyZ.FindPropertyRelative("f1");
										w = propertyW.FindPropertyRelative("f1");

										if (PKFxAttributePropertyDrawer.DrawAttributeInt(attrDesc, x, y, z, w))
										{
											if (attribContainer != null)
												attribContainer.SetAttributeUnsafe(i, PKFxUtils.Float2Int(x.floatValue),
																					  PKFxUtils.Float2Int(y.floatValue),
																					  PKFxUtils.Float2Int(z.floatValue),
																					  PKFxUtils.Float2Int(w.floatValue));
										}
										break;
								}

							}
							else
							{
								SerializedProperty m_DropList = attrDesc.FindPropertyRelative("m_DropNameList");

								++EditorGUI.indentLevel;
								x = propertyX.FindPropertyRelative("f1");
								EditorGUILayout.BeginHorizontal();
								PKFxAttributePropertyDrawer.DrawAttributeName(attrDesc);
								int newValue = EditorGUILayout.Popup((int)x.floatValue, m_DropList.stringValue.Split('|').Where(x => !string.IsNullOrEmpty(x)).ToArray());
								if (newValue != (int)x.floatValue)
								{
									x.floatValue = newValue;
									if (attribContainer != null)
										attribContainer.SetAttributeUnsafe(i, PKFxUtils.Float2Int(newValue));
								}
								EditorGUILayout.EndHorizontal();
								--EditorGUI.indentLevel;
							}
							EditorGUILayout.EndVertical();
							++numLine;
						}
						if (m_FxAttributesDesc.arraySize > 0 && m_FxSamplers.arraySize > 0)
						{
							PkFxEditorSplitter.Splitter(0);
						}

						for (int i = 0; i < m_FxSamplers.arraySize; i++)
						{
							SerializedProperty smp = m_FxSamplers.GetArrayElementAtIndex(i);
							SamplerField(smp);
						}
					}

					//Clear FxName, Attributes and Samplers if the asset is None
					if (m_FxAsset.objectReferenceInstanceIDValue == 0)
					{
						m_FxAttributesDesc.ClearArray();
						m_FxAttributesStartValues.ClearArray();
						m_FxSamplers.ClearArray();
						m_FxName.stringValue = "";
					}
				}

				if (EditorGUI.EndChangeCheck())
				{
					m_RequiresApplyModifiedProperties = true;
				}
			}
		}

		private Texture2D MakeTex(int width, int height, Color col)
		{
			Color[] pix = new Color[width * height];

			for (int i = 0; i < pix.Length; i++)
				pix[i] = col;

			Texture2D result = new Texture2D(width, height);
			result.SetPixels(pix);
			result.Apply();

			return result;
		}

		private void SetColorBackgroundByParity(int numLine)
		{
			EditorGUILayout.BeginVertical((numLine % 2 == 0) ? m_oddStyle : m_pairStyle);
		}

		private bool DrawEventsHeader()
		{
			EditorGUILayout.BeginHorizontal();
			m_Events.isExpanded = EditorGUILayout.Foldout(m_Events.isExpanded, "Events", true);

			DrawHeader("Events");
			return (m_Events.isExpanded);
		}

		private bool DrawAttributesHeader()
		{
			EditorGUILayout.BeginHorizontal();
			m_FxAttributesDesc.isExpanded = EditorGUILayout.Foldout(m_FxAttributesDesc.isExpanded, "Attribute", true);

			DrawHeader("Attribute");
			return (m_FxAttributesDesc.isExpanded);
		}

		private void DrawHeader(string title)
		{
			EditorGUILayout.Separator();

			if (serializedObject.targetObjects.Length == 1)
			{
				if (GUILayout.Button(new GUIContent("Reset All", "Reset all attributes to default values"), GUILayout.Width(70)))
				{
					Object[] effects = serializedObject.targetObjects;
					foreach (PKFxEmitter fx in effects)
					{
						if (EditorUpdatePkFX(fx, true))
						{
							ForceSerializedObjectUpdate(fx);
						}
					}
				}
			}
			EditorGUILayout.EndHorizontal();
			EditorGUILayout.Separator();
		}

		//----------------------------------------------------------------------------

		private void DrawPlaybackControls()
		{
			Object[] effects = serializedObject.targetObjects;
			bool effectsPlaying = true;

			foreach (PKFxEmitter fx in effects)
			{
				if (!fx.Alive)
					effectsPlaying = false;
			}

			EditorGUILayout.BeginHorizontal();
			EditorGUI.BeginDisabledGroup(!EditorApplication.isPlaying);

			if (!effectsPlaying)
			{
				if (GUILayout.Button(
					new GUIContent(m_CtrlPlayIcon, "Start emitter"),
					EditorStyles.miniButtonLeft,
					GUILayout.MaxHeight(m_PlaybackControlsMaxHeight)))
				{
					foreach (PKFxEmitter fx in effects)
					{
						if (!fx.Alive)
							fx.StartEffect();
					}
				}
			}
			else
			{
				if (GUILayout.Button(
					new GUIContent(m_CtrlStopIcon, "Stop emitter"),
					EditorStyles.miniButtonLeft,
					GUILayout.MaxHeight(m_PlaybackControlsMaxHeight)))
				{
					foreach (PKFxEmitter fx in effects)
					{
						fx.TerminateEffect();
					}
				}
			}

			if (GUILayout.Button(
				new GUIContent(m_CtrlRestartIcon, "Restart emitter"),
				EditorStyles.miniButtonMid,
				GUILayout.MaxHeight(m_PlaybackControlsMaxHeight)))
			{
				foreach (PKFxEmitter fx in effects)
				{
					if (fx.Alive)
						fx.TerminateEffect();
					fx.StartEffect();
				}
			}

			EditorGUI.BeginDisabledGroup(!effectsPlaying);
			if (GUILayout.Button(
				new GUIContent(m_CtrlKillIcon, "Stop emitter and destroy particles"),
				EditorStyles.miniButtonRight,
				GUILayout.MaxHeight(m_PlaybackControlsMaxHeight)))
			{
				foreach (PKFxEmitter fx in effects)
				{
					fx.KillEffect();
				}
			}
			EditorGUI.EndDisabledGroup();

			EditorGUI.EndDisabledGroup();
			EditorGUILayout.EndHorizontal();
		}

		//----------------------------------------------------------------------------

		private SerializedProperty SamplerField(SerializedProperty sampler)
		{
			bool hasChanged = false;
			SerializedProperty samplerName = sampler.FindPropertyRelative("m_Descriptor.m_Name");
			SerializedProperty samplerType = sampler.FindPropertyRelative("m_Descriptor.m_Type");
			SerializedProperty descriptionProp = sampler.FindPropertyRelative("m_Descriptor.m_Description");
			string description = descriptionProp.stringValue.Replace("\\n", "\n");

			EditorGUI.indentLevel++;
			sampler.isExpanded = EditorGUILayout.Foldout(sampler.isExpanded, new GUIContent(samplerName.stringValue, description), true);
			if (!sampler.isExpanded)
			{
				EditorGUI.indentLevel--;
				return sampler;
			}

			ESamplerType eType = (ESamplerType)samplerType.intValue;

			if (eType == ESamplerType.SamplerShape)
			{
				SerializedProperty m_ShapeType = sampler.FindPropertyRelative("m_ShapeType");

				int oldType = m_ShapeType.intValue;

				m_ShapeType.intValue = EditorGUILayout.Popup(m_ShapeType.intValue, ShapeTypes);

				bool shapeHasChanged = oldType != m_ShapeType.intValue;
				bool meshHasChanged = false;

				SerializedProperty worldSpaceSampling = sampler.FindPropertyRelative("m_WorldSpaceSampling");

				// Show world space:
				worldSpaceSampling.boolValue = EditorGUILayout.Toggle("Show in world space", worldSpaceSampling.boolValue);

				if (m_ShapeType.intValue == (int)Sampler.EShapeType.BoxShape)
					BoxField(sampler, shapeHasChanged);
				else if (m_ShapeType.intValue == (int)Sampler.EShapeType.SphereShape)
					SphereField(sampler, shapeHasChanged);
				else if (m_ShapeType.intValue == (int)Sampler.EShapeType.CylinderShape)
					CylinderField(sampler, shapeHasChanged);
				else if (m_ShapeType.intValue == (int)Sampler.EShapeType.CapsuleShape)
					CapsuleField(sampler, shapeHasChanged);
				else if (m_ShapeType.intValue == (int)Sampler.EShapeType.MeshShape)
					meshHasChanged = MeshField(sampler, shapeHasChanged);
				else if (m_ShapeType.intValue == (int)Sampler.EShapeType.MeshFilterShape)
					meshHasChanged = MeshFilterField(sampler, shapeHasChanged);
				else if (m_ShapeType.intValue == (int)Sampler.EShapeType.SkinnedMeshShape)
					meshHasChanged = SkinnedMeshField(sampler, shapeHasChanged);
				else if (m_ShapeType.intValue == (int)Sampler.EShapeType.BakedMeshShape)
					meshHasChanged = BakedMeshField(sampler, shapeHasChanged);
				else
				{
					SerializedProperty m_Transform = sampler.FindPropertyRelative("m_ShapeTransform");
					SerializedProperty m_TransformRef = sampler.FindPropertyRelative("m_ShapeTransformReference");

					DrawShapeTransform(m_Transform, m_TransformRef);
				}

				hasChanged = shapeHasChanged || meshHasChanged;
			}
			else if (eType == ESamplerType.SamplerCurve)
			{
				SerializedProperty m_CurvesArray = sampler.FindPropertyRelative("m_CurvesArray");
				SerializedProperty m_CurvesTimeKeys = sampler.FindPropertyRelative("m_CurvesTimeKeys");
				SerializedProperty m_CurveIsOverride = sampler.FindPropertyRelative("m_CurveIsOverride");

				hasChanged = MultipleCurvesEditor(m_CurvesArray);

				if (hasChanged)
					m_CurveIsOverride.boolValue = true;

				if (m_CurvesArray.arraySize != 0)
				{
					int iKey = 0;
					m_CurvesTimeKeys.arraySize = m_CurvesArray.GetArrayElementAtIndex(0).animationCurveValue.keys.Length;

					Keyframe[] keyframes = m_CurvesArray.GetArrayElementAtIndex(0).animationCurveValue.keys;

					foreach (var key in keyframes)
						m_CurvesTimeKeys.GetArrayElementAtIndex(iKey++).floatValue = key.time;
				}
				else
					m_CurvesTimeKeys.arraySize = 0;
			}
			else if (eType == ESamplerType.SamplerImage)
			{
				SerializedProperty m_Tex = sampler.FindPropertyRelative("m_Texture");
				SerializedProperty m_TextureTexcoordMode = sampler.FindPropertyRelative("m_TextureTexcoordMode");

				Texture2D newTex = (Texture2D)EditorGUILayout.ObjectField("Texture", m_Tex.objectReferenceValue, typeof(Texture2D), false);

				if (newTex != m_Tex.objectReferenceValue)
				{
					m_Tex.objectReferenceValue = newTex;
					hasChanged = true;
				}
				EditorGUILayout.LabelField("Texcoord Mode");
				Sampler.ETexcoordMode newType = (Sampler.ETexcoordMode)EditorGUILayout.EnumPopup((Sampler.ETexcoordMode)m_TextureTexcoordMode.intValue);
				m_TextureTexcoordMode.intValue = (int)newType;
			}
			else if (eType == ESamplerType.SamplerText)
			{
				SerializedProperty m_Text = sampler.FindPropertyRelative("m_Text");

				string newValue = EditorGUILayout.TextField(m_Text.stringValue);

				if (m_Text.stringValue != newValue)
				{
					m_Text.stringValue = newValue;
					hasChanged = true;
				}
			}
			else if (eType == ESamplerType.SamplerUnsupported)
			{
				EditorGUI.BeginDisabledGroup(true);
				EditorGUILayout.LabelField("Edits not supported");
				EditorGUI.EndDisabledGroup();
			}
			else
			{
				Debug.LogWarning("[PopcornFX] Unknown Sampler type.");
			}

			if (hasChanged)
			{
				SerializedProperty wasModified = sampler.FindPropertyRelative("m_WasModified");
				wasModified.boolValue = true;
				m_RequiresApplyModifiedProperties = true;
			}
			EditorGUI.indentLevel--;
			return sampler;
		}

		public bool MultipleCurvesEditor(SerializedProperty curvesArray)
		{
			bool hasChanged = false;
			for (int i = 0; i < curvesArray.arraySize; ++i)
			{
				Keyframe[] keysCache = curvesArray.GetArrayElementAtIndex(i).animationCurveValue.keys;
				EditorGUILayout.PropertyField(curvesArray.GetArrayElementAtIndex(i), new GUIContent(CurveDimensionsNames[i]));
				hasChanged |= MultipleCurvesCheckModify(curvesArray, i, keysCache);
			}
			return hasChanged;
		}

		public bool MultipleCurvesCheckModify(SerializedProperty currentKeysProp, int curveIndex, Keyframe[] oldKeys)
		{
			bool hasChanged = false;

			AnimationCurve[] curves = new AnimationCurve[currentKeysProp.arraySize];
			for (int i = 0; i < currentKeysProp.arraySize; ++i)
				curves[i] = new AnimationCurve(currentKeysProp.GetArrayElementAtIndex(i).animationCurveValue.keys);

			AnimationCurve curve = curves[curveIndex];
			if (curve.keys.Length > 0)
			{
				if (curve.keys.Length < 2)
				{
					hasChanged = true;
					curve.keys = oldKeys;
				}
				curves[curveIndex] = curve;

				List<float> changedOldKeys;
				List<float> changedNewKeys;

				MultipleCurvesFindKeysChanges(curve.keys, oldKeys, out changedOldKeys, out changedNewKeys);

				for (int i = 0; i < changedOldKeys.Count; ++i)
				{
					int idx = changedNewKeys.FindIndex(x => x == changedOldKeys[i]);

					if (idx == -1)
					{
						MultipleCurvesDeleteKey(ref curves, currentKeysProp, curveIndex, changedOldKeys[i]);
						hasChanged = true;
					}
					else
					{
						MultipleCurvesChangeKey(ref curves, currentKeysProp, curveIndex, changedOldKeys[i], changedNewKeys[idx]);
						hasChanged = true;
					}
				}
				// No need to change the keys here, all the key that should gave changed are now updated:
				for (int i = 0; i < changedNewKeys.Count; ++i)
				{
					if (!changedOldKeys.Exists(x => x == changedNewKeys[i]))
					{
						MultipleCurvesAddKey(ref curves, currentKeysProp, curveIndex, changedNewKeys[i]);
						hasChanged = true;
					}
				}
			}

			if (hasChanged)
			{
				for (int i = 0; i < currentKeysProp.arraySize; ++i)
					currentKeysProp.GetArrayElementAtIndex(i).animationCurveValue = curves[i];
			}

			return hasChanged;
		}

		public void MultipleCurvesFindKeysCountChanges(Keyframe[] refKeys, Keyframe[] compKeys, List<float> diffKeys)
		{
			foreach (var key in refKeys)
			{
				bool found = false;
				foreach (var othkey in compKeys)
				{
					if (key.time == othkey.time)
					{
						found = true;
						break;
					}
				}
				if (!found)
					diffKeys.Add(key.time);
			}
		}

		public void MultipleCurvesFindKeysChanges(Keyframe[] actualKeys, Keyframe[] cacheKeys, out List<float> oldKeys, out List<float> newKeys)
		{
			List<float> addedKeys = new List<float>();
			List<float> deletedKeys = new List<float>();
			MultipleCurvesFindKeysCountChanges(actualKeys, cacheKeys, addedKeys);
			MultipleCurvesFindKeysCountChanges(cacheKeys, actualKeys, deletedKeys);
			if (addedKeys.Count != 0 && addedKeys.Count == deletedKeys.Count)
			{
				addedKeys.Sort();
				deletedKeys.Sort();
			}
			oldKeys = deletedKeys;
			newKeys = addedKeys;
		}

		public void MultipleCurvesAddKey(ref AnimationCurve[] curves, SerializedProperty curvesArray, int sourceCurveIndex, float time)
		{
			for (int i = 0; i < curvesArray.arraySize; ++i)
			{
				if (i == sourceCurveIndex)
					continue;
				AnimationCurve curve = curves[i];
				//Evaluate the curve data on the unmodified version of the curve;
				AnimationCurve curveData = curvesArray.GetArrayElementAtIndex(i).animationCurveValue;
				curve.AddKey(time, curveData.Evaluate(time));
			}
		}

		public void MultipleCurvesDeleteKey(ref AnimationCurve[] curves, SerializedProperty curvesArray, int sourceCurveIndex, float time)
		{
			for (int i = 0; i < curvesArray.arraySize; ++i)
			{
				if (i == sourceCurveIndex)
					continue;
				AnimationCurve curve = curves[i];
				for (int iKey = 0; iKey < curve.keys.Length; ++iKey)
				{
					if (curve.keys[iKey].time == time)
					{
						curve.RemoveKey(iKey);
						break;
					}
				}
			}
		}

		public void MultipleCurvesChangeKey(ref AnimationCurve[] curves, SerializedProperty curvesArray, int sourceCurveIndex, float oldTime, float newTime)
		{
			for (int i = 0; i < curvesArray.arraySize; ++i)
			{
				if (i == sourceCurveIndex)
					continue;
				AnimationCurve curve = curves[i];
				int iKey;
				for (iKey = 0; iKey < curve.keys.Length; ++iKey)
				{
					if (curve.keys[iKey].time == oldTime)
					{
						Keyframe keyframe = curve.keys[iKey];
						keyframe.time = newTime;
						curve.MoveKey(iKey, keyframe);
						break;
					}
				}
			}
		}

		//----------------------------------------------------------------------------

		private static readonly string[] ShapeTypes = { "BOX", "SPHERE", "CYLINDER", "CAPSULE", "MESH", "MESHFILTER", "SKINNEDMESH", "BAKEDMESH", "Default" };
		private static readonly string[] CurveDimensionsNames = { "X", "Y", "Z", "W" };

		private static void BoxField(SerializedProperty sampler, bool shapeHasChanged)
		{
			SerializedProperty m_Transform = sampler.FindPropertyRelative("m_ShapeTransform");
			SerializedProperty m_TransformRef = sampler.FindPropertyRelative("m_ShapeTransformReference");
			SerializedProperty m_Dimensions = sampler.FindPropertyRelative("m_Dimensions");
			Vector3 oldValue = m_Dimensions.vector3Value;
			// Reset the default dimension:
			if (shapeHasChanged)
			{
				m_Dimensions.vector3Value = new Vector3(1, 1, 1);
			}

			EditorGUILayout.PropertyField(m_Dimensions);
			if (oldValue != m_Dimensions.vector3Value)
				sampler.FindPropertyRelative("m_WasModified").boolValue = true;
			DrawShapeTransform(m_Transform, m_TransformRef);
		}

		private static Transform transform;

		private static void SphereField(SerializedProperty sampler, bool shapeHasChanged)
		{
			SerializedProperty m_Transform = sampler.FindPropertyRelative("m_ShapeTransform");
			SerializedProperty m_TransformRef = sampler.FindPropertyRelative("m_ShapeTransformReference");
			SerializedProperty m_Dimensions = sampler.FindPropertyRelative("m_Dimensions");
			Vector3 oldValue = m_Dimensions.vector3Value;
			// Reset the default dimension:
			if (shapeHasChanged)
			{
				m_Dimensions.vector3Value = new Vector3(1, 0, 0);
			}

			Vector3 tmp = m_Dimensions.vector3Value;
			tmp.y = EditorGUILayout.FloatField("Inner Radius", Mathf.Min(tmp.x, tmp.y));
			tmp.x = EditorGUILayout.FloatField("Radius", Mathf.Max(tmp.x, tmp.y));
			m_Dimensions.vector3Value = tmp;
			if (oldValue != m_Dimensions.vector3Value)
				sampler.FindPropertyRelative("m_WasModified").boolValue = true;
			DrawShapeTransform(m_Transform, m_TransformRef);
		}

		private static void CylinderField(SerializedProperty sampler, bool shapeHasChanged)
		{
			SerializedProperty m_Transform = sampler.FindPropertyRelative("m_ShapeTransform");
			SerializedProperty m_TransformRef = sampler.FindPropertyRelative("m_ShapeTransformReference");
			SerializedProperty m_Dimensions = sampler.FindPropertyRelative("m_Dimensions");
			Vector3 oldValue = m_Dimensions.vector3Value;

			// Reset the default dimension:
			if (shapeHasChanged)
			{
				m_Dimensions.vector3Value = new Vector3(1, 0, 1);
			}

			Vector3 tmp = m_Dimensions.vector3Value;
			tmp.y = EditorGUILayout.FloatField("Inner Radius", Mathf.Min(tmp.x, tmp.y));
			tmp.x = EditorGUILayout.FloatField("Radius", Mathf.Max(tmp.x, tmp.y));
			tmp.z = EditorGUILayout.FloatField("Height", tmp.z);
			m_Dimensions.vector3Value = tmp;
			if (oldValue != m_Dimensions.vector3Value)
				sampler.FindPropertyRelative("m_WasModified").boolValue = true;
			DrawShapeTransform(m_Transform, m_TransformRef);
		}

		private static void CapsuleField(SerializedProperty sampler, bool shapeHasChanged)
		{
			SerializedProperty m_Transform = sampler.FindPropertyRelative("m_ShapeTransform");
			SerializedProperty m_TransformRef = sampler.FindPropertyRelative("m_ShapeTransformReference");
			SerializedProperty m_Dimensions = sampler.FindPropertyRelative("m_Dimensions");
			Vector3 oldValue = m_Dimensions.vector3Value;

			if (shapeHasChanged)
			{
				m_Dimensions.vector3Value = new Vector3(1, 0, 1);
			}

			Vector3 tmp = m_Dimensions.vector3Value;
			tmp.y = EditorGUILayout.FloatField("Inner Radius", Mathf.Min(tmp.x, tmp.y));
			tmp.x = EditorGUILayout.FloatField("Radius", Mathf.Max(tmp.x, tmp.y));
			tmp.z = EditorGUILayout.FloatField("Height", tmp.z);
			m_Dimensions.vector3Value = tmp;
			if (oldValue != m_Dimensions.vector3Value)
				sampler.FindPropertyRelative("m_WasModified").boolValue = true;
			DrawShapeTransform(m_Transform, m_TransformRef);
		}

		private static bool MeshField(SerializedProperty sampler, bool shapeHasChanged)
		{
			SerializedProperty m_Transform = sampler.FindPropertyRelative("m_ShapeTransform");
			SerializedProperty m_TransformRef = sampler.FindPropertyRelative("m_ShapeTransformReference");
			SerializedProperty m_Dimensions = sampler.FindPropertyRelative("m_Dimensions");
			SerializedProperty m_SkinnedMeshRenderer = sampler.FindPropertyRelative("m_SkinnedMeshRenderer");
			SerializedProperty m_BakedMesh = sampler.FindPropertyRelative("m_BakedMesh");
			SerializedProperty m_MeshFilter = sampler.FindPropertyRelative("m_MeshFilter");
			SerializedProperty m_Mesh = sampler.FindPropertyRelative("m_Mesh");
			Vector3 oldValue = m_Dimensions.vector3Value;

			if (shapeHasChanged)
			{
				m_Dimensions.vector3Value = Vector3.one;
			}

			int oldNameHash = 0;
			int newNameHash = 0;

			if (m_Mesh.objectReferenceValue != null)
			{
				oldNameHash = (m_Mesh.objectReferenceValue as Mesh).name.GetHashCode();
			}

			EditorGUILayout.PropertyField(m_Mesh);

			if (m_Mesh.objectReferenceValue != null)
			{
				newNameHash = (m_Mesh.objectReferenceValue as Mesh).name.GetHashCode();
			}

			m_SkinnedMeshRenderer.objectReferenceValue = null;
			m_MeshFilter.objectReferenceValue = null;
			m_BakedMesh.objectReferenceValue = null;
			EditorGUILayout.PropertyField(m_Dimensions);
			if (oldValue != m_Dimensions.vector3Value)
				sampler.FindPropertyRelative("m_WasModified").boolValue = true;
			DrawShapeTransform(m_Transform, m_TransformRef);
			return oldNameHash != newNameHash;
		}

		private static bool MeshFilterField(SerializedProperty sampler, bool shapeHasChanged)
		{
			SerializedProperty m_Transform = sampler.FindPropertyRelative("m_ShapeTransform");
			SerializedProperty m_TransformRef = sampler.FindPropertyRelative("m_ShapeTransformReference");
			SerializedProperty m_Dimensions = sampler.FindPropertyRelative("m_Dimensions");
			SerializedProperty m_SkinnedMeshRenderer = sampler.FindPropertyRelative("m_SkinnedMeshRenderer");
			SerializedProperty m_BakedMesh = sampler.FindPropertyRelative("m_BakedMesh");
			SerializedProperty m_MeshFilter = sampler.FindPropertyRelative("m_MeshFilter");
			SerializedProperty m_Mesh = sampler.FindPropertyRelative("m_Mesh");
			Vector3 oldValue = m_Dimensions.vector3Value;

			if (shapeHasChanged)
			{
				m_Dimensions.vector3Value = Vector3.one;
			}

			int oldNameHash = 0;
			int newNameHash = 0;

			if (m_Mesh.objectReferenceValue != null)
			{
				oldNameHash = (m_Mesh.objectReferenceValue as Mesh).name.GetHashCode();
			}

			EditorGUILayout.PropertyField(m_MeshFilter);

			if (m_MeshFilter.objectReferenceValue != null)
			{
				m_Mesh.objectReferenceValue = (m_MeshFilter.objectReferenceValue as MeshFilter).sharedMesh;
				if (m_Mesh.objectReferenceValue != null)
				{
					newNameHash = (m_Mesh.objectReferenceValue as Mesh).name.GetHashCode();
				}
			}
			else
			{
				m_Mesh.objectReferenceValue = null;
			}

			m_SkinnedMeshRenderer.objectReferenceValue = null;
			m_BakedMesh.objectReferenceValue = null;
			EditorGUILayout.PropertyField(m_Dimensions);
			if (oldValue != m_Dimensions.vector3Value)
				sampler.FindPropertyRelative("m_WasModified").boolValue = true;
			DrawShapeTransform(m_Transform, m_TransformRef);
			return oldNameHash != newNameHash;
		}

		private static bool SkinnedMeshField(SerializedProperty sampler, bool shapeHasChanged)
		{
			SerializedProperty m_Transform = sampler.FindPropertyRelative("m_ShapeTransform");
			SerializedProperty m_TransformRef = sampler.FindPropertyRelative("m_ShapeTransformReference");
			SerializedProperty m_Dimensions = sampler.FindPropertyRelative("m_Dimensions");
			SerializedProperty m_SkinnedMeshRenderer = sampler.FindPropertyRelative("m_SkinnedMeshRenderer");
			SerializedProperty m_BakedMesh = sampler.FindPropertyRelative("m_BakedMesh");
			SerializedProperty m_MeshFilter = sampler.FindPropertyRelative("m_MeshFilter");
			SerializedProperty m_Mesh = sampler.FindPropertyRelative("m_Mesh");
			Vector3 oldValue = m_Dimensions.vector3Value;

			if (shapeHasChanged)
			{
				m_Dimensions.vector3Value = Vector3.one;
			}

			int oldNameHash = 0;
			int newNameHash = 0;

			if (m_Mesh.objectReferenceValue != null)
			{
				oldNameHash = (m_Mesh.objectReferenceValue as Mesh).name.GetHashCode();
			}

			EditorGUILayout.PropertyField(m_SkinnedMeshRenderer);

			if (m_SkinnedMeshRenderer.objectReferenceValue != null)
			{
				Mesh mesh = (m_SkinnedMeshRenderer.objectReferenceValue as SkinnedMeshRenderer).sharedMesh;
				if (mesh != null)
				{
					m_Mesh.objectReferenceValue = mesh;
				}
				else
				{
					m_Mesh.objectReferenceValue = null;
				}
			}
			else
			{
				m_Mesh.objectReferenceValue = null;
			}

			if (m_Mesh.objectReferenceValue != null)
			{
				newNameHash = (m_Mesh.objectReferenceValue as Mesh).name.GetHashCode();
			}

			m_MeshFilter.objectReferenceValue = null;
			m_BakedMesh.objectReferenceValue = null;
			EditorGUILayout.PropertyField(m_Dimensions);
			if (oldValue != m_Dimensions.vector3Value)
				sampler.FindPropertyRelative("m_WasModified").boolValue = true;
			DrawShapeTransform(m_Transform, m_TransformRef);
			return oldNameHash != newNameHash;
		}

		private static bool BakedMeshField(SerializedProperty sampler, bool shapeHasChanged)
		{
			SerializedProperty m_Transform = sampler.FindPropertyRelative("m_ShapeTransform");
			SerializedProperty m_TransformRef = sampler.FindPropertyRelative("m_ShapeTransformReference");
			SerializedProperty m_Dimensions = sampler.FindPropertyRelative("m_Dimensions");
			SerializedProperty m_SkinnedMeshRenderer = sampler.FindPropertyRelative("m_SkinnedMeshRenderer");
			SerializedProperty m_BakedMeshSubMeshId = sampler.FindPropertyRelative("m_BakedMeshSubMeshId");
			SerializedProperty m_BakedMesh = sampler.FindPropertyRelative("m_BakedMesh");
			SerializedProperty m_MeshFilter = sampler.FindPropertyRelative("m_MeshFilter");
			SerializedProperty m_Mesh = sampler.FindPropertyRelative("m_Mesh");
			Vector3 oldValue = m_Dimensions.vector3Value;

			if (shapeHasChanged)
			{
				m_Dimensions.vector3Value = Vector3.one;
			}

			int oldNameHash = 0;
			int newNameHash = 0;

			if (m_BakedMesh.objectReferenceValue != null)
			{
				oldNameHash = (m_BakedMesh.objectReferenceValue as PKFxMeshAsset).name.GetHashCode();
			}

			EditorGUILayout.PropertyField(m_BakedMesh);
			EditorGUILayout.PropertyField(m_BakedMeshSubMeshId);
			EditorGUILayout.PropertyField(m_SkinnedMeshRenderer);

			if (m_BakedMesh.objectReferenceValue != null)
			{
				newNameHash = (m_BakedMesh.objectReferenceValue as PKFxMeshAsset).name.GetHashCode();
			}

			m_Mesh.objectReferenceValue = null;
			m_MeshFilter.objectReferenceValue = null;
			EditorGUILayout.PropertyField(m_Dimensions);
			if (oldValue != m_Dimensions.vector3Value)
				sampler.FindPropertyRelative("m_WasModified").boolValue = true;
			DrawShapeTransform(m_Transform, m_TransformRef);
			return oldNameHash != newNameHash;
		}

		//----------------------------------------------------------------------------

		public void DrawSphere(int i, Sampler sampler, bool worldSpaceSampling)
		{
			PKFxEmitter fx = (PKFxEmitter)target;
			float radius = sampler.Dimensions.x;
			float innerRadius = sampler.Dimensions.y;
			Matrix4x4 shapeTransform;

			if (sampler.m_ShapeTransformReference != null)
				shapeTransform = sampler.m_ShapeTransformReference.localToWorldMatrix;
			else
				shapeTransform = sampler.m_ShapeTransform.transform;

			if (!worldSpaceSampling)
			{
				shapeTransform = ((GameObject)fx.gameObject).transform.localToWorldMatrix * shapeTransform;
			}

			Vector3 position = shapeTransform.GetColumn(3);
			Quaternion rotation = shapeTransform.rotation;
			Vector3 scale = shapeTransform.lossyScale;

			Handles.color = Color.blue;
			innerRadius = Handles.RadiusHandle(rotation, position, Mathf.Min(radius, innerRadius));
			Handles.color = Color.cyan;
			radius = Handles.RadiusHandle(rotation, position, Mathf.Max(radius, innerRadius));
		}

		//----------------------------------------------------------------------------

		private void _PrimitiveCapsule(float radius, Vector2 minMax, float height, Vector3 center, Quaternion rotation)
		{
			Vector3 topCenter = center + new Vector3(0f, height / 2f, 0f);
			Vector3 lowCenter = center - new Vector3(0f, height / 2f, 0f);

			Vector3 dir = topCenter - center;
			dir = rotation * dir;
			topCenter = center + dir;

			dir = lowCenter - center;
			dir = rotation * dir;
			lowCenter = center + dir;

			if (minMax.x != -1)
			{
				radius = Handles.RadiusHandle(rotation, topCenter, Mathf.Max(radius, minMax.x));
				radius = Handles.RadiusHandle(rotation, lowCenter, Mathf.Max(radius, minMax.x));
			}
			else if (minMax.y != -1)
			{
				radius = Handles.RadiusHandle(rotation, topCenter, Mathf.Min(radius, minMax.y));
				radius = Handles.RadiusHandle(rotation, lowCenter, Mathf.Min(radius, minMax.y));
			}
			Handles.DrawLine(topCenter + rotation * new Vector3(radius, 0f, 0f), lowCenter + rotation * new Vector3(radius, 0f, 0f));
			Handles.DrawLine(topCenter - rotation * new Vector3(radius, 0f, 0f), lowCenter - rotation * new Vector3(radius, 0f, 0f));
			Handles.DrawLine(topCenter + rotation * new Vector3(0f, 0f, radius), lowCenter + rotation * new Vector3(0f, 0f, radius));
			Handles.DrawLine(topCenter - rotation * new Vector3(0f, 0f, radius), lowCenter - rotation * new Vector3(0f, 0f, radius));
		}

		public void DrawCapsule(int i, Sampler sampler, bool worldSpaceSampling)
		{
			PKFxEmitter fx = (PKFxEmitter)target;

			float radius = sampler.Dimensions.x;
			float innerRadius = sampler.Dimensions.y;
			float height = sampler.Dimensions.z;
			Matrix4x4 shapeTransform;

			if (sampler.m_ShapeTransformReference != null)
				shapeTransform = sampler.m_ShapeTransformReference.localToWorldMatrix;
			else
				shapeTransform = sampler.m_ShapeTransform.transform;

			if (!worldSpaceSampling)
			{
				shapeTransform = ((GameObject)fx.gameObject).transform.localToWorldMatrix * shapeTransform;
			}

			Vector3 position = shapeTransform.GetColumn(3);
			Quaternion rotation = shapeTransform.rotation;

			Handles.color = Color.blue;
			_PrimitiveCapsule(innerRadius, new Vector2(-1, radius), height, position, rotation);

			Handles.color = Color.cyan;
			_PrimitiveCapsule(radius, new Vector2(innerRadius, -1), height, position, rotation);
		}

		//----------------------------------------------------------------------------

		public void _PrimitiveCylinder(float radius, float height, Vector3 center, Quaternion rotation)
		{
			Vector3 topCenter = center + new Vector3(0f, height / 2f, 0f);
			Vector3 lowCenter = center - new Vector3(0f, height / 2f, 0f);

			Vector3 dir = topCenter - center;
			dir = rotation * dir;
			topCenter = center + dir;

			dir = lowCenter - center;
			dir = rotation * dir;
			lowCenter = center + dir;

#if UNITY_5_6_OR_NEWER
			Handles.CircleHandleCap(0, topCenter, rotation * Quaternion.FromToRotation(Vector3.forward, Vector3.up), radius, EventType.Repaint);
			Handles.CircleHandleCap(0, lowCenter, rotation * Quaternion.FromToRotation(Vector3.forward, Vector3.up), radius, EventType.Repaint);
#else
		Handles.CircleCap(0, topCenter, rotation * Quaternion.FromToRotation(Vector3.forward, Vector3.up), radius);
		Handles.CircleCap(0, lowCenter, rotation * Quaternion.FromToRotation(Vector3.forward, Vector3.up), radius);
#endif
			Handles.DrawLine(topCenter + rotation * new Vector3(radius, 0f, 0f), lowCenter + rotation * new Vector3(radius, 0f, 0f));
			Handles.DrawLine(topCenter - rotation * new Vector3(radius, 0f, 0f), lowCenter - rotation * new Vector3(radius, 0f, 0f));
			Handles.DrawLine(topCenter + rotation * new Vector3(0f, 0f, radius), lowCenter + rotation * new Vector3(0f, 0f, radius));
			Handles.DrawLine(topCenter - rotation * new Vector3(0f, 0f, radius), lowCenter - rotation * new Vector3(0f, 0f, radius));
		}

		public void DrawCylinder(int i, Sampler sampler, bool worldSpaceSampling)
		{
			PKFxEmitter fx = (PKFxEmitter)target;
			float radius = sampler.Dimensions.x;
			float innerRadius = sampler.Dimensions.y;
			float height = sampler.Dimensions.z;
			Matrix4x4 shapeTransform;

			if (sampler.m_ShapeTransformReference != null)
				shapeTransform = sampler.m_ShapeTransformReference.localToWorldMatrix;
			else
				shapeTransform = sampler.m_ShapeTransform.transform;

			if (!worldSpaceSampling)
			{
				shapeTransform = ((GameObject)fx.gameObject).transform.localToWorldMatrix * shapeTransform;
			}

			Vector3 position = shapeTransform.GetColumn(3);
			Quaternion rotation = shapeTransform.rotation;

			Handles.color = Color.blue;
			_PrimitiveCylinder(innerRadius, height, position, rotation);

			Handles.color = Color.cyan;
			_PrimitiveCylinder(radius, height, position, rotation);
		}

		//----------------------------------------------------------------------------

		public void DrawCube(int i, Sampler sampler, bool worldSpaceSampling)
		{
			PKFxEmitter fx = (PKFxEmitter)target;
			Matrix4x4 shapeTransform;

			if (sampler.m_ShapeTransformReference != null)
				shapeTransform = sampler.m_ShapeTransformReference.localToWorldMatrix;
			else
				shapeTransform = sampler.m_ShapeTransform.transform;

			if (!worldSpaceSampling)
			{
				shapeTransform = ((GameObject)fx.gameObject).transform.localToWorldMatrix * shapeTransform;
			}

			Vector3 position = shapeTransform.GetColumn(3);
			Quaternion rotation = shapeTransform.rotation;

			Vector3 size = sampler.Dimensions;
			Vector3 A = position + rotation * new Vector3(-size.x / 2, size.y / 2, size.z / 2);
			Vector3 B = position + rotation * new Vector3(size.x / 2, size.y / 2, size.z / 2);
			Vector3 C = position + rotation * new Vector3(size.x / 2, -size.y / 2, size.z / 2);
			Vector3 D = position + rotation * new Vector3(-size.x / 2, -size.y / 2, size.z / 2);
			Vector3 E = position + rotation * new Vector3(-size.x / 2, size.y / 2, -size.z / 2);
			Vector3 F = position + rotation * new Vector3(size.x / 2, size.y / 2, -size.z / 2);
			Vector3 G = position + rotation * new Vector3(size.x / 2, -size.y / 2, -size.z / 2);
			Vector3 H = position + rotation * new Vector3(-size.x / 2, -size.y / 2, -size.z / 2);
			Vector3[] face = new Vector3[5];

			Handles.color = Color.cyan;
			face[0] = A;
			face[1] = B;
			face[2] = C;
			face[3] = D;
			face[4] = A;
			Handles.DrawPolyLine(face);
			face[0] = A;
			face[1] = E;
			face[2] = H;
			face[3] = D;
			face[4] = A;
			Handles.DrawPolyLine(face);
			face[0] = B;
			face[1] = F;
			face[2] = G;
			face[3] = C;
			face[4] = B;
			Handles.DrawPolyLine(face);
			face[0] = E;
			face[1] = F;
			face[2] = G;
			face[3] = H;
			face[4] = E;
			Handles.DrawPolyLine(face);
		}

		public static void DrawShapeTransform(SerializedProperty shapeTransform, SerializedProperty shapeTransformRef)
		{
			bool disableTransformDraw = shapeTransformRef.objectReferenceValue != null;

			EditorGUILayout.PropertyField(shapeTransformRef);
			EditorGUI.BeginDisabledGroup(disableTransformDraw);
			++EditorGUI.indentLevel;
			shapeTransform.isExpanded = EditorGUILayout.Foldout(shapeTransform.isExpanded, "Transform");
			if (shapeTransform.isExpanded)
			{
				SerializedProperty positionProp = shapeTransform.FindPropertyRelative("m_Position");
				SerializedProperty rotationProp = shapeTransform.FindPropertyRelative("m_Rotation");
				SerializedProperty scaleProp = shapeTransform.FindPropertyRelative("m_Scale");

				positionProp.vector3Value = EditorGUILayout.Vector3Field("Position", positionProp.vector3Value);
				rotationProp.quaternionValue = Quaternion.Euler(EditorGUILayout.Vector3Field("Rotation", rotationProp.quaternionValue.eulerAngles));
				scaleProp.vector3Value = EditorGUILayout.Vector3Field("Scale", scaleProp.vector3Value);
			}
			--EditorGUI.indentLevel;
			EditorGUI.EndDisabledGroup();
		}

		public static bool EditorUpdatePkFX(PKFxEmitter fx, bool resetAllAttributes)
		{
			bool fxWasUpdated = false;
			if (fx.EffectAsset == null)
			{
				fx.ClearAllAttributesAndSamplers();
				fx.EffectName = "";
			}
			// We update the attributes and samplers IFN:
			else if (fx.UpdateEffectAsset(fx.EffectAsset, resetAllAttributes))
			{
				fxWasUpdated = true;
			}
			return fxWasUpdated;
		}
	}
}