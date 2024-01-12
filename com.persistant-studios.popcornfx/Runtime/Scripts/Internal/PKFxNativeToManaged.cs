//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
using UnityEngine;
using UnityEngine.Rendering;
using System;
using System.Runtime.InteropServices;
using System.IO;
using AOT;
using System.Collections.Generic;
#if UNITY_EDITOR
using UnityEditor;
#endif

//----------------------------------------------------------------------------
namespace PopcornFX
{
	public delegate void AssetChangeCallback(ref SAssetChangesDesc assetChange);

	[StructLayout(LayoutKind.Sequential)]
	public struct SRaycastPack
	{
		public IntPtr m_OutNormals;
		public IntPtr m_OutPositions;
		public IntPtr m_OutDistances;
	};

	// Attributes:
	[StructLayout(LayoutKind.Sequential)]
	public struct SNativeAttributeDesc
	{
		public EAttributeType		m_AttributeType;
		public EAttributeSemantic	m_AttributeSemantic;
		public EAttributeDropMode	m_AttributeDropMode;
		public int m_MinMaxFlag;

		public IntPtr m_AttributeName;
		public IntPtr m_Description;
		public IntPtr m_DropNameList;

		public SAttribContainer_Vector4 m_DefaultValue;
		public SAttribContainer_Vector4 m_MinValue;
		public SAttribContainer_Vector4 m_MaxValue;
	};

	// Samplers:
	[StructLayout(LayoutKind.Sequential)]
	public struct SNativeSamplerDesc
	{
		public ESamplerType m_SamplerType;
		public IntPtr m_SamplerName;
		public IntPtr m_Description;
		public int m_SamplerUsageFlags;

		public Quaternion m_ShapeRotation; // Rotation quaternion
		public Vector3 m_ShapePosition;

		public int m_CurveDimension;
		public int m_CurveKeyCount;
		public IntPtr m_CurveTimes;
		public IntPtr m_CurveFloatValues;
		public IntPtr m_CurveFloatTangents;
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct SRenderingFeatureLitDesc
	{
		public IntPtr m_NormalMap;
		public IntPtr m_RoughMetalMap;

		public int m_CastShadows;

		public float m_NormalBendingFactor;
		public float m_Roughness;
		public float m_Metalness;
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct SRenderingFeatureVATDesc
	{
		public IntPtr m_PositionMap;
		public IntPtr m_NormalMap;
		public IntPtr m_ColorMap;
		public IntPtr m_RotationMap;
		public int m_NumFrames;
		public int m_PackedData;
		public Vector4 m_Color;
		public Vector2 m_BoundsPivot;
		public int m_NormalizedData;
		public Vector2 m_BoundsPosition;
		public int m_PadToPowerOf2;
		public Vector2 m_PaddedRatio;
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct SRenderingFeatureSkeletalAnimDesc
	{
		public IntPtr m_AnimTextureMap;
		public int m_TextureResolX;
		public int m_TextureResolY;
		public int m_AnimCount;
		public int m_UseBoneScale;
		public Vector3 m_TranslationBoundsMin;
		public Vector3 m_TranslationBoundsMax;
		public Vector3 m_ScaleBoundsMin;
		public Vector3 m_ScaleBoundsMax;
	};

	// Create Renderers:
	// Billboards and ribbons:
	[StructLayout(LayoutKind.Sequential)]
	public struct SPopcornRendererDesc
	{
		public int m_ShaderVariationFlags;
		public EBlendMode m_BlendMode;
		public int m_RotateTexture;

		public IntPtr m_DiffuseMap;
		public IntPtr m_EmissiveMap;
		public IntPtr m_AlphaRemap;
		public IntPtr m_DiffuseRampMap;
		public IntPtr m_EmissiveRampMap;
		public float m_InvSoftnessDistance;
		public float m_AlphaClipThreshold;
		public int m_TransformUVs_RGBOnly;

		public EBillboardMode m_BillboardMode;
		public int m_DrawOrder;

		public IntPtr m_LitRendering;

		public int	m_CameraId;
		public int	m_UID;
	};

	// Meshes:
	[StructLayout(LayoutKind.Sequential)]
	public struct SMeshRendererDesc
	{
		public IntPtr m_MeshAsset;
		public int m_ShaderVariationFlags;
		public EBlendMode m_BlendMode;
		public int m_HasMeshAtlas;

		public IntPtr m_DiffuseMap;
		public IntPtr m_EmissiveMap;
		public IntPtr m_AlphaRemap;
		public IntPtr m_DiffuseRampMap;
		public IntPtr m_EmissiveRampMap;
		public float m_InvSofnessDistance;
		public float m_AlphaClipThreshold;
		public bool	m_DoubleSided;

		public IntPtr m_LitRendering;
		public IntPtr m_VatRendering;
		public IntPtr m_SkeletalAnim;

		public int		m_TextureAtlasCount;
		public IntPtr	m_TextureAtlas;
		public int		m_UID;
		public int		m_DrawOrder;
	};


	[StructLayout(LayoutKind.Sequential)]
	public struct SNativeEventDesc
	{
		public int		m_Slot;
		public IntPtr	m_EventName;
	}

	// Bounds
	[StructLayout(LayoutKind.Sequential)]
	public struct SUpdateRendererBounds
	{
		public float m_MinX;
		public float m_MinY;
		public float m_MinZ;
		public float m_MaxX;
		public float m_MaxY;
		public float m_MaxZ;
	};

	// Effect info:
	[StructLayout(LayoutKind.Sequential)]
	public struct SRetrieveRendererInfo
	{
		public IntPtr m_IsIndex32;

		public IntPtr m_VertexBufferSize;
		public IntPtr m_IndexBufferSize;
		public IntPtr m_InfoBSize;
		public IntPtr m_AtlasesBSize;
		public IntPtr m_IndirectArgsParticleCountMultiplier;

		public IntPtr m_VBHandler;
		public IntPtr m_IBHandler;
		public IntPtr m_InfoBHandler;
		public IntPtr m_AtlasesBHandler;
		public IntPtr m_IndirectArgsBHandler;
		public IntPtr m_UseComputeBuffers;

		public IntPtr m_HasCustomMaterial;
		public IntPtr m_CustomMatID;
	};

	//Play sound
	[StructLayout(LayoutKind.Sequential)]
	public struct SNativeSoundDescriptor
	{
		public int m_ChannelGroup;
		public IntPtr m_Path;
		public IntPtr m_EventStart;
		public IntPtr m_EventStop;
		public float m_WorldPositionX;
		public float m_WorldPositionY;
		public float m_WorldPositionZ;
		public float m_Volume;
		public float m_StartTimeOffsetInSeconds;
		public float m_PlayTimeInSeconds;
		public int m_UserData;
	};

	public enum EAssetChangesType : int
	{
		Undefined = 0,
		Add,
		Remove,
		Update,
		Rename
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct SAssetChangesDesc
	{
		public string m_Path;
		public string m_PathOld;
		public int m_Type;
	}

	[StructLayout(LayoutKind.Sequential)]
	public struct SAssetImportError
	{
		public string m_Path;
		public string m_ErrorMessage;
	}

	public enum EUseInfoFlag : int
	{
		// Renderers:
		IsLinearTextureRenderer = (1 << 0),
		IsTextureRenderer = (1 << 1),
		IsMeshRenderer = (1 << 2),
		// Samplers:
		IsMeshSampler = (1 << 3),
		IsTextureSampler = (1 << 4),

		IsVatTexture = (1 << 5),

		IsThumbnail = (1 << 6),
		IsAnimatedThumbnail = (1 << 7),
	};

	public enum EPrintConsoleType : int
	{
		ErrorMessage,
		WarningMessage,
		InfoMessage
	};

	[StructLayout(LayoutKind.Sequential)]
	public struct SPopcornFXPayloadDesc
	{
		public IntPtr m_PayloadName;
		public uint m_PayloadType;
		public uint m_StartOffset;
		public uint m_Length;
	};

	internal partial class PKFxManagerImpl : object
	{
		// Native to managed delegates:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnResourceLoad(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnResourceWrite(IntPtr delegatePtr);

		// Not in this file:
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnRaycastStart(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnRaycastEnd(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnRaycastPack(IntPtr delegatePtr);
		//Fx lifetime functions
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnFxStopped(IntPtr delegatePtr);
		//Events
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnRaiseEvent(IntPtr delegatePtr);

		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnAudioWaveformData(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnAudioSpectrumData(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetupNewBillboardRenderer(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetupNewRibbonRenderer(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetupNewMeshRenderer(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetupNewTriangleRenderer(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnResizeRenderer(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetParticleCount(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetRendererActive(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetMeshInstancesCount(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetMeshInstancesBuffer(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetLightsBuffer(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnSetSoundsBuffer(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnRetrieveCustomMaterialInfo(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnRetrieveRendererBufferInfo(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnUpdateRendererBounds(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnGetMeshCount(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnGetMeshLODsCount(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnGetMeshBounds(IntPtr delegatePtr);

#if UNITY_EDITOR
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnEffectDependencyFound(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnEffectAttributeFound(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnEffectRendererFound(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnEffectRendererLink(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnEffectEventFound(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnEffectSamplerFound(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnGetEffectInfo(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnEffectAboutToBake(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnAssetChange(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnImportError(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnGetAllAssetPath(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnRendererNamesAdded(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnPrintUnityConsole(IntPtr delegatePtr);
		[DllImport(kPopcornPluginName, CallingConvention = kCallingConvention)]
		public static extern void SetDelegateOnPkkgExtracted(IntPtr delegatePtr);
#endif
		//----------------------------------------------------------------------------

		private delegate ulong ResourceLoadCallback(IntPtr path, ref IntPtr handler);

		[MonoPInvokeCallback(typeof(ResourceLoadCallback))]
		public static ulong OnResourceLoad(IntPtr pathHandler, ref IntPtr handler)
		{
			string path = Marshal.PtrToStringAnsi(pathHandler);
			string ext = Path.GetExtension(path);

			if (m_CurrentlyBuildAsset != null) // preferered path for loading dependencies.
			{
				if (ext == ".pkfx" && m_CurrentlyBuildAsset.AssetVirtualPath == path)
				{ 
					var pinned = PinnedData.PinAsset(m_CurrentlyBuildAsset);
					handler = pinned;
					return (ulong)m_CurrentlyBuildAsset.m_Data.Length;
				}
				foreach (PKFxEffectAsset.DependencyDesc dependency in m_CurrentlyBuildAsset.m_Dependencies)
				{
					if (dependency.m_Path == path)
					{
						Texture2D		depTex = dependency.m_Object as Texture2D;
						PKFxAsset		pkfxDependency = dependency.m_Object as PKFxAsset;
						if (pkfxDependency)
						{
							var pinned = PinnedData.PinAsset(pkfxDependency);
							handler = pinned;
							return (ulong)pkfxDependency.m_Data.Length;
						}
						else if (depTex)
						{
							ulong len = PinnedData.PinTexture(depTex, ref handler);
							return len;
						}
						// Need to handle vectorfield files when implemented
					}
				}
			}
			// failing that, check if we have it in the runtime resources
			if (m_RuntimeAssets != null && m_RuntimeAssets.Count > 0)
			{
				foreach (GameObject go in m_RuntimeAssets)
				{
					PKFxRuntimeMeshAsset asset = go.GetComponent<PKFxRuntimeMeshAsset>();
					if (asset.m_AssetVirtualPath == path)
					{
						var pinned = PinnedData.PinAsset(asset);
						handler = pinned;
						return (ulong)asset.m_Data.Length;
					}
				}
			}
			// On last resort: Load via general array of dependencies.
			if (m_Dependencies != null)
			{
				if (PKFxUtils.ArrayContains(s_CustomFileTypes, ext))
				{
					foreach (PKFxAsset asset in m_Dependencies)
					{
						string virtualPath = asset.AssetVirtualPath;
						if (virtualPath == path)
						{
							var pinned = PinnedData.PinAsset(asset);
							handler = pinned;

							Debug.LogWarning("[PopcornFX] Perfomance issue: " + path + " found in general dependencies.");
							return (ulong)asset.m_Data.Length;
						}
					}
				}
				else if (PKFxUtils.ArrayContains(s_TexFileTypes, ext))
				{
					foreach (var t in m_TexDependencies)
					{
						if (t.m_Path == path)
						{
							ulong len = PinnedData.PinTexture(t.m_Texture, ref handler);
							
							Debug.LogWarning("[PopcornFX] Perfomance issue: " + path + " found in general dependencies.");
							return len;
						}
					}
				}
			}
			Debug.LogError("[PopcornFX] " + path + " not found in dependencies.");
			return 0;
		}

		//----------------------------------------------------------------------------

		private delegate ulong ResourceWriteCallback(IntPtr path, IntPtr data, ulong offset, ulong size);

		[MonoPInvokeCallback(typeof(ResourceWriteCallback))]
		public static ulong OnResourceWrite(IntPtr pathHandler, IntPtr data, ulong offset, ulong size)
		{
			if (data == IntPtr.Zero)
				return 0;
			string path = Marshal.PtrToStringAnsi(pathHandler);
			if (!Application.isPlaying)
			{
#if UNITY_EDITOR
				string dir = Path.GetDirectoryName(path);

				Directory.CreateDirectory(dir);

				FileStream fs = new FileStream(path, FileMode.Create);
				BinaryWriter bw = new BinaryWriter(fs);
				byte[] managedArray = new byte[size];

				Marshal.Copy(data, managedArray, 0, (int)size);
				bw.Write(managedArray);

				bw.Close();
				fs.Close();

				UnityEditor.AssetDatabase.Refresh();

				Debug.Log("[PopcornFX] Wrote asset to PackFx : " + path);
				return size;
#endif
			}
			else
			{
				Debug.Assert(m_IsStarted == true);
				GameObject go = new GameObject();
				PKFxRuntimeMeshAsset newMesh = go.AddComponent<PKFxRuntimeMeshAsset>();
				newMesh.m_AssetVirtualPath = path;
				go.name = path;
				newMesh.m_Data = new byte[size];
				Marshal.Copy(data, newMesh.m_Data, 0, (int)size);
				if (m_RuntimeAssetsRoot == null)
				{
					m_RuntimeAssetsRoot = new GameObject("PopcornFX Runtime Assets");
					m_RuntimeAssetsRoot.transform.position = Vector3.zero;
					m_RuntimeAssetsRoot.isStatic = true;
					UnityEngine.Object.DontDestroyOnLoad(m_RuntimeAssetsRoot);
				}
				go.transform.parent = m_RuntimeAssetsRoot.transform;
				m_RuntimeAssets.Add(go);
				return size;
			}
#if !UNITY_EDITOR
		return 0;
#endif
		}

		//----------------------------------------------------------------------------

		public delegate void EffectDependencyFoundCallback(IntPtr dependencyPath, int useInfoFlags);

		[MonoPInvokeCallback(typeof(EffectDependencyFoundCallback))]
		private static void OnEffectDependencyFound(IntPtr dependencyPath, int useInfoFlags)
		{
			if (dependencyPath == IntPtr.Zero)
				return;

			PKFxEffectAsset.DependencyDesc depDesc = new PKFxEffectAsset.DependencyDesc();

			depDesc.m_Path = Marshal.PtrToStringAnsi(dependencyPath);
			depDesc.m_UsageFlags = useInfoFlags;

			bool isMeshRenderer = (useInfoFlags & (int)EUseInfoFlag.IsMeshRenderer) != 0;
			if (isMeshRenderer && Path.GetExtension(depDesc.m_Path).CompareTo(".pkmm") == 0)
				depDesc.m_Path = Path.ChangeExtension(depDesc.m_Path, ".fbx");
			else
				depDesc.m_UsageFlags &= ~(int)EUseInfoFlag.IsMeshRenderer;

			bool isMeshSampler = (useInfoFlags & (int)EUseInfoFlag.IsMeshSampler) != 0;
			if (isMeshSampler && Path.GetExtension(depDesc.m_Path).ToLower().CompareTo(".fbx") == 0)
				depDesc.m_Path = Path.ChangeExtension(depDesc.m_Path, ".pkmm");
			else
				depDesc.m_UsageFlags &= ~(int)EUseInfoFlag.IsMeshSampler;

			bool isLinearTexture = (useInfoFlags & (int)EUseInfoFlag.IsLinearTextureRenderer) != 0;

			if (isLinearTexture)
			{
				depDesc.m_Path = Path.GetDirectoryName(depDesc.m_Path) + "/" + Path.GetFileNameWithoutExtension(depDesc.m_Path) + "_linear" + Path.GetExtension(depDesc.m_Path);
				depDesc.m_Path = depDesc.m_Path.Replace('\\', '/');
			}

			int idx = m_CurrentlyImportedAsset.m_Dependencies.FindIndex(x => x.m_Path == depDesc.m_Path);

			if (idx < 0)
				m_CurrentlyImportedAsset.m_Dependencies.Add(depDesc);
			else
				m_CurrentlyImportedAsset.m_Dependencies[idx].m_UsageFlags |= depDesc.m_UsageFlags;
		}

		//----------------------------------------------------------------------------

		public delegate void EffectAttributeFoundCallback(IntPtr attributeDescPtr);

		[MonoPInvokeCallback(typeof(EffectAttributeFoundCallback))]
		private static void OnEffectAttributeFound(IntPtr attributeDescPtr)
		{
			if (attributeDescPtr == IntPtr.Zero)
				return;

			SNativeAttributeDesc nativeAttrDesc = (SNativeAttributeDesc)Marshal.PtrToStructure(attributeDescPtr, typeof(SNativeAttributeDesc));
			PKFxEffectAsset.AttributeDesc attrDesc = new PKFxEffectAsset.AttributeDesc(nativeAttrDesc);

			m_CurrentlyImportedAsset.m_AttributeDescs.Add(attrDesc);
		}

		//----------------------------------------------------------------------------

		public delegate void EffectSamplerFoundCallback(IntPtr attributeDescPtr);

		[MonoPInvokeCallback(typeof(EffectSamplerFoundCallback))]
		private static void OnEffectSamplerFound(IntPtr samplerDescPtr)
		{
			if (samplerDescPtr == IntPtr.Zero)
				return;

			SNativeSamplerDesc nativeSamplerDesc = (SNativeSamplerDesc)Marshal.PtrToStructure(samplerDescPtr, typeof(SNativeSamplerDesc));
			SamplerDesc samplerDesc = new SamplerDesc(nativeSamplerDesc);

			m_CurrentlyImportedAsset.m_SamplerDescs.Add(samplerDesc);
		}

		//----------------------------------------------------------------------------

		public delegate void EffectRendererFoundCallback(IntPtr RendererDescPtr, int type, int idx);

		[MonoPInvokeCallback(typeof(EffectRendererFoundCallback))]
		private static void OnEffectRendererFound(IntPtr RendererDescPtr, int type, int idx)
		{
#if UNITY_EDITOR
			if (RendererDescPtr == IntPtr.Zero)
				return;

			ERendererType etype = ERendererType.Billboard;
			if (type == 0)
				etype = ERendererType.Billboard;
			else if (type == 1)
				etype = ERendererType.Ribbon;
			else if (type == 2)
				etype = ERendererType.Mesh;
			else if (type == 3)
				etype = ERendererType.Triangle;
			else if (type == 4)
				etype = ERendererType.Decal;
			else if (type == 5)
				etype = ERendererType.Light;
			else if (type == 6)
				etype = ERendererType.Sound;

			if (etype == ERendererType.Mesh)
			{
				unsafe
				{
					SMeshRendererDesc* nativeRendererDesc = (SMeshRendererDesc*)RendererDescPtr.ToPointer();
					m_CurrentlyImportedAsset.AddRenderer(etype, *nativeRendererDesc, idx);
				}
			}
			else
			{
				unsafe
				{
					SPopcornRendererDesc* nativeRendererDesc = (SPopcornRendererDesc*)RendererDescPtr.ToPointer();
					m_CurrentlyImportedAsset.AddRenderer(etype, *nativeRendererDesc, idx);
				}
			}
#endif
		}

		//----------------------------------------------------------------------------

		public delegate void EffectRendererLinkCallback(int GlobalIdx, string qualityLevel, int UID);

		[MonoPInvokeCallback(typeof(EffectRendererLinkCallback))]
		private static void OnEffectRendererLink(int globalIdx, string qualityLevel, int UID)
		{
#if UNITY_EDITOR
			m_CurrentlyImportedAsset.LinkRenderer(globalIdx, qualityLevel, UID);
#endif
		}
		
		//----------------------------------------------------------------------------
		public delegate void EffectEventFoundCallback(IntPtr eventDescPtr);

		[MonoPInvokeCallback(typeof(EffectEventFoundCallback))]
		private static void OnEffectEventFound(IntPtr EventDescPtr)
		{
#if UNITY_EDITOR
			if (EventDescPtr == IntPtr.Zero)
				return;

			unsafe
			{
				SNativeEventDesc* nativeEventDesc = (SNativeEventDesc*)EventDescPtr.ToPointer();
				m_CurrentlyImportedAsset.AddEvent(*nativeEventDesc);

			}
#endif
		}

		//----------------------------------------------------------------------------

		public delegate void GetEffectInfoCallback(int requiresGameThreadCollect);

		[MonoPInvokeCallback(typeof(GetEffectInfoCallback))]
		private static void OnGetEffectInfo(int requiresGameThreadCollect)
		{
			m_CurrentlyImportedAsset.m_RequiresGameThreadCollect = requiresGameThreadCollect != 0;
		}

		//----------------------------------------------------------------------------

		private delegate void FxCallback(int guid);

		[MonoPInvokeCallback(typeof(FxCallback))]
		public static void OnFxStopped(int guid)
		{
			PKFxEmitter component;

			if (PKFxEmitter.g_ListEffects.TryGetValue(guid, out component))
			{
				component.OnFxStopPlaying(guid);
			}
		}


		private delegate void RaiseEventCallback(int guid, int key, string eventName, uint payloadCount, IntPtr payloadDescs, IntPtr payloadValues);

		[MonoPInvokeCallback(typeof(RaiseEventCallback))]
		public static void OnRaiseEvent(int guid, int key, string eventName, uint payloadCount, IntPtr payloadDescs, IntPtr payloadValues)
		{
			PKFxEmitter component;

			List<SPopcornFXPayloadData> payloads = new List<SPopcornFXPayloadData>();

			if (PKFxEmitter.g_ListEffects.TryGetValue(guid, out component))
			{
				unsafe
				{
					SPopcornFXPayloadDesc* payloadDescStart = (SPopcornFXPayloadDesc*)payloadDescs.ToPointer();
					SPayloadContainer_Vector4* payloadContainerStart = (SPayloadContainer_Vector4*)payloadValues.ToPointer();

					for (int i = 0; i < payloadCount; ++i)
					{
						uint length = payloadDescStart[i].m_Length;
						uint startOffset = payloadDescStart[i].m_StartOffset;

						SPopcornFXPayloadData data = new SPopcornFXPayloadData((int)length,
																			   Marshal.PtrToStringAnsi(payloadDescStart[i].m_PayloadName),
																			   (EAttributeType)payloadDescStart[i].m_PayloadType);

						for (int j = 0; j < length; ++j)
							data.Values.Add(payloadContainerStart[startOffset + j]);
						payloads.Add(data);
					}
				}
				component.TriggerRegisteredEvent(key, eventName, payloads);
			}
		}

		//----------------------------------------------------------------------------

		public delegate IntPtr AudioCallback(IntPtr channelName, IntPtr nbSamples);

		[MonoPInvokeCallback(typeof(AudioCallback))]
		public static IntPtr OnAudioWaveformData(IntPtr channelName, IntPtr nbSamples)
		{
			AudioListener.GetOutputData(m_Samples, 0);
			return m_SamplesHandle.AddrOfPinnedObject();
		}

		//----------------------------------------------------------------------------

		[MonoPInvokeCallback(typeof(AudioCallback))]
		public static IntPtr OnAudioSpectrumData(IntPtr channelName, IntPtr nbSamples)
		{
			AudioListener.GetSpectrumData(m_Samples, 0, FFTWindow.Rectangular);
			// Last value filled by Unity seems wrong...
			m_Samples[1023] = m_Samples[1022];
			return m_SamplesHandle.AddrOfPinnedObject();
		}

		//----------------------------------------------------------------------------
		// Billboard

		private delegate int RendererSetupCallback(IntPtr rendererDescPtr, int idx);

		[MonoPInvokeCallback(typeof(RendererSetupCallback))]
		public static int OnNewBillboardRendererSetup(IntPtr rendererDescPtr, int idx)
		{
			SBatchDesc batchDesc = null;

			unsafe
			{
				SPopcornRendererDesc* desc = (SPopcornRendererDesc*)rendererDescPtr.ToPointer();
				batchDesc = new SBatchDesc(ERendererType.Billboard, *desc);
			}

			// Create the material description:
			Material mat = PKFxSettings.MaterialFactory.ResolveParticleMaterial(batchDesc);

			GameObject renderingObject = GetNewRenderingObject(batchDesc.m_GeneratedName);
			return SetupRenderingObject(renderingObject, batchDesc, mat);
		}

		//----------------------------------------------------------------------------
		// Ribbon

		[MonoPInvokeCallback(typeof(RendererSetupCallback))]
		public static int OnNewRibbonRendererSetup(IntPtr rendererDescPtr, int idx)
		{
			SBatchDesc batchDesc = null;

			unsafe
			{
				SPopcornRendererDesc* desc = (SPopcornRendererDesc*)rendererDescPtr.ToPointer();
				batchDesc = new SBatchDesc(ERendererType.Ribbon, *desc);
			}

			// Create the material description:
			Material mat = PKFxSettings.MaterialFactory.ResolveParticleMaterial(batchDesc);

			GameObject renderingObject = GetNewRenderingObject(batchDesc.m_GeneratedName);
			return SetupRenderingObject(renderingObject, batchDesc, mat);
		}

		//----------------------------------------------------------------------------
		// Mesh

		[MonoPInvokeCallback(typeof(RendererSetupCallback))]
		public static int OnNewMeshRendererSetup(IntPtr rendererDescPtr, int idx)
		{
			SBatchDesc batchDesc = null;

			unsafe
			{
				SMeshRendererDesc* desc = (SMeshRendererDesc*)rendererDescPtr.ToPointer();

				batchDesc = new SBatchDesc(*desc);
			}

			// Create the material description:
			Material mat = PKFxSettings.MaterialFactory.ResolveParticleMaterial(batchDesc);

			if (mat == null)
			{
				Debug.LogError("Could not find the material for renderer " + batchDesc.GenerateNameFromDescription());
				return -1;
			}

			if (PKFxSettings.UseMeshInstancing)
				mat.enableInstancing = true;
			else
				mat.enableInstancing = false;
			GameObject renderingObject = GetNewRenderingObject(batchDesc.m_GeneratedName);
			return SetupMeshRenderingObject(renderingObject, batchDesc, mat);
		}

		//----------------------------------------------------------------------------
		// Triangle

		[MonoPInvokeCallback(typeof(RendererSetupCallback))]
		public static int OnNewTriangleRendererSetup(IntPtr rendererDescPtr, int idx)
		{
			SBatchDesc batchDesc = null;

			unsafe
			{
				SPopcornRendererDesc* desc = (SPopcornRendererDesc*)rendererDescPtr.ToPointer();
				batchDesc = new SBatchDesc(ERendererType.Triangle, *desc);
			}

			// Create the material description:
			Material mat = PKFxSettings.MaterialFactory.ResolveParticleMaterial(batchDesc);

			GameObject renderingObject = GetNewRenderingObject(batchDesc.m_GeneratedName);
			return SetupRenderingObject(renderingObject, batchDesc, mat);
		}

		//----------------------------------------------------------------------------

		private delegate bool RendererResizeCallback(int rendererGUID, int particleCount, int vertexCount, int indexCount);
		[MonoPInvokeCallback(typeof(RendererResizeCallback))]
		public static bool OnRendererResize(int rendererGUID, int particleCount, int vertexCount, int indexCount)
		{
			Debug.Assert(m_Renderers.Count > rendererGUID);
			Debug.Assert(rendererGUID != -1);

			if (rendererGUID >= m_Renderers.Count)
			{
				Debug.LogError("[PopcornFX] Invalid renderer guid on renderer resize.");
				return false;
			}
			SMeshDesc renderer = m_Renderers[rendererGUID];
			if (renderer.m_Procedural != null)
				return OnRendererComputeBufferResize(rendererGUID, particleCount, vertexCount, indexCount);
			else
				return OnRendererMeshResize(rendererGUID, vertexCount, indexCount);
		}

		private delegate void SetParticleCountCallback(int rendererGUID, int particleCount);
		[MonoPInvokeCallback(typeof(RendererResizeCallback))]
		public static void OnSetParticleCount(int rendererGUID, int particleCount)
		{
			Debug.Assert(m_Renderers.Count > rendererGUID);
			Debug.Assert(rendererGUID != -1);
			SMeshDesc renderer = m_Renderers[rendererGUID];
			if (renderer.m_Procedural != null)
				renderer.m_Procedural.ParticleCount = particleCount;
		}

		public static bool OnRendererComputeBufferResize(int rendererGUID, int particleCount, int vertexCount, int indexCount)
		{
			Debug.Assert(m_Renderers.Count > rendererGUID);
			Debug.Assert(rendererGUID != -1);
			SMeshDesc renderer = m_Renderers[rendererGUID];

			Debug.Assert(renderer.m_Procedural != null);

			// Load the default size configuration
			string particleMeshGeneratedName = renderer.m_BatchDesc.m_GeneratedName;
			PKFxSettings.SParticleMeshDefaultSize meshSizeToUpdate = GetParticleMeshDefaultSizeSettings(particleMeshGeneratedName);

			uint usedVertexCount = (uint)vertexCount;
			uint usedIndexCount = (uint)indexCount;
			uint reserveVertexCount = usedVertexCount;
			uint reserveIndexCount = usedIndexCount;
			// If we need to grow buffer but did not find a setting for the buffer size
			{
				reserveVertexCount += (uint)(usedVertexCount);
				reserveIndexCount += (uint)(usedIndexCount);
				reserveIndexCount += 3 - (reserveIndexCount % 3);

				if (meshSizeToUpdate == null) // Did not find a setting for this buffer
				{
#if UNITY_EDITOR
					if (PKFxSettings.AutomaticMeshResizing)
					{
						// If in editor mode, we register the size of the buffers in a new particle mesh default size entry
						meshSizeToUpdate = new PKFxSettings.SParticleMeshDefaultSize();

						meshSizeToUpdate.m_GeneratedName = particleMeshGeneratedName;
						meshSizeToUpdate.m_DefaultVertexBufferSize = (int)reserveVertexCount;
						meshSizeToUpdate.m_DefaultIndexBufferSize = (int)reserveIndexCount;
						PKFxSettings.MeshesDefaultSize.Add(meshSizeToUpdate);
					}
#endif
				}
				else // We did find a setting for this buffer
				{
					if (usedVertexCount < meshSizeToUpdate.m_DefaultVertexBufferSize) // The registered
					{
						reserveVertexCount = (uint)meshSizeToUpdate.m_DefaultVertexBufferSize; // Otherwise, just update the reserved count
						usedVertexCount = reserveVertexCount; // Forces the mesh to get resized to this size if we found a config
					}
#if UNITY_EDITOR
					else if (PKFxSettings.AutomaticMeshResizing)
						meshSizeToUpdate.m_DefaultVertexBufferSize = (int)reserveVertexCount; // Update the settings IFN
#endif

					if (usedIndexCount < meshSizeToUpdate.m_DefaultIndexBufferSize)
					{
						reserveIndexCount = (uint)meshSizeToUpdate.m_DefaultIndexBufferSize; // Otherwise, just update the reserved count
						usedIndexCount = reserveIndexCount; // Forces the mesh to get resized to this size if we found a config
					}
#if UNITY_EDITOR
					else if (PKFxSettings.AutomaticMeshResizing)
						meshSizeToUpdate.m_DefaultIndexBufferSize = (int)reserveIndexCount; // Update the settings IFN
#endif
				}
			}
			//TODO Invalid logic.
			if (meshSizeToUpdate != null)
				renderer.m_Procedural.m_Bounds = meshSizeToUpdate.m_StaticWorldBounds; // Unsure if bounds is correct
			renderer.m_Procedural.GUID = rendererGUID;
			return renderer.m_Procedural.ResizeParticleComputeBuffer((uint)particleCount, usedVertexCount, usedIndexCount, reserveVertexCount, reserveIndexCount);
		}

		public static bool OnRendererMeshResize(int rendererGUID, int vertexCount, int indexCount)
		{
			Debug.Assert(m_Renderers.Count > rendererGUID);
			Debug.Assert(indexCount % 3 == 0);

			SMeshDesc renderer = m_Renderers[rendererGUID];
			MeshFilter filter = renderer.m_Slice;

			Debug.Assert(filter != null);

			// Load the default size configuration
			string particleMeshGeneratedName = renderer.m_BatchDesc.m_GeneratedName;
			PKFxSettings.SParticleMeshDefaultSize meshSizeToUpdate = GetParticleMeshDefaultSizeSettings(particleMeshGeneratedName);

			uint usedVertexCount = (uint)vertexCount;
			uint usedIndexCount = (uint)indexCount;
			uint reserveVertexCount = usedVertexCount;
			uint reserveIndexCount = usedIndexCount;

			Bounds initBounds = new Bounds();

			// If we need to grow buffer but did not find a setting for the buffer size
			{
				reserveVertexCount += (uint)(usedVertexCount * PKFxSettings.VertexBufferSizeMultiplicator);
				reserveIndexCount += (uint)(usedIndexCount * PKFxSettings.IndexBufferSizeMultiplicator);
				reserveIndexCount += 3 - (reserveIndexCount % 3);

				if (meshSizeToUpdate == null) // Did not find a setting for this buffer
				{
#if UNITY_EDITOR
					if (PKFxSettings.AutomaticMeshResizing)
					{
						// If in editor mode, we register the size of the buffers in a new particle mesh default size entry
						meshSizeToUpdate = new PKFxSettings.SParticleMeshDefaultSize();

						meshSizeToUpdate.m_GeneratedName = particleMeshGeneratedName;
						meshSizeToUpdate.m_DefaultVertexBufferSize = (int)reserveVertexCount;
						meshSizeToUpdate.m_DefaultIndexBufferSize = (int)reserveIndexCount;
						PKFxSettings.MeshesDefaultSize.Add(meshSizeToUpdate);
					}
#endif
				}
				else // We did find a setting for this buffer
				{
					if (usedVertexCount < meshSizeToUpdate.m_DefaultVertexBufferSize) // The registered
					{
						reserveVertexCount = (uint)meshSizeToUpdate.m_DefaultVertexBufferSize; // Otherwise, just update the reserved count
						usedVertexCount = reserveVertexCount; // Forces the mesh to get resized to this size if we found a config
					}
#if UNITY_EDITOR
					else if (PKFxSettings.AutomaticMeshResizing)
						meshSizeToUpdate.m_DefaultVertexBufferSize = (int)reserveVertexCount; // Update the settings IFN
#endif

					if (usedIndexCount < meshSizeToUpdate.m_DefaultIndexBufferSize)
					{
						reserveIndexCount = (uint)meshSizeToUpdate.m_DefaultIndexBufferSize; // Otherwise, just update the reserved count
						usedIndexCount = reserveIndexCount; // Forces the mesh to get resized to this size if we found a config
					}
#if UNITY_EDITOR
					else if (PKFxSettings.AutomaticMeshResizing)
						meshSizeToUpdate.m_DefaultIndexBufferSize = (int)reserveIndexCount; // Update the settings IFN
#endif
					initBounds = meshSizeToUpdate.m_StaticWorldBounds;
				}
			}

			// We choose to use large indices or not here:
			bool useLargeIndices = reserveVertexCount > UInt16.MaxValue;

			Mesh mesh = filter.mesh;

			mesh.bounds = initBounds;

			bool ok = ResizeParticleMeshBuffer(mesh,
												renderer,
												usedVertexCount,
												usedIndexCount,
												reserveVertexCount,
												reserveIndexCount,
												useLargeIndices);
			return ok; 
		}

		//----------------------------------------------------------------------------

		private static bool ResizeParticleMeshBuffer(Mesh mesh,
														SMeshDesc renderer,
														uint usedVertexCount,
														uint usedIndexCount,
														uint reservedVertexCount,
														uint reservedIndexCount,
														bool useLargeIdx)
		{
			bool hasBeenResized = false;

			if (renderer.m_VertexCount < usedVertexCount)
			{
				// We only do the transition from uint16 to uint32 because the transition clear the index buffer...
				if (useLargeIdx == true && mesh.indexFormat == IndexFormat.UInt16)
					mesh.indexFormat = IndexFormat.UInt32;
				else if (useLargeIdx == false && mesh.indexFormat == IndexFormat.UInt32)
					mesh.indexFormat = IndexFormat.UInt16;

				mesh.Clear();

				VertexAttribute additionalUVIdx = 0;

				List<VertexAttributeDescriptor> layout = new List<VertexAttributeDescriptor>();
				layout.Add(new VertexAttributeDescriptor(VertexAttribute.Position, VertexAttributeFormat.Float32, 3));			// positions

				if (renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_Lighting))
				{
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.Normal, VertexAttributeFormat.Float32, 3));		// normal
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.Tangent, VertexAttributeFormat.Float32, 4));		// tangent
				}
				if (renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_Color))
				{
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.Color, VertexAttributeFormat.Float32, 4));			// color
				}
				if (renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_RibbonComplex))
				{
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord0, VertexAttributeFormat.Float32, 2));		// uvFactors
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord1, VertexAttributeFormat.Float32, 2));		// uvScale
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord2, VertexAttributeFormat.Float32, 2));     // uvOffset
					additionalUVIdx = VertexAttribute.TexCoord3;
					if (renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaRemap))
					{
						layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord3, VertexAttributeFormat.Float32, 2));	// alpha cursor
						additionalUVIdx = VertexAttribute.TexCoord4;
					}
				}
				else if (renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_AnimBlend))
				{
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord0, VertexAttributeFormat.Float32, 2));		// uv0
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord1, VertexAttributeFormat.Float32, 2));		// uv1
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord2, VertexAttributeFormat.Float32, 2));     // atlas id and if Has_AlphaRemap, alpha cursor
					additionalUVIdx = VertexAttribute.TexCoord3;
				}
				else
				{
					if (renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_DiffuseMap) ||
						renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_DistortionMap))
					{
						layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord0, VertexAttributeFormat.Float32, 2)); // uv0
						additionalUVIdx = VertexAttribute.TexCoord1;
					}
					if (renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_AlphaRemap))
					{
						layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord1, VertexAttributeFormat.Float32, 2)); // alpha cursor
						additionalUVIdx = VertexAttribute.TexCoord2;
					}
				}

				if (renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_Emissive))
				{
					layout.Add(new VertexAttributeDescriptor(additionalUVIdx, VertexAttributeFormat.Float32, 3)); 				// emissive color
				}
				if (renderer.HasShaderVariationFlag(EShaderVariationFlags.Has_TransformUVs))
				{
					// Should be packed more IFP
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord6, VertexAttributeFormat.Float32, 2)); 	// TransformUVs rotate
					layout.Add(new VertexAttributeDescriptor(VertexAttribute.TexCoord7, VertexAttributeFormat.Float32, 4)); 	// TransformUVs ScaleAndOffset
				}

				mesh.SetVertexBufferParams((int)reservedVertexCount, layout.ToArray());
				renderer.m_VertexCount = (int)reservedVertexCount;

				hasBeenResized = true;
			}

			if (hasBeenResized || renderer.m_IndexCount < usedIndexCount)
			{
				mesh.SetIndexBufferParams((int)reservedIndexCount, mesh.indexFormat);
				renderer.m_IndexCount = (int)reservedIndexCount;

				mesh.subMeshCount = 1;
				SubMeshDescriptor desc = new SubMeshDescriptor();
				desc.topology = MeshTopology.Triangles;
				desc.firstVertex = 0;
				desc.baseVertex = 0;
				desc.indexStart = 0;
				desc.vertexCount = renderer.m_VertexCount;
				desc.indexCount = renderer.m_IndexCount;
				mesh.SetSubMesh(0, desc, MeshUpdateFlags.DontRecalculateBounds);

				hasBeenResized = true;
			}

			return hasBeenResized;
		}

		//----------------------------------------------------------------------------

		private delegate void SetRendererActiveCallback(int rendererGUID, bool active);

		[MonoPInvokeCallback(typeof(SetRendererActiveCallback))]
		public static void OnSetRendererActive(int rendererGUID, bool active)
		{
			if (m_Renderers.Count > rendererGUID)
			{
				SMeshDesc renderer = m_Renderers[rendererGUID];
				renderer.m_RenderingObject.SetActive(active);
			}
		}

		//----------------------------------------------------------------------------

		private delegate void SetMeshInstancesBufferCallback(int rendererGUID, int submesh, IntPtr instancesBuffer);

		[MonoPInvokeCallback(typeof(SetMeshInstancesBufferCallback))]
		public static void OnSetMeshInstancesBuffer(int rendererGUID, int submesh, IntPtr instancesBuffer)
		{
			Debug.Assert(m_Renderers[rendererGUID].m_InstancesRenderer != null);
			m_Renderers[rendererGUID].m_InstancesRenderer.SetInstanceBuffer(instancesBuffer, submesh);
		}

		//----------------------------------------------------------------------------

		private delegate void SetMeshInstancesCountCallback(int rendererGUID, int submesh, int instancesCount);

		[MonoPInvokeCallback(typeof(SetMeshInstancesCountCallback))]
		public static void OnSetMeshInstancesCount(int rendererGUID, int submesh, int instancesCount)
		{
			Debug.Assert(m_Renderers[rendererGUID].m_InstancesRenderer != null);
			m_Renderers[rendererGUID].m_InstancesRenderer.SetInstanceCount(instancesCount, submesh);
		}

		//----------------------------------------------------------------------------

		private delegate void SetLightsBufferCallback(IntPtr lightInfos, int count);

		[MonoPInvokeCallback(typeof(SetLightsBufferCallback))]
		public static void OnSetLightsBuffer(IntPtr lightInfos, int count)
		{
			if (PKFxManager.RenderingPlugin != null)
			{
				PKFxManager.RenderingPlugin.SetLightsBuffer(lightInfos, count);
			}
		}

		//----------------------------------------------------------------------------

		private delegate void SetSoundsBufferCallback(IntPtr soundInfos, int count);

		[MonoPInvokeCallback(typeof(SetSoundsBufferCallback))]
		public static void OnSetSoundsBuffer(IntPtr soundInfos, int count)
		{
			if (PKFxManager.RenderingPlugin != null)
			{
				PKFxManager.RenderingPlugin.SetSoundsBuffer(soundInfos, count);
			}
		}

		//----------------------------------------------------------------------------

		private delegate void RetrieveCustomMaterialInfoCallback(int type, IntPtr rendererDescPtr, int idx, IntPtr hasCustomMaterialPtr, IntPtr customMaterialIdPtr);

		[MonoPInvokeCallback(typeof(RetrieveRendererBufferInfoCallback))]
		public static void OnRetrieveCustomMaterialInfo(int type, IntPtr rendererDescPtr, int idx, IntPtr hasCustomMaterialPtr, IntPtr customMaterialIdPtr)
		{
			unsafe
			{
				int* hasCustomMat = (int*)hasCustomMaterialPtr;
				int* customMatID = (int*)customMaterialIdPtr;

				SBatchDesc batchDesc = null;
				if (type == 0 || type == 1 || type == 3)
				{
					SPopcornRendererDesc* desc = (SPopcornRendererDesc*)rendererDescPtr.ToPointer();
					batchDesc = new SBatchDesc((ERendererType)type, *desc);
				}
				else if (type == 2)
				{
					SMeshRendererDesc* desc = (SMeshRendererDesc*)rendererDescPtr.ToPointer();
					batchDesc = new SBatchDesc(*desc);
				}

				PKFxCustomMaterialInfo matInfo = m_CurrentlyBuildAsset.FindCustomMaterialInfo(batchDesc);

				if (matInfo == null || matInfo.m_CustomMaterial == null)
					*hasCustomMat = 0;
				else
				{
					*hasCustomMat = 1;
					*customMatID = matInfo.m_CustomMaterial.GetInstanceID();
				}
			}
		}

		private delegate void RetrieveRendererBufferInfoCallback(int rendererGUID, ref SRetrieveRendererInfo rendererInfo);

		[MonoPInvokeCallback(typeof(RetrieveRendererBufferInfoCallback))]
		public static void OnRetrieveRendererBufferInfo(int rendererGUID, ref SRetrieveRendererInfo rendererInfo)
		{
			if (rendererGUID >= m_Renderers.Count)
			{
				Debug.LogError("[PopcornFX] Invalid renderer guid  on RetrieveRendererBufferInfo.");
				return;
			}
			unsafe
			{
				int* isIdx32 = (int*)rendererInfo.m_IsIndex32.ToPointer();
				int* customMatID = (int*)rendererInfo.m_CustomMatID.ToPointer();
				int* vertexBufferSize = (int*)rendererInfo.m_VertexBufferSize.ToPointer();
				int* indexBufferSize = (int*)rendererInfo.m_IndexBufferSize.ToPointer();
				IntPtr* vbHandler = (IntPtr*)rendererInfo.m_VBHandler.ToPointer();
				IntPtr* ibHandler = (IntPtr*)rendererInfo.m_IBHandler.ToPointer();

				SMeshDesc renderer = m_Renderers[rendererGUID];

				if (rendererInfo.m_HasCustomMaterial != IntPtr.Zero)
				{
					int* hasCustomMat = (int*)rendererInfo.m_HasCustomMaterial.ToPointer();

					if (hasCustomMat != null && customMatID != null)
					{
						PKFxCustomMaterialInfo matInfo = m_CurrentlyBuildAsset.FindCustomMaterialInfo(renderer.m_BatchDesc);
						if (matInfo != null && matInfo.m_CustomMaterial != null)
						{
							*hasCustomMat = 1;
							*customMatID = matInfo.m_CustomMaterial.GetInstanceID();
						}
						else
							*hasCustomMat = 0;
					}
				}

				if (renderer.m_Slice != null)
				{
					int atlasesBSize = (int)renderer.NativeRawAtlasesBufferSize;
					int* atlasesBSizePtr = (int*)rendererInfo.m_AtlasesBSize.ToPointer();
					IntPtr* atlasesHandler = (IntPtr*)rendererInfo.m_AtlasesBHandler.ToPointer();

					Mesh currentRendererMesh = renderer.m_Slice.mesh;

					if (currentRendererMesh == null)
					{
						Debug.LogWarning("[PopcornFX] null mesh in renderer slice.");
						return;
					}

					int indexCount = renderer.m_IndexCount;
					int vertexCount = renderer.m_VertexCount;

					if (rendererInfo.m_UseComputeBuffers != IntPtr.Zero)
					{
						int* useComputebuffers = (int*)rendererInfo.m_UseComputeBuffers.ToPointer();
						if (useComputebuffers != null)
							*useComputebuffers = 0;
					}
					if (isIdx32 != null)
						*isIdx32 = currentRendererMesh.indexFormat == IndexFormat.UInt32 ? 1 : 0;
					if (vertexBufferSize != null)
						*vertexBufferSize = vertexCount;
					if (indexBufferSize != null)
						*indexBufferSize = indexCount;

					if (atlasesBSizePtr != null)
						*atlasesBSizePtr = atlasesBSize;

					if (vertexCount > 0 || indexCount > 0)
					{
						if (vbHandler != null)
							*vbHandler = currentRendererMesh.GetNativeVertexBufferPtr(0);
						if (ibHandler != null)
							*ibHandler = currentRendererMesh.GetNativeIndexBufferPtr();
						if (atlasesHandler != null)
							*atlasesHandler = renderer.GetNativeRawAtlasesBuffer();
					}
					else
					{
						if (vbHandler != null)
							*vbHandler = IntPtr.Zero;
						if (ibHandler != null)
							*ibHandler = IntPtr.Zero;
						if (atlasesHandler != null)
							*atlasesHandler = IntPtr.Zero;
					}
				}
				else if (renderer.m_Procedural != null)
				{
					int indexCount = (int)renderer.m_Procedural.ReservedIndexCount;
					int vertexCount = (int)renderer.m_Procedural.ReservedVertexCount;
					int infoBSize = (int)renderer.m_Procedural.NativeRawBillboardingInfoBufferSize;
					int atlasesBSize = (int)renderer.m_Procedural.NativeRawAtlasesBufferSize;
					int indirectArgsParticleCountMultiplier = renderer.m_Procedural.VertexMultiplier;

					int* infoBSizePtr = (int*)rendererInfo.m_InfoBSize.ToPointer();
					IntPtr* infoHandler = (IntPtr*)rendererInfo.m_InfoBHandler.ToPointer();
					int* atlasesBSizePtr = (int*)rendererInfo.m_AtlasesBSize.ToPointer();
					IntPtr* atlasesHandler = (IntPtr*)rendererInfo.m_AtlasesBHandler.ToPointer();
					int* indirectArgsParticleCountMultiplierPtr = (int*)rendererInfo.m_IndirectArgsParticleCountMultiplier.ToPointer();
					IntPtr* indirectArgsHandler = (IntPtr*)rendererInfo.m_IndirectArgsBHandler.ToPointer();

					if (rendererInfo.m_UseComputeBuffers != IntPtr.Zero)
					{
						int* useComputebuffers = (int*)rendererInfo.m_UseComputeBuffers.ToPointer();
						*useComputebuffers = 1;
					}
					if (isIdx32 != null)
						*isIdx32 = 1;
					if (vertexBufferSize != null)
						*vertexBufferSize = vertexCount;
					if (indexBufferSize != null)
						*indexBufferSize = indexCount;
					if (infoBSizePtr != null)
						*infoBSizePtr = infoBSize;
					if (atlasesBSizePtr != null)
						*atlasesBSizePtr = atlasesBSize;
					if (indirectArgsParticleCountMultiplierPtr != null)
						*indirectArgsParticleCountMultiplierPtr = indirectArgsParticleCountMultiplier;
					if (vertexCount > 0 || indexCount > 0)
					{
						if (vbHandler != null)
							*vbHandler = renderer.m_Procedural.GetNativeRawVertexBuffer();
						if (ibHandler != null)
							*ibHandler = renderer.m_Procedural.GetNativeRawIndexBuffer();
						if (infoHandler != null)
							*infoHandler = renderer.m_Procedural.GetNativeRawBillboardingInfoBuffer();
						if (atlasesHandler != null)
							*atlasesHandler = renderer.m_Procedural.GetNativeRawAtlasesBuffer();
						if (indirectArgsHandler != null && renderer.m_Procedural.DrawIndirectArgs != null)
							*indirectArgsHandler = renderer.m_Procedural.GetNativeRawArgumentBuffer();
					}
					else
					{
						if (vbHandler != null)
							*vbHandler = IntPtr.Zero;
						if (ibHandler != null)
							*ibHandler = IntPtr.Zero;
						if (infoHandler != null)
							*infoHandler = IntPtr.Zero;
						if (atlasesHandler != null)
							*atlasesHandler = IntPtr.Zero;
						if (indirectArgsHandler != null)
							*indirectArgsHandler = IntPtr.Zero;
					}
				}
			}
		}

		//----------------------------------------------------------------------------

		private delegate void RendererBoundsUpdateCallback(int rendererGUID, ref SUpdateRendererBounds bounds);

		[MonoPInvokeCallback(typeof(RendererBoundsUpdateCallback))]
		public static void OnRendererBoundsUpdate(int rendererGUID, ref SUpdateRendererBounds bounds)
		{
			if (rendererGUID == -1 || rendererGUID >= m_Renderers.Count)
				return;
			Bounds b = new Bounds();
			b.min = new Vector3(bounds.m_MinX, bounds.m_MinY, bounds.m_MinZ);
			b.max = new Vector3(bounds.m_MaxX, bounds.m_MaxY, bounds.m_MaxZ);

#if UNITY_EDITOR
			if (PKFxSettings.DebugEffectsBoundingBoxes && !PKFxSettings.UseGPUBillboarding)
			{
				Color boundsColor = m_Renderers[rendererGUID].m_BoundsDebugColor;

				Debug.DrawLine(new Vector3(bounds.m_MinX, bounds.m_MinY, bounds.m_MinZ), new Vector3(bounds.m_MaxX, bounds.m_MinY, bounds.m_MinZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MinX, bounds.m_MinY, bounds.m_MinZ), new Vector3(bounds.m_MinX, bounds.m_MaxY, bounds.m_MinZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MinX, bounds.m_MinY, bounds.m_MinZ), new Vector3(bounds.m_MinX, bounds.m_MinY, bounds.m_MaxZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MaxX, bounds.m_MaxY, bounds.m_MaxZ), new Vector3(bounds.m_MinX, bounds.m_MaxY, bounds.m_MaxZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MaxX, bounds.m_MaxY, bounds.m_MaxZ), new Vector3(bounds.m_MaxX, bounds.m_MinY, bounds.m_MaxZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MaxX, bounds.m_MaxY, bounds.m_MaxZ), new Vector3(bounds.m_MaxX, bounds.m_MaxY, bounds.m_MinZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MinX, bounds.m_MaxY, bounds.m_MaxZ), new Vector3(bounds.m_MinX, bounds.m_MinY, bounds.m_MaxZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MinX, bounds.m_MaxY, bounds.m_MaxZ), new Vector3(bounds.m_MinX, bounds.m_MaxY, bounds.m_MinZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MaxX, bounds.m_MinY, bounds.m_MinZ), new Vector3(bounds.m_MaxX, bounds.m_MaxY, bounds.m_MinZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MaxX, bounds.m_MinY, bounds.m_MinZ), new Vector3(bounds.m_MaxX, bounds.m_MinY, bounds.m_MaxZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MinX, bounds.m_MaxY, bounds.m_MinZ), new Vector3(bounds.m_MaxX, bounds.m_MaxY, bounds.m_MinZ), boundsColor);
				Debug.DrawLine(new Vector3(bounds.m_MaxX, bounds.m_MinY, bounds.m_MaxZ), new Vector3(bounds.m_MinX, bounds.m_MinY, bounds.m_MaxZ), boundsColor);
			}
#endif
			if (m_Renderers[rendererGUID].m_Slice != null)
				m_Renderers[rendererGUID].m_Slice.mesh.bounds = b;
			else if (m_Renderers[rendererGUID].m_Procedural != null)
				m_Renderers[rendererGUID].m_Procedural.m_Bounds = b;
		}


		private delegate int GetMeshCountCallback(int rendererGUID, int lod);

		[MonoPInvokeCallback(typeof(GetMeshCountCallback))]
		public static int OnGetMeshCount(int rendererGUID, int lod)
		{
			if (rendererGUID < 0)
				return -1;
			SMeshDesc desc = m_Renderers[rendererGUID];
			if (desc == null ||
				desc.m_InstancesRenderer == null ||
				desc.m_InstancesRenderer.m_PerLODsSubmeshCount == null)
				return -1;
			return desc.m_InstancesRenderer.m_PerLODsSubmeshCount[lod];
		}

		private delegate int GetMeshLODsCountCallback(int rendererGUID);

		[MonoPInvokeCallback(typeof(GetMeshCountCallback))]
		public static int OnGetMeshLODsCount(int rendererGUID)
		{
			if (rendererGUID < 0)
				return -1;
			SMeshDesc desc = m_Renderers[rendererGUID];
			if (desc == null ||
				desc.m_InstancesRenderer == null ||
				desc.m_InstancesRenderer.m_PerLODsSubmeshCount == null)
				return -1;
			return desc.m_InstancesRenderer.m_PerLODsSubmeshCount.Length;
		}

		private delegate void GetMeshBoundsCallback(int rendererGUID, int submesh, IntPtr bbox);

		[MonoPInvokeCallback(typeof(GetMeshCountCallback))]
		public static void OnGetMeshBounds(int rendererGUID, int submesh, IntPtr bbox)
		{
			if (rendererGUID < 0)
				return;
			SMeshDesc desc = m_Renderers[rendererGUID];
			if (desc == null ||
				desc.m_InstancesRenderer == null ||
				desc.m_InstancesRenderer.m_PerLODsSubmeshCount == null)
				return;
			Bounds bounds = desc.m_InstancesRenderer.Meshes[submesh].m_Mesh.bounds;
			Matrix4x4 trans = desc.m_InstancesRenderer.Meshes[submesh].m_ImportTransform;

			Vector3 scale = new Vector3(
				trans.GetColumn(0)[0],
				trans.GetColumn(1)[1],
				trans.GetColumn(2)[2]
			);
			Vector3 extents = bounds.extents;
			extents.Scale(scale);
			unsafe
			{
				Vector3* bbptr = (Vector3*)bbox.ToPointer();
				*bbptr = extents;
			}
		}

		//----------------------------------------------------------------------------

#if UNITY_EDITOR
		private delegate void EffectAboutToBakeCallback(string path);

		[MonoPInvokeCallback(typeof(EffectAboutToBakeCallback))]
		public static void OnEffectAboutToBake(string path)
		{
			PKFxManager.ImportedAssetPath = path;
			PKFxManager.ImportedAssetName = Path.GetFileName(path);
		}


		[MonoPInvokeCallback(typeof(AssetChangeCallback))]
		public static void OnAssetChange(ref SAssetChangesDesc assetChange)
		{
			string relativepath = "Assets" + PKFxSettings.UnityPackFxPath;
			string assetPath = assetChange.m_Path + ".asset";

			LockPackWatcherChanges();
			if (assetChange.m_Type == (int)EAssetChangesType.Add)
			{
				PKFxAssetCreationUtils.CreatePKFXAsset(assetChange);
			}
			else if (assetChange.m_Type == (int)EAssetChangesType.Remove)
			{
				AssetDatabase.DeleteAsset(Path.Combine(relativepath, assetPath));
			}
			else if (assetChange.m_Type == (int)EAssetChangesType.Update)
			{
				PKFxEffectAsset fxAsset = (PKFxEffectAsset)AssetDatabase.LoadAssetAtPath(Path.Combine(relativepath, assetPath), typeof(PKFxEffectAsset));
				if (fxAsset != null)
				{
					fxAsset.Clean();
					fxAsset.m_Data = File.ReadAllBytes(PKFxAssetCreationUtils.m_TempBakeDirectory + assetChange.m_Path);
					PKFxAssetCreationUtils.UpdatePKFXAsset(fxAsset, assetChange.m_Path);
				}
				else
				{
					PKFxAssetCreationUtils.CreatePKFXAsset(assetChange);
				}
			}
			else if (assetChange.m_Type == (int)EAssetChangesType.Rename)
			{
				AssetDatabase.RenameAsset(Path.Combine(relativepath, assetChange.m_PathOld + ".asset"), Path.GetFileName(assetChange.m_Path));
				PKFxEffectAsset fxAsset = (PKFxEffectAsset)AssetDatabase.LoadAssetAtPath(Path.Combine(relativepath, assetPath), typeof(PKFxEffectAsset));

				if (fxAsset != null)
				{
					fxAsset.Clean();
					fxAsset.m_Data = File.ReadAllBytes(PKFxAssetCreationUtils.m_TempBakeDirectory + assetChange.m_Path);
					PKFxAssetCreationUtils.UpdateAndRenamePKFXAsset(fxAsset, assetChange.m_PathOld, assetChange.m_Path);
				}
				else
				{
					PKFxAssetCreationUtils.CreatePKFXAsset(assetChange);
				}
			}
			UnlockPackWatcherChanges();
		}

		private delegate void GetAllAssetPathCallback([MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)]IntPtr[] pathArray, int size);

		[MonoPInvokeCallback(typeof(GetAllAssetPathCallback))]
		public static void OnGetAllAssetPath([MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] IntPtr[] pathArray, int size)
		{
			List<string> paths = new List<string>(size);

			for (int i = 0; i < size; ++i)
			{
				paths.Add(Marshal.PtrToStringAnsi(pathArray[i]));
			}
			PKFxSettings.AssetPathList = paths;
		}

		private delegate void PkkgExtractedCallback(string path);

		[MonoPInvokeCallback(typeof(PkkgExtractedCallback))]
		public static void OnPkkgExtracted(string path)
		{
			PKFxManager.ExtractedPkkg = path;
		}
#endif
	}
}