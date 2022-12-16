//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Serialization;
using System;
using System.Runtime.InteropServices;

#if UNITY_EDITOR
using UnityEditor;
#endif

namespace PopcornFX
{
	public class PKFxEffectAsset : PKFxAsset
	{
		[HideInInspector]
		public override byte[] m_Data
		{
			get { return _data; }
			set
			{
				_data = value;
			}
		}

		public List<DependencyDesc> m_Dependencies = new List<DependencyDesc>();
		public List<AttributeDesc> m_AttributeDescs = new List<AttributeDesc>();
		public int m_AttributeDescsHash = 0;
		public List<SamplerDesc> m_SamplerDescs = new List<SamplerDesc>();
		public List<SBatchDesc> m_RendererDescs = new List<SBatchDesc>();
		public List<Material> m_Materials = new List<Material>();

		public List<PKFxCustomMaterialInfo> m_CustomMaterials = new List<PKFxCustomMaterialInfo>();
		public List<SEventDesc> m_EventDescs = new List<SEventDesc>();
		public int m_SamplerDescsHash = 0;
		public int m_RendererDescsHash = 0;
		public int m_EventDescsHash = 0;

		public bool m_UsesMeshRenderer = false;

		[Serializable]
		public class DependencyDesc
		{
			public string m_Path;
			public int m_UsageFlags;
			public UnityEngine.Object m_Object;

			public bool HasUsageFlag(EUseInfoFlag flag)
			{
				return (m_UsageFlags & (int)flag) != 0;
			}
		}

		[Serializable]
		public class AttributeDesc
		{
			// Has min, has max, has desc:
			public enum EAttrDescFlag : int
			{
				HasMin = 0x01,
				HasMax = 0x02,
				HasDesc = 0x04,
			};

			[FormerlySerializedAs("Type")] public EAttributeType m_Type;
			[FormerlySerializedAs("MinMaxFlag")] public int m_MinMaxFlag;
			[FormerlySerializedAs("Name")] public string m_Name;
			[FormerlySerializedAs("Description")] public string m_Description;

			public SAttribContainer_Vector4 m_DefaultValue;
			public SAttribContainer_Vector4 m_MinValue;
			public SAttribContainer_Vector4 m_MaxValue;

			public AttributeDesc(SNativeAttributeDesc desc)
			{
				m_Type = desc.m_AttributeType;
				m_MinMaxFlag = (int)desc.m_MinMaxFlag;
				m_Name = Marshal.PtrToStringAnsi(desc.m_AttributeName);
				//			if ((byte)desc.MinMaxFlag & EAttrDescFlag.HasDesc)
				m_Description = Marshal.PtrToStringAnsi(desc.m_Description);
				m_DefaultValue = desc.m_DefaultValue;
				m_MinValue = desc.m_MinValue;
				m_MaxValue = desc.m_MaxValue;

				if (m_Type == EAttributeType.Bool  ||
					m_Type == EAttributeType.Bool2 ||
					m_Type == EAttributeType.Bool3 ||
					m_Type == EAttributeType.Bool4)
				{
					m_DefaultValue.x.b1 = m_DefaultValue.x.b1 != 0 ? (byte)255 : (byte)0;
					m_DefaultValue.x.b2 = m_DefaultValue.x.b2 != 0 ? (byte)255 : (byte)0;
					m_DefaultValue.x.b3 = m_DefaultValue.x.b3 != 0 ? (byte)255 : (byte)0;
					m_DefaultValue.x.b4 = m_DefaultValue.x.b4 != 0 ? (byte)255 : (byte)0;
				}
			}

			public AttributeDesc(EAttributeType type, IntPtr name)
			{
				m_Type = type;
				m_Name = Marshal.PtrToStringAnsi(name);
			}

			public AttributeDesc(EAttributeType type, string name)
			{
				m_Type = type;
				m_Name = name;
			}

			public bool HasMin()
			{
				return (m_MinMaxFlag & (int)EAttrDescFlag.HasMin) != 0;
			}

			public bool HasMax()
			{
				return (m_MinMaxFlag & (int)EAttrDescFlag.HasMax) != 0;
			}

			public SAttribContainer_Vector4 GetDefaultAttributeValueClamped()
			{
				return ClampAttributeValue(m_DefaultValue);
			}

			public SAttribContainer_Vector4 ClampAttributeValue(SAttribContainer_Vector4 attributeValue)
			{
				SAttribContainer_Vector4 result = attributeValue;
				if (HasMin())
				{
					if (m_Type == EAttributeType.Float ||
						m_Type == EAttributeType.Float2 ||
						m_Type == EAttributeType.Float3 ||
						m_Type == EAttributeType.Float4)
					{
						result.x.f1 = Mathf.Max(m_MinValue.x.f1, attributeValue.x.f1);
						result.y.f1 = Mathf.Max(m_MinValue.y.f1, attributeValue.y.f1);
						result.z.f1 = Mathf.Max(m_MinValue.z.f1, attributeValue.z.f1);
						result.w.f1 = Mathf.Max(m_MinValue.w.f1, attributeValue.w.f1);
					}
					else if (m_Type == EAttributeType.Int ||
							 m_Type == EAttributeType.Int2 ||
							 m_Type == EAttributeType.Int3 ||
							 m_Type == EAttributeType.Int4)
					{
						result.x.i1 = Math.Max(m_MinValue.x.i1, attributeValue.x.i1);
						result.y.i1 = Math.Max(m_MinValue.y.i1, attributeValue.y.i1);
						result.z.i1 = Math.Max(m_MinValue.z.i1, attributeValue.z.i1);
						result.w.i1 = Math.Max(m_MinValue.w.i1, attributeValue.w.i1);
					}
				}
				if (HasMax())
				{
					if (m_Type == EAttributeType.Float ||
						m_Type == EAttributeType.Float2 ||
						m_Type == EAttributeType.Float3 ||
						m_Type == EAttributeType.Float4)
					{
						result.x.f1 = Math.Min(m_MaxValue.x.f1, attributeValue.x.f1);
						result.y.f1 = Math.Min(m_MaxValue.y.f1, attributeValue.y.f1);
						result.z.f1 = Math.Min(m_MaxValue.z.f1, attributeValue.z.f1);
						result.w.f1 = Math.Min(m_MaxValue.w.f1, attributeValue.w.f1);
					}
					else if (m_Type == EAttributeType.Int ||
							 m_Type == EAttributeType.Int2 ||
							 m_Type == EAttributeType.Int3 ||
							 m_Type == EAttributeType.Int4)
					{
						result.x.i1 = Math.Min(m_MaxValue.x.i1, attributeValue.x.i1);
						result.y.i1 = Math.Min(m_MaxValue.y.i1, attributeValue.y.i1);
						result.z.i1 = Math.Min(m_MaxValue.z.i1, attributeValue.z.i1);
						result.w.i1 = Math.Min(m_MaxValue.w.i1, attributeValue.w.i1);
					}
				}
				return result;
			}

			public override string ToString()
			{
				string minval = m_MinValue.x.f1 + ";" + m_MinValue.y.f1 + ";" + m_MinValue.z.f1 + ";" + m_MinValue.w.f1;
				string maxval = m_MaxValue.x.f1 + ";" + m_MaxValue.y.f1 + ";" + m_MaxValue.z.f1 + ";" + m_MaxValue.w.f1;
				string dftval = m_DefaultValue.x.f1 + ";" + m_DefaultValue.y.f1 + ";" + m_DefaultValue.z.f1 + ";" + m_DefaultValue.w.f1;
				// Just the type and the name are taken into account when mix and matching the attributes:
				return m_Type.ToString() + ";" + m_Name + ";" + m_MinMaxFlag + ";" + minval + ";" + maxval + ";" + dftval;
			}
		}

		public PKFxShaderInputBindings GetRuntimeShaderInputBindings(SBatchDesc batchDesc)
		{
			PKFxCustomMaterialInfo curMat = FindCustomMaterialInfo(batchDesc);
			if (curMat != null && curMat.m_CustomMaterial != null)
			{
				return curMat;
			}
			else
			{
				return PKFxSettings.MaterialFactory.ResolveBatchBinding(batchDesc);
			}
		}

		public PKFxCustomMaterialInfo FindCustomMaterialInfo(SBatchDesc batchDesc)
		{
			if (m_CustomMaterials == null)
				return null;
			int idx = m_CustomMaterials.FindIndex(x => x.m_InternalId == batchDesc.m_InternalId);
			if (idx >= 0)
			{
				PKFxCustomMaterialInfo info = m_CustomMaterials[idx];
				if (AssetVirtualPath == info.m_AssetVirtualPath &&
					batchDesc.m_GeneratedName == info.m_BatchDescName &&
					batchDesc.m_InternalId == info.m_InternalId)
					return info;
			}
			return null;
		}

#if UNITY_EDITOR
		public void RemoveOutdatedCustomMaterials(SBatchDesc batchDesc)
		{
			if (m_CustomMaterials == null)
				return;
			int idx = m_CustomMaterials.FindIndex(x => x.m_InternalId == batchDesc.m_InternalId);
			
			if (idx >= 0)
			{
				PKFxCustomMaterialInfo info = m_CustomMaterials[idx];
				if (AssetVirtualPath == info.m_AssetVirtualPath &&
					batchDesc.m_GeneratedName == info.m_BatchDescName &&
					batchDesc.m_InternalId == info.m_InternalId)
					return;
				AssetDatabase.RemoveObjectFromAsset(info);
				m_CustomMaterials.RemoveAt(idx);
				AssetDatabase.SaveAssets();
			}
		}

		public static void DrawEditorCustomMaterial(PKFxCustomMaterialInfo customMat, bool showFx)
		{
			EditorGUI.BeginDisabledGroup(true);
			string assetVirtualPath = customMat.m_AssetVirtualPath;
			string assetFullPath = "Assets" + PKFxSettings.UnityPackFxPath + "/" + assetVirtualPath + ".asset";
			PKFxEffectAsset curAsset = (PKFxEffectAsset)AssetDatabase.LoadAssetAtPath(assetFullPath, typeof(PKFxEffectAsset));
			if (curAsset == null)
				return;
			if (showFx)
			{
				EditorGUILayout.BeginHorizontal();
				EditorGUILayout.LabelField("Effect Asset");
				EditorGUILayout.ObjectField(curAsset, typeof(PKFxEffectAsset), false);
				EditorGUILayout.EndHorizontal();
			}
			EditorGUILayout.BeginHorizontal();
			EditorGUILayout.LabelField("Renderer #" + customMat.m_InternalId + " Material");
			EditorGUILayout.ObjectField(customMat.m_CustomMaterial, typeof(Material), false);
			EditorGUILayout.EndHorizontal();
			EditorGUI.EndDisabledGroup();
			SBatchDesc customMatBatchDesc = curAsset.m_RendererDescs.Find(delegate (SBatchDesc desc)
			{
				return desc.m_GeneratedName == customMat.m_BatchDescName &&
						desc.m_InternalId == customMat.m_InternalId;
			});
			EditorGUI.indentLevel += 1;
			customMat.DrawEditorShaderInputBindings(customMatBatchDesc);
			EditorGUI.indentLevel -= 1;
		}

		public void AddCustomMaterial(SerializedProperty prop, SBatchDesc sBatchDesc, Material newMat, int id)
		{
			if (m_CustomMaterials == null)
				m_CustomMaterials = new List<PKFxCustomMaterialInfo>();

			PKFxCustomMaterialInfo customRule = FindCustomMaterialInfo(sBatchDesc);

			SerializedObject so; 
			if (customRule == null)
			{
				customRule = ScriptableObject.CreateInstance<PKFxCustomMaterialInfo>();
				customRule.name = AssetVirtualPath + " [" + id + "]";

				AssetDatabase.AddObjectToAsset(customRule, this);
				AssetDatabase.SaveAssets();

				prop.InsertArrayElementAtIndex(m_CustomMaterials.Count);
				prop.GetArrayElementAtIndex(m_CustomMaterials.Count).objectReferenceValue = customRule;

				so = new SerializedObject(customRule);
				so.FindProperty("m_AssetVirtualPath").stringValue = AssetVirtualPath;
				so.FindProperty("m_BatchDescName").stringValue = sBatchDesc.m_GeneratedName;
				so.FindProperty("m_InternalId").intValue = id;
				so.FindProperty("m_CustomMaterial").objectReferenceValue = newMat;
			}
			else
				so = new SerializedObject(customRule);

			int idx = m_CustomMaterials.FindIndex(x => x.m_InternalId == sBatchDesc.m_InternalId);
			if (idx >= 0)
			{
				so.FindProperty("m_CustomMaterial").objectReferenceValue = newMat;
				prop.GetArrayElementAtIndex(idx).objectReferenceValue = customRule;
				m_Materials[idx] = newMat;
			}

			so.ApplyModifiedProperties();
		}

		public int ResetParticleMaterial(SBatchDesc batchDesc)
		{
			int idx = -1;
			if (batchDesc != null)
			{
				PKFxCustomMaterialInfo customRule = FindCustomMaterialInfo(batchDesc);
				if (customRule != null)
				{
					AssetDatabase.RemoveObjectFromAsset(customRule);
					idx = m_CustomMaterials.IndexOf(customRule);
					m_CustomMaterials.RemoveAt(idx);
				}
			}
			m_Materials[batchDesc.MaterialIdx] = PKFxSettings.MaterialFactory.EditorResolveMaterial(batchDesc, this);
			AssetDatabase.SaveAssets();
			return idx;
		}

		public void ResetAllCustomMaterials(SerializedProperty mats, SerializedProperty CustomMats)
		{
			UnityEngine.Object[] data = AssetDatabase.LoadAllAssetsAtPath("Assets/" + PKFxSettings.UnityPackFxPath + "/" + AssetVirtualPath + ".asset");
			foreach (UnityEngine.Object asset in data)
			{
				PKFxCustomMaterialInfo info = asset as PKFxCustomMaterialInfo;
				if (info != null)
				{
					foreach (SBatchDesc desc in m_RendererDescs)
					{
						if (desc.MaterialIdx == info.m_InternalId)
						{
							m_Materials[info.m_InternalId] = PKFxSettings.MaterialFactory.EditorGetDefaultMaterial(desc, this);
							AssetDatabase.RemoveObjectFromAsset(info);
							mats.GetArrayElementAtIndex(info.m_InternalId).objectReferenceValue = PKFxSettings.MaterialFactory.EditorResolveMaterial(desc, this);
							break;
						}

					}
				}
			}
			AssetDatabase.SaveAssets();
			m_CustomMaterials.Clear();

			CustomMats.ClearArray();

		}

		public void AddRenderer(ERendererType type, SPopcornRendererDesc renderer, int idx)
		{
			SBatchDesc batch = new SBatchDesc(type, renderer, idx);
			m_RendererDescs.Add(batch);

			Material mat = null;
			RemoveOutdatedCustomMaterials(batch);
			PKFxCustomMaterialInfo curMat = FindCustomMaterialInfo(batch);
			if (curMat != null && curMat.m_CustomMaterial != null)
			{
				curMat.SetMaterialKeywords(batch, curMat.m_CustomMaterial);
				curMat.BindMaterialProperties(batch, curMat.m_CustomMaterial, this);
				mat = curMat.m_CustomMaterial;
			}
			else
				mat = PKFxSettings.MaterialFactory.EditorResolveMaterial(batch, this);

			while (idx >= m_Materials.Count)
				m_Materials.Add(null);
			if (mat == null)
			{
				string assetFullPath = "Assets" + PKFxSettings.UnityPackFxPath + "/" + AssetVirtualPath + ".asset";
				PKFxEffectAsset asset = (PKFxEffectAsset)AssetDatabase.LoadAssetAtPath(assetFullPath, typeof(PKFxEffectAsset));
				Debug.LogError("Can't find a material for asset " + AssetVirtualPath + "in following batch desc" + m_RendererDescs[m_RendererDescs.Count - 1].m_GeneratedName, asset);
				
				return;
			}
			m_Materials[idx] = mat;
		}

		public void AddRenderer(ERendererType type, SMeshRendererDesc renderer, int idx)
		{
			SBatchDesc batch = new SBatchDesc(renderer, idx);
			m_RendererDescs.Add(batch);

			Material mat = null;
			RemoveOutdatedCustomMaterials(batch);
			PKFxCustomMaterialInfo curMat = FindCustomMaterialInfo(batch);
			if (curMat != null && curMat.m_CustomMaterial != null)
			{
				curMat.SetMaterialKeywords(batch, curMat.m_CustomMaterial);
				curMat.BindMaterialProperties(batch, curMat.m_CustomMaterial, this);
				mat = curMat.m_CustomMaterial;
			}
			else
				mat = PKFxSettings.MaterialFactory.EditorResolveMaterial(batch, this);

			while (idx >= m_Materials.Count)
				m_Materials.Add(null);
			if (mat == null)
			{
				string assetFullPath = "Assets" + PKFxSettings.UnityPackFxPath + "/" + AssetVirtualPath + ".asset";
				PKFxEffectAsset asset = (PKFxEffectAsset)AssetDatabase.LoadAssetAtPath(assetFullPath, typeof(PKFxEffectAsset));
				Debug.LogError("Can't find a material for asset " + AssetVirtualPath + " in following batch desc" + m_RendererDescs[m_RendererDescs.Count - 1].m_GeneratedName, asset);
				return;
			}
			m_Materials[idx] = mat;
		}

		public void AddEvent(SNativeEventDesc eventdesc)
		{
			m_EventDescs.Add(new SEventDesc(eventdesc));
		}
#endif

		public void ComputePropertiesHash()
		{
			string allAttributesString = "";
			string allSamplersString = "";
			string allRenderersString = "";
			string allEventsString = "";

			foreach (AttributeDesc attrDesc in m_AttributeDescs)
			{
				allAttributesString += attrDesc.ToString();
			}
			foreach (SamplerDesc smpDesc in m_SamplerDescs)
			{
				allSamplersString += smpDesc.ToString();
			}
			foreach (SBatchDesc batchDesc in m_RendererDescs)
			{
				allRenderersString += batchDesc.ToString();
			}
			foreach (SEventDesc EventDesc in m_EventDescs)
			{
				allEventsString += EventDesc.ToString();
			}
			m_AttributeDescsHash = allAttributesString.GetHashCode();
			m_SamplerDescsHash = allSamplersString.GetHashCode();
			m_EventDescsHash = allEventsString.GetHashCode();
			m_RendererDescsHash = allRenderersString.GetHashCode();
		}

		public int AttributeIdFromName(string attribName)
		{
			for (int i = 0; i < m_AttributeDescs.Count; ++i)
			{
				if (m_AttributeDescs[i].m_Name == attribName)
				{
					return i;
				}
			}
			Debug.LogError("[PopcornFX] Cannot find the attribute " + attribName, this);
			return -1;
		}

		public override void Clean()
		{
			base.Clean();
			m_AttributeDescs = new List<AttributeDesc>();
			m_SamplerDescs = new List<SamplerDesc>();
			m_RendererDescs = new List<SBatchDesc>();
			m_Materials = new List<Material>();
			m_Dependencies = new List<DependencyDesc>();
			m_EventDescs = new List<SEventDesc>();
			m_AttributeDescsHash = 0;
			m_SamplerDescsHash = 0;
			m_RendererDescsHash = 0;
			m_UsesMeshRenderer = false;
		}
	}
}
