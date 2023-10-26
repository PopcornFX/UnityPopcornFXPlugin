//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using UnityEngine.Events;
using System;
using System.IO;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PopcornFX
{
	[Serializable]public class PkFxUserEventCallback
	{
		public SEventDesc										m_Desc;
		public UnityEvent<string, List<SPopcornFXPayloadData>>	m_Event;

		public int m_Key;

		public PkFxUserEventCallback(SEventDesc desc)
		{
			m_Desc = desc;
			m_Key = -1;
		}
	}

	public partial class PKFxEmitter : MonoBehaviour
	{
		// Static
		static public Dictionary<int, PKFxEmitter> g_ListEffects = new Dictionary<int, PKFxEmitter>();
		static public List<PKFxEmitter> g_PlayingEffectsToUpdate = new List<PKFxEmitter>();

		static public string UpdateAssetPathIFN(string assetPath)
		{
			string newAssetPath = assetPath;
			if (!string.IsNullOrEmpty(newAssetPath))
			{
				int split = newAssetPath.IndexOf("PackFx");
				// We remove the leading "PackFx" folder from the asset path (the user can now name the folder as they want):
				newAssetPath = (split == 0) ? newAssetPath.Substring(7) : newAssetPath;
				if (Path.GetExtension(newAssetPath) == ".pkfx")
				{
					newAssetPath += ".asset";
				}
			}
			return newAssetPath;
		}

		//Instance
		private int m_FXGUID = -1;

		private bool m_AskedToStart = false;
		private bool m_Awaked = false;
		private bool m_IsPlaying = false;
		private bool m_IsStopped = true;

		public bool m_PlayOnStart = true;
		public bool m_TriggerAndForget = false;
		public bool m_KillEffectOnDestroy = false;

		[SerializeField] private float m_PreWarm = 0;

		//Used in editor and startup
		[SerializeField] private List<PkFxUserEventCallback>					m_Events;
		[SerializeField] private List<PKFxEffectAsset.AttributeDesc>			m_FxAttributesDesc = null;

		[SerializeField] private List<PKFxRaiseEventCallbackData>				m_DelayedCallbacks;
		[SerializeField] private Dictionary<int, PKFxRaiseEventCallbackData>	m_Callbacks;
		public int m_EventDescsHash = 0;

		[SerializeField] private PKFxAttributesContainer m_AttributesContainer = null;
		[SerializeField] private int m_FxAttributesDescHash = 0;
		public int FXAttributesDescHash { get { return m_FxAttributesDescHash; } }
		[SerializeField] private SGenericTypedValueContainer[] m_FxAttributesStartValues = null; // New version of the attributes

		public List<Sampler> m_FxSamplersList = new List<Sampler>();
		[SerializeField] private int m_FxSamplersDescHash = 0;
		public int FXSamplersDescHash { get { return m_FxSamplersDescHash; } }

		public delegate void OnFxStoppedDelegate(PKFxEmitter component);
		public OnFxStoppedDelegate m_OnFxStopped = null;

		public UnityEngine.Object m_BoundFx;

		[SerializeField] internal PKFxEffectAsset	m_FxAsset;
		[SerializeField] internal String			m_FxName;

		//----------------------------------------------------------------------------
		public int FXGUID { get { return m_FXGUID; } }
		public float PreWarm { get { return m_PreWarm; } set { m_PreWarm = value; } }
		public bool Alive { get { return m_IsPlaying; } }
		public bool Stopped { get { return m_IsStopped; } }

		public int SamplerCount { get { return m_FxSamplersList.Count; } }
		public int AttributeCount { get { return m_FxAttributesDesc.Count; } }

		public PKFxEffectAsset EffectAsset
		{
			get { return m_FxAsset; }
		}
		public string EffectName { get { return m_FxName; }  set { m_FxName = value; } }

		internal PKFxAttributesContainer AttributesContainer { get { return m_AttributesContainer; } }

		#region Monobehaviour Methods
		//----------------------------------------------------------------------------
		// MonoBehaviour Methods
		//----------------------------------------------------------------------------
		void Awake()
		{
			PKFxManager.StartupPopcorn(false);

			m_DelayedCallbacks = new List<PKFxRaiseEventCallbackData>();
			m_Callbacks = new Dictionary<int, PKFxRaiseEventCallbackData>();
			if (m_Events == null)
				m_Events = new List<PkFxUserEventCallback>();
			m_Awaked = true;
		}

		//----------------------------------------------------------------------------

		void Start()
		{
			PreloadIFN();
			if ((m_PlayOnStart || m_AskedToStart) && !Alive)
				StartEffect();
			m_AskedToStart = false;

		}

		//----------------------------------------------------------------------------

		
		void OnDrawGizmos()
		{
			//To Keep implemented and empty, to have the gizmo drawn via the .meta informations
		}

		//----------------------------------------------------------------------------

		public void OnDestroy()
		{
			// When the component is destroyed before the end of the effect the m_OnFxStopped callback is not called
			m_OnFxStopped = null;
			if (m_IsStopped)
			{
				// If the effect is stopped, we manually call the on OnFxStopPlaying
				OnFxStopPlaying(m_FXGUID);
			}
			else if (m_IsPlaying && m_FXGUID != -1)
			{
				// Here we manually kill or terminate the effect:
				if (m_KillEffectOnDestroy)
					KillEffect();
				else
					TerminateEffect();
			}
			if (m_Callbacks != null)
				m_Callbacks.Clear();
			if (m_Events != null)
				m_Events.Clear();
			if (m_DelayedCallbacks != null)
				m_DelayedCallbacks.Clear();
		}
		#endregion

		#region Effect lifecycle management
		//----------------------------------------------------------------------------
		// Effect control
		//----------------------------------------------------------------------------

		public void PreloadIFN()
		{
			if (m_FXGUID == -1 && m_FxAsset != null)
			{
				PreloadEffectFromAsset(m_FxAsset);
			}
		}

		public void InstantiateIFN()
		{
			if (m_FXGUID == -1 && m_FxAsset != null)
			{
				// Preload the FX:
				PreloadIFN();

				// Instantiate the FX:
				m_FXGUID = CreateEffect(m_FxAsset, transform);

				if (m_FXGUID != -1)
				{
					// Add the effect to the list of all the effects components:
					if (g_ListEffects.ContainsKey(m_FXGUID))
						g_ListEffects[m_FXGUID] = this;
					else
						g_ListEffects.Add(m_FXGUID, this);
					// We are checking if the FX component is up to date (mix and match the attributes and samplers IFN):
					UpdateEffectAsset(m_FxAsset, false, true);

					EffectUnregisterAllEvents();

					m_AttributesContainer = new PKFxAttributesContainer(m_FxAsset, m_FXGUID);
					// Setup the default value for the attributes:
					if (m_FxAttributesStartValues != null && m_FxAttributesStartValues.Length != 0)
					{
						m_AttributesContainer.SetAllAttributes(m_FxAttributesStartValues);
					}
					// Setup the default values for the samplers:
					UpdateSamplers(true);

				}
				else
				{
					Debug.LogWarning("[PopcornFX] Could not instantiate the effect", this);
				}
			}
		}

		//----------------------------------------------------------------------------

		public void StartEffect()
		{
			StartEffectWithDt(0);
		}

		//----------------------------------------------------------------------------

		public void StartEffectWithDt(float dt)
		{
			if (!m_Awaked)
			{
				m_AskedToStart = true;
				return;
			}

			// instantiate the FX IFN:
			InstantiateIFN();

			if (m_FXGUID == -1)
			{
				Debug.LogWarning("[PopcornFX] Could not start effect as the effect was not instantiated", this);
			}
			else if (Alive)
			{
				Debug.LogWarning("[PopcornFX] Effect already started", this);
			}
			else
			{
				// Set the m_IsPlaying to true:
				SetPlayingState(true);  // Do that BEFORE the call to StartEffect as it can call inline the OnFxStopped delegate
				// Start the FX in the native plugin:
				PKFxManager.StartEffect(m_FXGUID, dt, m_PreWarm);

				foreach (var it in m_DelayedCallbacks)
				{
					int unityKey = ("" + m_FXGUID + it.m_EventName).GetHashCode();
					if (m_Callbacks.ContainsKey(unityKey) == false)
					{
						m_Callbacks.Add(unityKey, it);
					}
					else
					{
						m_Callbacks[unityKey].m_Cb += it.m_Cb;
						m_Callbacks[unityKey].m_Count += 1;
					}
				}

				foreach (var it in m_Callbacks)
				{
					if (!PKFxManager.EffectRegisterEvent(m_FXGUID, it.Value.m_EventName, it.Key))
						Debug.LogError("[PopcornFX]  Error Registering Event :" + it.Value.m_EventName + " in " + EffectName);
				}

				foreach (PkFxUserEventCallback eventCallback in m_Events)
				{
					if (eventCallback.m_Event != null && eventCallback.m_Event.GetPersistentEventCount() > 0)
					{
						int unityKey = ("" + m_FXGUID + eventCallback.m_Desc.m_Name).GetHashCode();
						if (!PKFxManager.EffectRegisterEvent(m_FXGUID, eventCallback.m_Desc.m_Name, unityKey))
						{
							Debug.LogError("[PopcornFX]  Error Registering Event :" + eventCallback.m_Desc.m_Name + " in " + EffectName);
						}
						eventCallback.m_Key = unityKey;
					}
				}
			}
		}

		//----------------------------------------------------------------------------

		public void TerminateEffect()
		{
			TerminateEffectWithDt(0);
		}

		//----------------------------------------------------------------------------

		public void TerminateEffectWithDt(float dt)
		{
			if (!m_IsPlaying || m_FXGUID == -1)
				Debug.LogWarning("[PopcornFX] Trying to terminate an effect that is not playing", this);
			else if (m_IsStopped)
				Debug.LogWarning("[PopcornFX] Trying to terminate an effect that was stopped", this);
			else
			{
				PKFxManager.TerminateEffect(m_FXGUID, dt);
				OnFxStopPlaying(m_FXGUID);
			}
		}

		//----------------------------------------------------------------------------

		public void StopEffect()
		{
			StopEffectWithDt(0);
		}

		public void StopEffectWithDt(float dt)
		{
			if (!m_IsPlaying || m_FXGUID == -1)
				Debug.LogWarning("[PopcornFX] Trying to stop an effect that is not playing", this);
			else
			{
				PKFxManager.StopEffect(m_FXGUID, dt);
				m_IsStopped = true;
			}
		}

		//----------------------------------------------------------------------------

		public void OnFxStopPlaying(int guid)
		{
			Debug.Assert(guid == m_FXGUID);
			if (m_OnFxStopped != null)
				m_OnFxStopped(this);			// Call the OnFxStopped callback
			SetPlayingState(false);				// Remove from the currently playing FXs list
			g_ListEffects.Remove(m_FXGUID);		// Remove the effect from the FXs list
			m_AttributesContainer = null;		// The attribute container is only valid while the effect is instantiated
			m_FXGUID = -1;						// Set the fxId to -1
		}

		//----------------------------------------------------------------------------

		public void KillEffect()
		{
			KillEffectWithDt(0);
		}

		//----------------------------------------------------------------------------

		public void KillEffectWithDt(float dt)
		{
			if (!m_IsPlaying || m_FXGUID == -1)
				Debug.LogWarning("[PopcornFX] Trying to kill an effect that is not playing", this);
			else
			{
				PKFxManager.KillEffect(m_FXGUID, dt);
				OnFxStopPlaying(m_FXGUID);
			}
		}
		#endregion

		#region Events
		//----------------------------------------------------------------------------
		// Event Callback
		//----------------------------------------------------------------------------

		public bool EffectRegisterEvent(string eventName, PKFxRaiseEventCallback callback)
		{
			if (m_FXGUID >= 0)
			{
				string hashString = "" + m_FXGUID + eventName;
				int unityKey = hashString.GetHashCode();

				if (m_Callbacks.ContainsKey(unityKey) == false)
				{
					PKFxRaiseEventCallbackData data = new PKFxRaiseEventCallbackData(eventName, callback);
					m_Callbacks.Add(unityKey, data);
				}
				else
				{
					m_Callbacks[unityKey].m_Cb += callback;
					m_Callbacks[unityKey].m_Count += 1;
				}
				return PKFxManager.EffectRegisterEvent(m_FXGUID, eventName, unityKey);
			}
			else
			{
				PKFxRaiseEventCallbackData data = new PKFxRaiseEventCallbackData(eventName, callback);
				m_DelayedCallbacks.Add(data);
			}
			return true;
		}

		public bool EffectUnregisterEvent(string eventName, PKFxRaiseEventCallback callback)
		{
			string hashString = "" + m_FXGUID + eventName;

			int unityKey = hashString.GetHashCode();

			if (m_Callbacks.ContainsKey(unityKey))
			{
				m_Callbacks[unityKey].m_Cb -= callback;
				m_Callbacks[unityKey].m_Count -= 1;

				if (m_Callbacks[unityKey].m_Count == 0)
					m_Callbacks.Remove(unityKey);
			}
			return PKFxManager.EffectUnregisterEvent(m_FXGUID, eventName, unityKey);
		}

		public void EffectUnregisterAllEvents()
		{
			m_Callbacks.Clear();
			PKFxManager.EffectUnregisterAllEvents(m_FXGUID);
		}

		public void TriggerRegisteredEvent(int unityKey, string eventName, List<SPopcornFXPayloadData> payloads)
		{
			if (m_Callbacks.ContainsKey(unityKey) == true)
				m_Callbacks[unityKey].m_Cb(eventName, payloads);
			foreach (PkFxUserEventCallback eventCallback in m_Events)
			{
				if (eventCallback.m_Key == unityKey && eventCallback.m_Event.GetPersistentEventCount() > 0)
				{
					eventCallback.m_Event.Invoke(eventName, payloads);
				}
			}
		}

		#endregion Events

		public void UpdateEffectTransforms()
		{
			UpdateTransformEffect(m_FXGUID, transform);
		}

		//----------------------------------------------------------------------------

		// Will not actually change the asset, just retrieve the attributes and samplers from it
		public bool UpdateEffectAsset(PKFxEffectAsset updatedAsset, bool resetAllAttributes, bool mismatchAttribsWarning = false)
		{
			m_FxAsset = updatedAsset;
			bool hasChanged = false;

			if (updatedAsset == null)
			{
				if (!string.IsNullOrEmpty(EffectName))
				{
					hasChanged = true;
				}
				EffectName = "";
				ClearAllAttributesAndSamplers();
			}
			else
			{
				if (EffectName != updatedAsset.AssetVirtualPath)
				{
					EffectName = updatedAsset.AssetVirtualPath;
					hasChanged = true;
				}
				if (updatedAsset.m_AttributeDescsHash != m_FxAttributesDescHash || resetAllAttributes)
				{
					if (mismatchAttribsWarning)
					{
						Debug.LogWarning("[PopcornFX] Performance warning: the attributes are not up to date on this PkFX component (" + m_FxAsset.AssetVirtualPath + ")", this);
						Debug.LogWarning("[PopcornFX] Right click on the Project explorer > PopcornFX > Update PKFxFX References", this);
					}
					LoadAttributes(updatedAsset.m_AttributeDescs, resetAllAttributes);
					m_FxAttributesDescHash = updatedAsset.m_AttributeDescsHash;
					hasChanged = true;
				}
				if (updatedAsset.m_SamplerDescsHash != m_FxSamplersDescHash || resetAllAttributes)
				{
					if (mismatchAttribsWarning)
					{
						Debug.LogWarning("[PopcornFX] Performance warning: the samplers are not up to date on this PkFX component (" + m_FxAsset.AssetVirtualPath + ")", this);
						Debug.LogWarning("[PopcornFX] Right click on the Project explorer > PopcornFX > Update PKFxFX References", this);
					}
					LoadSamplers(updatedAsset.m_SamplerDescs, resetAllAttributes);
					m_FxSamplersDescHash = updatedAsset.m_SamplerDescsHash;
					hasChanged = true;
				}
				if (updatedAsset.m_EventDescsHash != m_EventDescsHash || resetAllAttributes)
				{
					LoadEvents(updatedAsset.m_EventDescs, resetAllAttributes);
					m_EventDescsHash = updatedAsset.m_EventDescsHash;
					hasChanged = true;
				}
			}
			return hasChanged;
		}

		public void ClearAllAttributesAndSamplers()
		{
			m_FxAttributesStartValues = null;
			if (m_FxAttributesDesc != null)
				m_FxAttributesDesc = null;
			m_FxAttributesDescHash = -1;

			if (m_FxSamplersList != null)
				m_FxSamplersList.Clear();
			m_FxSamplersDescHash = -1;

			if (m_Events != null)
				m_Events.Clear();
			m_EventDescsHash = -1;
		}

		private Sampler GetSamplerFromDesc(SamplerDesc desc)
		{
			foreach (Sampler attr in m_FxSamplersList)
			{
				if (attr.m_Descriptor.m_Name == desc.m_Name &&
					attr.m_Descriptor.m_Type == desc.m_Type &&
					attr.m_Descriptor.m_UsageFlags == desc.m_UsageFlags)
					return attr;
			}
			return null;
		}

		private void LoadSamplers(List<SamplerDesc> samplersFromPkfxFile, bool resetAllToDefault)
		{
			List<Sampler> newList = new List<Sampler>();

			foreach (SamplerDesc desc in samplersFromPkfxFile)
			{
				Sampler sampler = resetAllToDefault ? null : GetSamplerFromDesc(desc);

				if (sampler == null)
				{
					newList.Add(new Sampler(desc));
				}
				else
				{
					sampler.UpdateDefaultCurveValueIFN(desc);
					newList.Add(sampler);
				}
			}
			m_FxSamplersList = newList;
		}
		
		//----------------------------------------------------------------------------

		public void UpdateSamplers(bool forceUpdate)
		{
			for (int i = 0; i < m_FxSamplersList.Count; i++)
			{
				int samplerId = i;
				Sampler curSampler = m_FxSamplersList[i];

				// for the sampler shape:
				if (curSampler.m_Descriptor.m_Type == ESamplerType.SamplerShape)
				{
					// Sampler shape mesh:
					if (curSampler.m_ShapeType == Sampler.EShapeType.MeshShape ||
						curSampler.m_ShapeType == Sampler.EShapeType.MeshFilterShape ||
						curSampler.m_ShapeType == Sampler.EShapeType.SkinnedMeshShape ||
						curSampler.m_ShapeType == Sampler.EShapeType.BakedMeshShape)
					{
						bool isSkinned = curSampler.m_ShapeType == Sampler.EShapeType.SkinnedMeshShape ||
											(curSampler.m_ShapeType == Sampler.EShapeType.BakedMeshShape && curSampler.m_SkinnedMeshRenderer != null);
						// Upload the mesh data to the PopcornFX plugin IFN:
						if (forceUpdate || curSampler.m_WasModified)
						{
							if (curSampler.m_BakedMesh == null)
							{
								// Setup the mesh from skinned mesh renderer or mesh filter:
								if (curSampler.m_SkinnedMeshRenderer != null)
								{
									curSampler.m_Mesh = curSampler.m_SkinnedMeshRenderer.sharedMesh;
								}
								if (curSampler.m_MeshFilter != null)
								{
									curSampler.m_Mesh = curSampler.m_MeshFilter.sharedMesh;
								}
							}
							if (curSampler.m_BakedMesh != null)
							{
								SMeshSamplerBaked samplerBaked = new SMeshSamplerBaked();

								samplerBaked.m_SubmeshId = curSampler.m_BakedMeshSubMeshId;
								samplerBaked.m_IsSkinned = isSkinned;
								if (isSkinned)
									samplerBaked.m_BonesCount = curSampler.m_SkinnedMeshRenderer.bones.Length;
								else
									samplerBaked.m_BonesCount = 0;
								PKFxManagerImpl.SetMeshSamplerBaked(m_FXGUID, samplerId, samplerBaked, curSampler.m_BakedMesh, curSampler.Dimensions, curSampler.m_AsynchronousSet);
							}
							else if (curSampler.m_Mesh != null) // Setup the mesh here...
							{
								int usageFlags = m_FxAsset.m_SamplerDescs[samplerId].m_UsageFlags;
								// Feed the mesh to the PopcornFX runtime:
								IntPtr meshToFillPtr = GetMeshToFill(curSampler.m_Mesh, usageFlags, isSkinned);
								UpdateMeshToFill(meshToFillPtr, curSampler.m_Mesh);
								PKFxManager.SetMeshSampler(m_FXGUID, samplerId, meshToFillPtr, curSampler.Dimensions, curSampler.m_AsynchronousSet);
							}
							else
							{
								PKFxManager.SetDefaultSampler(m_FXGUID, samplerId);
							}
						}
						// Update the skinning:
						if (isSkinned && curSampler.m_SkinnedMeshRenderer != null)
						{
							if (UpdateMeshBones(m_FXGUID, samplerId, curSampler.m_SkinnedMeshRenderer))
							{
								// Update skinning should be async:
								PKFxManager.BeginUpdateSamplerSkinning(m_FXGUID, samplerId, Time.deltaTime);
								PKFxManager.EndUpdateSamplerSkinning(m_FXGUID, samplerId);
							}
						}
					}
					else
					{
						if (forceUpdate || curSampler.m_WasModified)
						{
							if (curSampler.m_ShapeType != Sampler.EShapeType.ShapeUnsupported)
							{
								PKFxManager.SetSamplerShape(m_FXGUID, samplerId, curSampler.m_ShapeType, curSampler.Dimensions);
							}
							else
							{
								PKFxManager.SetDefaultSampler(m_FXGUID, samplerId);
							}
						}
					}
					Matrix4x4 shapeTransform;

					if (curSampler.m_ShapeTransformReference != null)
						shapeTransform = curSampler.m_ShapeTransformReference.localToWorldMatrix;
					else
						shapeTransform = curSampler.m_ShapeTransform.transform;

					PKFxManager.SetSamplerShapeTransform(m_FXGUID, samplerId, shapeTransform);
				}
				else if (curSampler.m_Descriptor.m_Type == ESamplerType.SamplerCurve)
				{
					if (forceUpdate || curSampler.m_WasModified)
					{
						if (curSampler.m_CurveIsOverride)
						{
							int keyPointsCount = curSampler.m_CurvesTimeKeys == null ? 0 : curSampler.m_CurvesTimeKeys.Length;
							int curvesCount = curSampler.m_CurvesArray == null ? 0 : curSampler.m_CurvesArray.Length;
							IntPtr curveToFill = PKFxManager.GetCurveSamplerToFill(keyPointsCount, curvesCount);
							UpdateCurveToFill(curveToFill, curSampler.m_CurvesArray, curSampler.m_CurvesTimeKeys);
							PKFxManager.SetCurveSampler(m_FXGUID, samplerId, curveToFill);
						}
						else
						{
							PKFxManager.SetDefaultSampler(m_FXGUID, samplerId);
						}
					}
				}
				else if (curSampler.m_Descriptor.m_Type == ESamplerType.SamplerImage)
				{
					if (forceUpdate || curSampler.m_WasModified)
					{
						if (curSampler.m_Texture != null)
						{
							IntPtr textureToFill = PKFxTextureUtils.GetAndUpdateTextureToFill(curSampler.m_Texture, curSampler.m_TextureTexcoordMode);
							PKFxManager.SetTextureSampler(m_FXGUID, samplerId, textureToFill);
						}
						else
						{
							PKFxManager.SetDefaultSampler(m_FXGUID, samplerId);
						}
					}
				}
				else if (curSampler.m_Descriptor.m_Type == ESamplerType.SamplerText)
				{
					if (forceUpdate || curSampler.m_WasModified)
					{
						if (!string.IsNullOrEmpty(curSampler.m_Text))
						{
							PKFxManager.SetTextSampler(m_FXGUID, samplerId, curSampler.m_Text);
						}
						else
						{
							PKFxManager.SetDefaultSampler(m_FXGUID, samplerId);
						}
					}
				}
				// The sampler is now up to date:
				curSampler.m_WasModified = false;
			}
		}

		//----------------------------------------------------------------------------

		public void UpdatePkFXComponent(PKFxEffectAsset updatedAsset, bool updateAsset)
		{
			// Just in case we reset those to false: they should never be true when the editor is not running
			m_IsPlaying = false;
			m_IsStopped = false;
			if (updateAsset)
			{
				// Update effect asset:
				UpdateEffectAsset(updatedAsset, false);
			}
		}

		public void SetSamplerSafe(Sampler sampler)
		{
			int samplerId = m_FxSamplersList.FindIndex(x => x.m_Descriptor == sampler.m_Descriptor);

			if (samplerId != -1)
			{
				m_FxSamplersList[samplerId] = sampler;
			}
		}


		public void UpdateAttributes()
		{
			if (m_AttributesContainer != null)
				m_AttributesContainer.UpdateAttributes();
		}

		public int GetAttributeId(string name, EAttributeType type)
		{
			int attribId = 0;

			foreach (PKFxEffectAsset.AttributeDesc curDesc in m_FxAttributesDesc)
			{
				if (curDesc.m_Name == name && curDesc.m_Type == type)
					return attribId;
				++attribId;
			}
			return -1;
		}

		public EAttributeType GetAttributeType(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return EAttributeType.Bool;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			return attributeDesc.m_Type;
		}

		public string GetAttributeName(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return string.Empty;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			return attributeDesc.m_Name;
		}

		#region GetAttribute

		#region GetAttributeFloat

		public float GetAttributeFloat(string name)
		{
			return GetAttributeFloat(GetAttributeId(name, EAttributeType.Float));
		}

		public Vector2 GetAttributeFloat2(string name)
		{
			return GetAttributeFloat2(GetAttributeId(name, EAttributeType.Float2));
		}

		public Vector3 GetAttributeFloat3(string name)
		{
			return GetAttributeFloat3(GetAttributeId(name, EAttributeType.Float3));
		}

		public Vector4 GetAttributeFloat4(string name)
		{
			return GetAttributeFloat4(GetAttributeId(name, EAttributeType.Float4));
		}

		public float GetAttributeFloat(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return 0.0f;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Float)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return 0.0f;
			}
			return m_FxAttributesStartValues[attributeIdx * 4 + 0].f1;
		}

		public Vector2 GetAttributeFloat2(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return Vector2.zero;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Float2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return Vector2.zero;
			}
			return new Vector2(m_FxAttributesStartValues[attributeIdx * 4 + 0].f1,
								m_FxAttributesStartValues[attributeIdx * 4 + 1].f1);
		}

		public Vector3 GetAttributeFloat3(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return Vector3.zero;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Float3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return Vector3.zero;
			}
			return new Vector3(m_FxAttributesStartValues[attributeIdx * 4 + 0].f1,
								m_FxAttributesStartValues[attributeIdx * 4 + 1].f1,
								m_FxAttributesStartValues[attributeIdx * 4 + 2].f1);
		}

		public Vector4 GetAttributeFloat4(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return Vector4.zero;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Float4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return Vector4.zero;
			}
			return new Vector4(m_FxAttributesStartValues[attributeIdx * 4 + 0].f1,
								m_FxAttributesStartValues[attributeIdx * 4 + 1].f1,
								m_FxAttributesStartValues[attributeIdx * 4 + 2].f1,
								m_FxAttributesStartValues[attributeIdx * 4 + 3].f1);
		}


		public Tuple<float, float> GetAttributeFloatRange(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new Tuple<float, float>(0, 0);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Float)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new Tuple<float, float>(0, 0);
			}

			return new Tuple<float, float>(attributeDesc.HasMin() ? attributeDesc.m_MinValue.x.f1 : float.MinValue,
											attributeDesc.HasMax() ? attributeDesc.m_MaxValue.x.f1 : float.MaxValue);
		}

		public Tuple<Vector2, Vector2> GetAttributeFloat2Range(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new Tuple<Vector2, Vector2>(Vector2.zero, Vector2.zero);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Float2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new Tuple<Vector2, Vector2>(Vector2.zero, Vector2.zero);
			}

			Vector2 min = attributeDesc.HasMin() ? new Vector2(attributeDesc.m_MinValue.x.f1, attributeDesc.m_MinValue.y.f1)
												 : new Vector2(float.MinValue, float.MinValue);
			Vector2 max = attributeDesc.HasMax() ? new Vector2(attributeDesc.m_MaxValue.x.f1, attributeDesc.m_MaxValue.y.f1)
												 : new Vector2(float.MaxValue, float.MaxValue);
			return new Tuple<Vector2, Vector2>(min, max);
		}

		public Tuple<Vector3, Vector3> GetAttributeFloat3Range(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new Tuple<Vector3, Vector3>(Vector3.zero, Vector3.zero);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Float3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new Tuple<Vector3, Vector3>(Vector3.zero, Vector3.zero);
			}

			Vector3 min = attributeDesc.HasMin() ? new Vector3(attributeDesc.m_MinValue.x.f1, attributeDesc.m_MinValue.y.f1, attributeDesc.m_MinValue.z.f1)
												 : new Vector3(float.MinValue, float.MinValue, float.MinValue);
			Vector3 max = attributeDesc.HasMax() ? new Vector3(attributeDesc.m_MaxValue.x.f1, attributeDesc.m_MaxValue.y.f1, attributeDesc.m_MaxValue.z.f1)
												 : new Vector3(float.MaxValue, float.MaxValue, float.MaxValue);
			return new Tuple<Vector3, Vector3>(min, max);
		}

		public Tuple<Vector4, Vector4> GetAttributeFloat4Range(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new Tuple<Vector4, Vector4>(Vector4.zero, Vector4.zero);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Float4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new Tuple<Vector4, Vector4>(Vector4.zero, Vector4.zero);
			}
			Vector4 min = attributeDesc.HasMin() ? new Vector4(attributeDesc.m_MinValue.x.f1, attributeDesc.m_MinValue.y.f1, attributeDesc.m_MinValue.z.f1, attributeDesc.m_MinValue.w.f1)
												 : new Vector4(float.MinValue, float.MinValue, float.MinValue, float.MinValue);
			Vector4 max = attributeDesc.HasMax() ? new Vector4(attributeDesc.m_MaxValue.x.f1, attributeDesc.m_MaxValue.y.f1, attributeDesc.m_MaxValue.z.f1, attributeDesc.m_MaxValue.w.f1)
												 : new Vector4(float.MaxValue, float.MaxValue, float.MaxValue, float.MaxValue);
			return new Tuple<Vector4, Vector4>(min, max);
		}

		#endregion

		#region GetAttributeInt

		public int GetAttributeInt(string name)
		{
			return GetAttributeInt(GetAttributeId(name, EAttributeType.Int));
		}

		public Vector2Int GetAttributeInt2(string name)
		{
			return GetAttributeInt2(GetAttributeId(name, EAttributeType.Int2));
		}

		public Vector3Int GetAttributeInt3(string name)
		{
			return GetAttributeInt3(GetAttributeId(name, EAttributeType.Int3));
		}

		public PopcornFX.Vector4Int GetAttributeInt4(string name)
		{
			return GetAttributeInt4(GetAttributeId(name, EAttributeType.Int4));
		}

		public int GetAttributeInt(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return 0;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Int)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return 0;
			}
			return m_FxAttributesStartValues[attributeIdx * 4 + 0].i1;
		}

		public Vector2Int GetAttributeInt2(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return Vector2Int.zero;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Int2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return Vector2Int.zero;
			}
			return new Vector2Int(m_FxAttributesStartValues[attributeIdx * 4 + 0].i1,
								m_FxAttributesStartValues[attributeIdx * 4 + 1].i1);
		}

		public Vector3Int GetAttributeInt3(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return Vector3Int.zero;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Int3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return Vector3Int.zero;
			}
			return new Vector3Int(m_FxAttributesStartValues[attributeIdx * 4 + 0].i1,
								m_FxAttributesStartValues[attributeIdx * 4 + 1].i1,
								m_FxAttributesStartValues[attributeIdx * 4 + 2].i1);
		}

		public PopcornFX.Vector4Int GetAttributeInt4(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new PopcornFX.Vector4Int(0);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Int4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new PopcornFX.Vector4Int(0);
			}
			return new PopcornFX.Vector4Int(m_FxAttributesStartValues[attributeIdx * 4 + 0].i1,
											m_FxAttributesStartValues[attributeIdx * 4 + 1].i1,
											m_FxAttributesStartValues[attributeIdx * 4 + 2].i1,
											m_FxAttributesStartValues[attributeIdx * 4 + 3].i1);
		}

		public Tuple<int, int> GetAttributeIntRange(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new Tuple<int, int>(0, 0);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Int)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new Tuple<int, int>(0, 0);
			}

			return new Tuple<int, int>(attributeDesc.HasMin() ? attributeDesc.m_MinValue.x.i1 : int.MinValue,
										attributeDesc.HasMax() ? attributeDesc.m_MaxValue.x.i1 : int.MaxValue);
		}

		public Tuple<Vector2Int, Vector2Int> GetAttributeInt2Range(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new Tuple<Vector2Int, Vector2Int>(Vector2Int.zero, Vector2Int.zero);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Int2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new Tuple<Vector2Int, Vector2Int>(Vector2Int.zero, Vector2Int.zero);
			}

			Vector2Int min = attributeDesc.HasMin() ? new Vector2Int(attributeDesc.m_MinValue.x.i1, attributeDesc.m_MinValue.y.i1)
													: new Vector2Int(int.MinValue, int.MinValue);
			Vector2Int max = attributeDesc.HasMax() ? new Vector2Int(attributeDesc.m_MaxValue.x.i1, attributeDesc.m_MaxValue.y.i1)
													: new Vector2Int(int.MaxValue, int.MaxValue);
			return new Tuple<Vector2Int, Vector2Int>(min, max);
		}

		public Tuple<Vector3Int, Vector3Int> GetAttributeInt3Range(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new Tuple<Vector3Int, Vector3Int>(Vector3Int.zero, Vector3Int.zero);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Int3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new Tuple<Vector3Int, Vector3Int>(Vector3Int.zero, Vector3Int.zero);
			}

			Vector3Int min = attributeDesc.HasMin() ? new Vector3Int(attributeDesc.m_MinValue.x.i1, attributeDesc.m_MinValue.y.i1, attributeDesc.m_MinValue.z.i1)
													: new Vector3Int(int.MinValue, int.MinValue, int.MinValue);
			Vector3Int max = attributeDesc.HasMax() ? new Vector3Int(attributeDesc.m_MaxValue.x.i1, attributeDesc.m_MaxValue.y.i1, attributeDesc.m_MaxValue.z.i1)
													: new Vector3Int(int.MaxValue, int.MaxValue, int.MaxValue);
			return new Tuple<Vector3Int, Vector3Int>(min, max);
		}

		public Tuple<Vector4Int, Vector4Int> GetAttributeInt4Range(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new Tuple<Vector4Int, Vector4Int>(new Vector4Int(0), new Vector4Int(0));
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Int4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new Tuple<Vector4Int, Vector4Int>(new Vector4Int(0), new Vector4Int(0));
			}

			Vector4Int min = attributeDesc.HasMin() ? new Vector4Int(attributeDesc.m_MinValue.x.i1, attributeDesc.m_MinValue.y.i1, attributeDesc.m_MinValue.z.i1, attributeDesc.m_MinValue.w.i1)
													: new Vector4Int(int.MinValue, int.MinValue, int.MinValue, int.MinValue);
			Vector4Int max = attributeDesc.HasMax() ? new Vector4Int(attributeDesc.m_MaxValue.x.i1, attributeDesc.m_MaxValue.y.i1, attributeDesc.m_MaxValue.z.i1, attributeDesc.m_MaxValue.w.i1)
													: new Vector4Int(int.MaxValue, int.MaxValue, int.MaxValue, int.MaxValue);
			return new Tuple<Vector4Int, Vector4Int>(min, max);
		}

		#endregion

		#region GetAttributeBool

		public bool GetAttributeBool(string name)
		{
			return GetAttributeBool(GetAttributeId(name, EAttributeType.Bool));
		}

		public PopcornFX.Vector2Bool GetAttributeBool2(string name)
		{
			return GetAttributeBool2(GetAttributeId(name, EAttributeType.Bool2));
		}

		public PopcornFX.Vector3Bool GetAttributeBool3(string name)
		{
			return GetAttributeBool3(GetAttributeId(name, EAttributeType.Bool3));
		}

		public PopcornFX.Vector4Bool GetAttributeBool4(string name)
		{
			return GetAttributeBool4(GetAttributeId(name, EAttributeType.Bool4));
		}

		public bool GetAttributeBool(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return false;
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Bool)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return false;
			}
			return m_FxAttributesStartValues[attributeIdx * 4 + 0].b1 != 0;
		}

		public PopcornFX.Vector2Bool GetAttributeBool2(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new PopcornFX.Vector2Bool(false);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Bool2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new PopcornFX.Vector2Bool(false);
			}
			SGenericTypedValueContainer current = m_FxAttributesStartValues[attributeIdx * 4];
			return new PopcornFX.Vector2Bool(current.b1 != 0, current.b2 != 0);
		}

		public PopcornFX.Vector3Bool GetAttributeBool3(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new PopcornFX.Vector3Bool(false);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Bool3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new PopcornFX.Vector3Bool(false);
			}

			SGenericTypedValueContainer current = m_FxAttributesStartValues[attributeIdx * 4];
			return new PopcornFX.Vector3Bool(current.b1 != 0, current.b2 != 0, current.b3 != 0);
		}

		public PopcornFX.Vector4Bool GetAttributeBool4(int attributeIdx)
		{
			if (attributeIdx < 0 || attributeIdx >= m_FxAttributesDesc.Count)
			{
				Debug.LogError("[PopcornFX] Get Attribute with invalid ID.");
				return new PopcornFX.Vector4Bool(false);
			}

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attributeIdx];
			if (attributeDesc.m_Type != EAttributeType.Bool4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return new PopcornFX.Vector4Bool(false);
			}
			SGenericTypedValueContainer current = m_FxAttributesStartValues[attributeIdx * 4];
			return new PopcornFX.Vector4Bool(current.b1 != 0, current.b2 != 0, current.b3 != 0, current.b4 != 0);
		}

		#endregion

		#endregion

		#region SetAttribute

		#region SetAttributeSafe from name
		public void SetAttributeSafe(string name, float valueX)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX);
			}
		}

		public void SetAttributeSafe(string name, float valueX, float valueY)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX, valueY);
			}
		}

		public void SetAttributeSafe(string name, float valueX, float valueY, float valueZ)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX, valueY, valueZ);
			}
		}

		public void SetAttributeSafe(string name, float valueX, float valueY, float valueZ, float valueW)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX, valueY, valueZ, valueW);
			}
		}

		public void SetAttributeSafe(string name, bool valueX)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX);
			}
		}

		public void SetAttributeSafe(string name, bool valueX, bool valueY)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX, valueY);
			}
		}

		public void SetAttributeSafe(string name, bool valueX, bool valueY, bool valueZ)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX, valueY, valueZ);
			}
		}

		public void SetAttributeSafe(string name, bool valueX, bool valueY, bool valueZ, bool valueW)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX, valueY, valueZ, valueW);
			}
		}

		public void SetAttributeSafe(string name, int valueX)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX);
			}
		}

		public void SetAttributeSafe(string name, int valueX, int valueY)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX, valueY);
			}
		}

		public void SetAttributeSafe(string name, int valueX, int valueY, int valueZ)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX, valueY, valueZ);
			}
		}

		public void SetAttributeSafe(string name, int valueX, int valueY, int valueZ, int valueW)
		{
			int attribId = m_FxAsset.AttributeIdFromName(name);

			if (attribId != -1)
			{
				SetAttributeSafe(attribId, valueX, valueY, valueZ, valueW);
			}
		}

		#endregion

		#region SetAttributeSafe from ID

		public void SetAttributeSafe(int attribId, float valueX)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Float)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.f1, valueX);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.f1, valueX);
			}
			SetAttributeUnsafe(attribId, valueX);
		}

		public void SetAttributeSafe(int attribId, float valueX, float valueY)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Float2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.f1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.f1, valueY);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.f1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.f1, valueY);
			}
			SetAttributeUnsafe(attribId, valueX, valueY);
		}

		public void SetAttributeSafe(int attribId, float valueX, float valueY, float valueZ)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Float3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.f1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.f1, valueY);
				valueZ = Mathf.Max(attributeDesc.m_MinValue.z.f1, valueZ);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.f1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.f1, valueY);
				valueZ = Mathf.Min(attributeDesc.m_MaxValue.z.f1, valueZ);
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ);
		}

		public void SetAttributeSafe(int attribId, float valueX, float valueY, float valueZ, float valueW)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Float4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.f1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.f1, valueY);
				valueZ = Mathf.Max(attributeDesc.m_MinValue.z.f1, valueZ);
				valueW = Mathf.Max(attributeDesc.m_MinValue.w.f1, valueW);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.f1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.f1, valueY);
				valueZ = Mathf.Min(attributeDesc.m_MaxValue.z.f1, valueZ);
				valueW = Mathf.Min(attributeDesc.m_MaxValue.w.f1, valueW);
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ, valueW);
		}

		public void SetAttributeSafe(int attribID, Vector4 value)
		{
			SetAttributeSafe(attribID, value.x, value.y, value.z, value.w);
		}

		public void SetAttributeSafe(int attribId, bool valueX)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Bool)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			SetAttributeUnsafe(attribId, valueX);
		}

		public void SetAttributeSafe(int attribId, bool valueX, bool valueY)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Bool2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			SetAttributeUnsafe(attribId, valueX, valueY);
		}

		public void SetAttributeSafe(int attribId, bool valueX, bool valueY, bool valueZ)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Bool3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ);
		}

		public void SetAttributeSafe(int attribId, bool valueX, bool valueY, bool valueZ, bool valueW)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Bool4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ, valueW);
		}

		public void SetAttributeSafe(int attribId, int valueX)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Int)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.i1, valueX);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.i1, valueX);
			}
			SetAttributeUnsafe(attribId, valueX);
		}

		public void SetAttributeSafe(int attribId, int valueX, int valueY)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Int2)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.i1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.i1, valueY);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.i1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.i1, valueY);
			}
			SetAttributeUnsafe(attribId, valueX, valueY);
		}

		public void SetAttributeSafe(int attribId, int valueX, int valueY, int valueZ)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Int3)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.i1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.i1, valueY);
				valueZ = Mathf.Max(attributeDesc.m_MinValue.z.i1, valueZ);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.i1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.i1, valueY);
				valueZ = Mathf.Min(attributeDesc.m_MaxValue.z.i1, valueZ);
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ);
		}

		public void SetAttributeSafe(int attribId, int valueX, int valueY, int valueZ, int valueW)
		{
			if (m_FxAsset.m_AttributeDescs == null || attribId < 0 || attribId >= m_FxAsset.m_AttributeDescs.Count)
				return;

			PKFxEffectAsset.AttributeDesc attributeDesc = m_FxAsset.m_AttributeDescs[attribId];

			if (attributeDesc.m_Type != EAttributeType.Int4)
			{
				Debug.LogError("[PopcornFX] Type mismatch for the attribute", this);
				return;
			}
			if (attributeDesc.HasMin())
			{
				valueX = Mathf.Max(attributeDesc.m_MinValue.x.i1, valueX);
				valueY = Mathf.Max(attributeDesc.m_MinValue.y.i1, valueY);
				valueZ = Mathf.Max(attributeDesc.m_MinValue.z.i1, valueZ);
				valueW = Mathf.Max(attributeDesc.m_MinValue.w.i1, valueW);
			}
			if (attributeDesc.HasMax())
			{
				valueX = Mathf.Min(attributeDesc.m_MaxValue.x.i1, valueX);
				valueY = Mathf.Min(attributeDesc.m_MaxValue.y.i1, valueY);
				valueZ = Mathf.Min(attributeDesc.m_MaxValue.z.i1, valueZ);
				valueW = Mathf.Min(attributeDesc.m_MaxValue.w.i1, valueW);
			}
			SetAttributeUnsafe(attribId, valueX, valueY, valueZ, valueW);
		}

		#endregion

		#region SetAttributeUnsafe

		public void SetAttributeUnsafe(int attribId, float valueX)
		{
			m_FxAttributesStartValues[attribId * 4 + 0].f1 = valueX;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, valueX);
			}
		}

		public void SetAttributeUnsafe(int attribId, float valueX, float valueY)
		{
			m_FxAttributesStartValues[attribId * 4 + 0].f1 = valueX;
			m_FxAttributesStartValues[attribId * 4 + 1].f1 = valueY;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, valueX, valueY);
			}
		}

		public void SetAttributeUnsafe(int attribId, float valueX, float valueY, float valueZ)
		{
			m_FxAttributesStartValues[attribId * 4 + 0].f1 = valueX;
			m_FxAttributesStartValues[attribId * 4 + 1].f1 = valueY;
			m_FxAttributesStartValues[attribId * 4 + 2].f1 = valueZ;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, valueX, valueY, valueZ);
			}
		}

		public void SetAttributeUnsafe(int attribId, float valueX, float valueY, float valueZ, float valueW)
		{
			m_FxAttributesStartValues[attribId * 4 + 0].f1 = valueX;
			m_FxAttributesStartValues[attribId * 4 + 1].f1 = valueY;
			m_FxAttributesStartValues[attribId * 4 + 2].f1 = valueZ;
			m_FxAttributesStartValues[attribId * 4 + 3].f1 = valueW;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, valueX, valueY, valueZ, valueW);
			}
		}

		public void SetAttributeUnsafe(int attribId, Vector4 value)
		{
			SetAttributeUnsafe(attribId, value.x, value.y, value.z, value.w);
		}

		public void SetAttributeUnsafe(int attribId, bool valueX)
		{
			m_FxAttributesStartValues[attribId * 4].b1 = valueX ? (byte)255 : (byte)0;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, m_FxAttributesStartValues[attribId * 4].i1);
			}
		}

		public void SetAttributeUnsafe(int attribId, bool valueX, bool valueY)
		{
			m_FxAttributesStartValues[attribId * 4].b1 = valueX ? (byte)255 : (byte)0;
			m_FxAttributesStartValues[attribId * 4].b2 = valueY ? (byte)255 : (byte)0;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, m_FxAttributesStartValues[attribId * 4].i1);
			}
		}

		public void SetAttributeUnsafe(int attribId, bool valueX, bool valueY, bool valueZ)
		{
			m_FxAttributesStartValues[attribId * 4].b1 = valueX ? (byte)255 : (byte)0;
			m_FxAttributesStartValues[attribId * 4].b2 = valueY ? (byte)255 : (byte)0;
			m_FxAttributesStartValues[attribId * 4].b3 = valueZ ? (byte)255 : (byte)0;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, m_FxAttributesStartValues[attribId * 4].i1);
			}
		}

		public void SetAttributeUnsafe(int attribId, bool valueX, bool valueY, bool valueZ, bool valueW)
		{
			m_FxAttributesStartValues[attribId * 4].b1 = valueX ? (byte)255 : (byte)0;
			m_FxAttributesStartValues[attribId * 4].b2 = valueY ? (byte)255 : (byte)0;
			m_FxAttributesStartValues[attribId * 4].b3 = valueZ ? (byte)255 : (byte)0;
			m_FxAttributesStartValues[attribId * 4].b4 = valueW ? (byte)255 : (byte)0;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, m_FxAttributesStartValues[attribId * 4].i1);
			}
		}

		public void SetAttributeUnsafe(int attribId, int valueX)
		{
			m_FxAttributesStartValues[attribId * 4 + 0].i1 = valueX;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, valueX);
			}
		}

		public void SetAttributeUnsafe(int attribId, int valueX, int valueY)
		{
			m_FxAttributesStartValues[attribId * 4 + 0].i1 = valueX;
			m_FxAttributesStartValues[attribId * 4 + 1].i1 = valueY;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, valueX, valueY);
			}
		}

		public void SetAttributeUnsafe(int attribId, int valueX, int valueY, int valueZ)
		{
			m_FxAttributesStartValues[attribId * 4 + 0].i1 = valueX;
			m_FxAttributesStartValues[attribId * 4 + 1].i1 = valueY;
			m_FxAttributesStartValues[attribId * 4 + 2].i1 = valueZ;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, valueX, valueY, valueZ);
			}
		}

		public void SetAttributeUnsafe(int attribId, int valueX, int valueY, int valueZ, int valueW)
		{
			m_FxAttributesStartValues[attribId * 4 + 0].i1 = valueX;
			m_FxAttributesStartValues[attribId * 4 + 1].i1 = valueY;
			m_FxAttributesStartValues[attribId * 4 + 2].i1 = valueZ;
			m_FxAttributesStartValues[attribId * 4 + 3].i1 = valueW;
			if (m_AttributesContainer != null)
			{
				m_AttributesContainer.SetAttributeUnsafe(attribId, valueX, valueY, valueZ, valueW);
			}
		}

		#endregion

#endregion
	}
}
